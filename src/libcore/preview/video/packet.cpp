#include "video.hpp"

#include <stdexcept>

namespace minty::core::video {
    packet::packet() : pkt(av_packet_alloc()) {
        if (!pkt) { throw std::runtime_error("Failed to allocate packet"); }
    }

    packet::~packet() { av_packet_free(&pkt); }

    auto packet::data() -> AVPacket* { return pkt; }

    auto packet::stream(AVStream* stream) -> bool {
        return pkt->stream_index == stream->index;
    }

    auto packet::unref() -> void { av_packet_unref(pkt); }
}
