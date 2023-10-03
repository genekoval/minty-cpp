#include "preview.hpp"
#include "video/video.hpp"

namespace minty::core {
    auto generate_audio_preview(bucket& bucket, const fstore::object& object)
        -> ext::task<std::optional<UUID::uuid>> {
        auto source = co_await bucket.get(object.id);

        auto io = video::io_context(source);
        auto format = video::format_context(io.data());

        AVStream* stream = nullptr;

        try {
            stream = format.find_video_stream();
        }
        catch (const video::stream_not_found& ex) {
            TIMBER_DEBUG(
                "Cannot get preview for audio file ({}): {}",
                object.id,
                ex.what()
            );
            co_return std::optional<UUID::uuid>();
        }

        auto packet = video::packet();

        while (format.read_frame(packet.data())) {
            if (!packet.stream(stream)) {
                packet.unref();
                continue;
            }

            auto* pkt = packet.data();

            const auto obj = co_await bucket.add(
                {},
                pkt->size,
                [pkt](auto&& part) -> ext::task<> {
                    co_await part.write(std::span(
                        reinterpret_cast<const std::byte*>(pkt->data),
                        pkt->size
                    ));
                }
            );
            ;

            co_return obj.id;
        }

        co_return std::optional<UUID::uuid>();
    }
}
