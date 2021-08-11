#include "video.h"

#include <cstring>
#include <ext/data_size.h>
#include <stdexcept>

using namespace ext::literals;

namespace {
    constexpr auto buffer_size = 4_KiB;
}

namespace minty::core::video {
    io_context::io_context(std::span<std::byte> data) :
        bd {
            .data = reinterpret_cast<uint8_t*>(data.data()),
            .remaining = data.size(),
            .size = data.size(),
            .start = reinterpret_cast<uint8_t*>(data.data())
        },
        buffer(allocate_buffer(buffer_size)),
        ctx(avio_alloc_context(
            buffer,
            buffer_size,
            0,
            &bd,
            &read_packet,
            nullptr,
            &seek
        ))
    {
        if (!ctx) {
            av_freep(buffer);
            throw std::runtime_error("Failed to allocate IO context");
        }
    }

    io_context::~io_context() {
        if (ctx) av_freep(&ctx->buffer);
        avio_context_free(&ctx);
    }

    auto io_context::data() -> AVIOContext* {
        return ctx;
    }

    auto io_context::read_packet(
        void* opaque,
        uint8_t* buffer,
        int buffer_size
    ) -> int {
        auto* bd = static_cast<buffer_data*>(opaque);
        buffer_size = std::min(
            static_cast<std::size_t>(buffer_size),
            bd->size
        );

        if (!buffer_size) return AVERROR_EOF;

        std::memcpy(buffer, bd->data, buffer_size);
        bd->data += buffer_size;
        bd->remaining -= buffer_size;

        return buffer_size;
    }

    auto io_context::seek(
        void* opaque,
        int64_t offset,
        int whence
    ) -> int64_t {
        auto* bd = static_cast<buffer_data*>(opaque);

        const auto size = whence & AVSEEK_SIZE;
        if (size == AVSEEK_SIZE) return bd->size;

        whence &= ~AVSEEK_FORCE;
        whence &= ~AVSEEK_SIZE;

        if (whence == SEEK_SET) {
            bd->data = bd->start + offset;
            bd->remaining = bd->size - offset;
        }
        else if (whence == SEEK_CUR) {
            bd->data += offset;
            bd->remaining -= offset;
        }
        else if (whence == SEEK_END) {
            bd->data = bd->start + bd->size + offset;
            bd->remaining = 0 - offset;
        }

        return std::distance(bd->start, bd->data);
    }
}
