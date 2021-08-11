#include "video.h"

#include <stdexcept>

namespace minty::core::video {
    auto allocate_buffer(std::size_t size) -> uint8_t* {
        auto* result = av_malloc(size * sizeof(uint8_t));

        if (!result) throw std::runtime_error(
            "Failed to allocate buffer"
        );

        return reinterpret_cast<uint8_t*>(result);
    }

    av_buffer::av_buffer(std::size_t size) : buffer(allocate_buffer(size)) {
        if (!buffer) throw std::runtime_error("Failed to allocate buffer");
    }

    av_buffer::~av_buffer() {
        av_free(buffer);
    }

    auto av_buffer::data() -> uint8_t* {
        return buffer;
    }
}
