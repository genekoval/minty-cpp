#include "video.hpp"

#include <sstream>
#include <stdexcept>

namespace minty::core::video {
    codec_context::codec_context(const AVCodec* codec) :
        codec(codec),
        ctx(avcodec_alloc_context3(codec)) {
        if (!ctx) {
            throw std::runtime_error("Could not allocate video codec context");
        }
    }

    codec_context::~codec_context() { avcodec_free_context(&ctx); }

    auto codec_context::copy_params(AVCodecParameters* params) -> void {
        if (avcodec_parameters_to_context(ctx, params) < 0) {
            throw std::runtime_error(
                "Failed to copy codec parameters to context"
            );
        }
    }

    auto codec_context::data() -> AVCodecContext* { return ctx; }

    auto codec_context::decode(AVPacket* packet, AVFrame* frame) -> bool {
        if (avcodec_send_packet(ctx, packet) < 0) {
            throw std::runtime_error("Error sending packet for decoding");
        }

        return receive_frame(frame);
    }

    auto codec_context::open() -> void {
        if (avcodec_open2(ctx, codec, nullptr) < 0) {
            throw std::runtime_error("Could not open codec");
        }
    }

    auto codec_context::receive_frame(AVFrame* frame) -> bool {
        const auto ret = avcodec_receive_frame(ctx, frame);

        if (ret == 0) return true;
        if (ret == AVERROR(EAGAIN)) return false;

        auto os = std::ostringstream();
        os << "Error during decoding:";

        if (ret == AVERROR_EOF) {
            os << " the decoder has been fully flushed,"
               << " and there will be no more output frames";
        }
        else if (ret == AVERROR(EINVAL)) {
            os << " codec not opened, or it is an encoder";
        }
        else if (ret == AVERROR_INPUT_CHANGED) {
            os << " current decoded frame has changed parameters"
               << " with respect to first decoded frame";
        }
        else { os << " legitimate decoding errors"; }

        throw std::runtime_error(os.str());
    }
}
