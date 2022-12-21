#include "video.hpp"

#include <stdexcept>

namespace minty::core::video {
    sws_context::sws_context(
        AVCodecContext* codec,
        AVPixelFormat pixel_format
    ) :
        ctx(sws_getContext(
            codec->width,
            codec->height,
            codec->pix_fmt,
            codec->width,
            codec->height,
            pixel_format,
            SWS_BILINEAR,
            nullptr,
            nullptr,
            nullptr
        )),
        height(codec->height)
    {
        if (!ctx) throw std::runtime_error(
            "Failed to allocate scaling context"
        );
    }

    sws_context::~sws_context() {
        sws_freeContext(ctx);
    }

    auto sws_context::scale(AVFrame* src, AVFrame* dest) -> void {
        sws_scale(
            ctx,
            src->data,
            src->linesize,
            0,
            height,
            dest->data,
            dest->linesize
        );
    }
}
