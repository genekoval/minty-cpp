#include "preview.h"
#include "video/video.h"

namespace minty::core {
    auto generate_audio_preview(
        object_store& objects,
        const fstore::object_meta& object
    ) -> std::optional<std::string> {
        const auto source = objects.get(object.id);

        auto io = video::io_context(source.span());
        auto format = video::format_context(io.data());

        AVStream* stream = nullptr;

        try {
            stream = format.find_video_stream();
        }
        catch (const video::stream_not_found& ex) {
            DEBUG()
                << "Cannot get preview for audio file ("
                << object.id << "): " << ex.what();
            return {};
        }

        auto packet = video::packet();

        while (format.read_frame(packet.data())) {
            if (!packet.stream(stream)) {
                packet.unref();
                continue;
            }

            auto* pkt = packet.data();

            return objects.add({}, pkt->size, [pkt](auto&& part) {
                part.write(std::span(
                    reinterpret_cast<const std::byte*>(pkt->data),
                    pkt->size
                ));
            }).id;
        }

        return {};
    }
}
