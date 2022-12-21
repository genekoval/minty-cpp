#include "video.hpp"

#include <stdexcept>

namespace minty::core::video {
    frame::frame() : m_frame(av_frame_alloc()) {
        if (!m_frame) throw std::runtime_error(
            "Could not allocate frame"
        );
    }

    frame::~frame() {
        av_frame_free(&m_frame);
    }

    auto frame::data() -> AVFrame* {
        return m_frame;
    }
}
