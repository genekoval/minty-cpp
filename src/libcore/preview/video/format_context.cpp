#include "video.h"

#include <stdexcept>

namespace minty::core::video {
    auto format_context::open() -> void {
        if (avformat_open_input(
            &ctx,
            nullptr,
            nullptr,
            nullptr
        ) < 0) throw std::runtime_error("Failed to open file");

        if (avformat_find_stream_info(ctx, nullptr) < 0) {
            throw std::runtime_error("Failed to find stream information");
        }
    }

    format_context::format_context(AVIOContext* io) :
        ctx(avformat_alloc_context())
    {
        ctx->pb = io;
        open();
    }

    format_context::~format_context() {
        avformat_close_input(&ctx);
    }

    auto format_context::data() -> AVFormatContext* {
        return ctx;
    }

    auto format_context::find_video_stream(AVCodec** decoder) -> AVStream* {
        auto index = av_find_best_stream(
            ctx,
            AVMEDIA_TYPE_VIDEO,
            -1,
            -1,
            decoder,
            0
        );

        if (index == AVERROR_STREAM_NOT_FOUND) {
            throw std::runtime_error("Video stream not found");
        }

        if (index == AVERROR_DECODER_NOT_FOUND) {
            throw std::runtime_error("Unsupported codec");
        }

        if (index < 0) {
            throw std::runtime_error("Error finding best video stream");
        }

        return ctx->streams[index];
    }

    auto format_context::read_frame(AVPacket* packet) -> bool {
        const auto ret = av_read_frame(ctx, packet);
        if (ret == 0) return true;
        if (ret == AVERROR_EOF) return false;
        throw std::runtime_error("Error reading frame");
    }
}
