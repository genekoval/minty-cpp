#include "preview.hpp"
#include "video/video.hpp"

namespace {
    constexpr auto align = 1;

    constexpr auto pixel_format = AV_PIX_FMT_RGB24;

    auto save_image(
        minty::core::bucket& bucket,
        AVCodecContext* codec,
        AVFrame* frame
    ) -> ext::task<UUID::uuid> {
        auto frame_rgb = minty::core::video::frame();

        auto bytes = av_image_get_buffer_size(
            pixel_format,
            codec->width,
            codec->height,
            align
        );

        auto buffer = minty::core::video::av_buffer(bytes);

        av_image_fill_arrays(
            frame_rgb.data()->data,
            frame_rgb.data()->linesize,
            buffer.data(),
            pixel_format,
            codec->width,
            codec->height,
            align
        );

        auto sws = minty::core::video::sws_context(
            codec,
            pixel_format
        );

        sws.scale(frame, frame_rgb.data());

        co_return co_await minty::core::generate_image_preview(
            bucket,
            codec->width,
            codec->height,
            frame_rgb.data()->data[0]
        );
    }
}

namespace minty::core {
    auto generate_video_preview(
        bucket& bucket,
        const fstore::object& object
    ) -> ext::task<std::optional<UUID::uuid>> {
        const auto source = co_await bucket.get(object.id);

        auto io = video::io_context(source.span());
        auto format = video::format_context(io.data());

        AVCodec* decoder = nullptr;
        auto* stream = format.find_video_stream(&decoder);

        auto codec = video::codec_context(decoder);
        codec.copy_params(stream->codecpar);
        codec.open();

        auto pkt = video::packet();
        auto frame = video::frame();

        while (format.read_frame(pkt.data())) {
            if (!pkt.stream(stream)) {
                pkt.unref();
                continue;
            }

            if (codec.decode(pkt.data(), frame.data())) {
                if (frame.data()->key_frame) {
                    co_return co_await save_image(
                        bucket,
                        codec.data(),
                        frame.data()
                    );
                }
            }
        }

        throw std::runtime_error("Failed to obtain video frame for preview");
    }
}
