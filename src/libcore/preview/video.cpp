#include "preview.h"
#include "video/video.h"

namespace {
    constexpr auto align = 1;

    constexpr auto pixel_format = AV_PIX_FMT_RGB24;

    auto save_image(
        minty::core::object_store& objects,
        AVCodecContext* codec,
        AVFrame* frame
    ) -> std::string {
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

        return minty::core::generate_image_preview(
            objects,
            codec->width,
            codec->height,
            frame_rgb.data()->data[0]
        );
    }
}

namespace minty::core {
    auto generate_video_preview(
        object_store& objects,
        const fstore::object_meta& object
    ) -> std::string {
        const auto source = objects.get(object.id);

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
            if (pkt.stream(stream)) {
                if (codec.decode(pkt.data(), frame.data())) {
                    if (frame.data()->key_frame) {
                        return save_image(objects, codec.data(), frame.data());
                    }
                }
            }

            pkt.unref();
        }

        throw std::runtime_error("Failed to obtain video frame for preview");
    }
}
