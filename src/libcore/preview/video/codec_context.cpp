#include "video.h"

#include <stdexcept>

namespace minty::core::video {
    codec_context::codec_context(AVCodec* codec) :
        codec(codec),
        ctx(avcodec_alloc_context3(codec))
    {
        if (!ctx) {
            throw std::runtime_error(
                "Could not allocate video codec context"
            );
        }
    }

    codec_context::~codec_context() {
        avcodec_free_context(&ctx);
    }

    auto codec_context::copy_params(AVCodecParameters* params) -> void {
        if (avcodec_parameters_to_context(ctx, params) < 0) {
            throw std::runtime_error(
                "Failed to copy codec parameters to context"
            );
        }
    }

    auto codec_context::data() -> AVCodecContext* {
        return ctx;
    }

    auto codec_context::decode(AVPacket* packet, AVFrame* frame) -> void {
        if (avcodec_send_packet(ctx, packet) < 0) {
            throw std::runtime_error("Error sending packet for decoding");
        }

        if (avcodec_receive_frame(ctx, frame) < 0) {
            throw std::runtime_error("Error during decoding");
        }
    }

    auto codec_context::open() -> void {
        if (avcodec_open2(ctx, codec, nullptr) < 0) {
            throw std::runtime_error("Could not open codec");
        }
    }
}
