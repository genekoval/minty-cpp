#include <span>
#include <stdexcept>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

namespace minty::core::video {
    auto allocate_buffer(std::size_t size) -> uint8_t*;

    class stream_not_found : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    class av_buffer {
        uint8_t* buffer;
    public:
        av_buffer(std::size_t size);

        ~av_buffer();

        auto data() -> uint8_t*;
    };

    class codec_context {
        AVCodec* codec;
        AVCodecContext* ctx = nullptr;

        auto receive_frame(AVFrame* frame) -> bool;
    public:
        codec_context(AVCodec* coec);

        ~codec_context();

        auto copy_params(AVCodecParameters* params) -> void;

        auto data() -> AVCodecContext*;

        auto decode(AVPacket* packet, AVFrame* frame) -> bool;

        auto open() -> void;
    };

    class frame {
        AVFrame* m_frame;
    public:
        frame();

        ~frame();

        auto data() -> AVFrame*;
    };

    class format_context {
        AVFormatContext* ctx;

        auto open() -> void;
    public:
        format_context(AVIOContext* io);

        ~format_context();

        auto data() -> AVFormatContext*;

        auto find_video_stream(AVCodec** decoder = nullptr) -> AVStream*;

        auto read_frame(AVPacket* packet) -> bool;
    };

    class io_context {
        struct buffer_data {
            uint8_t* data = nullptr;
            std::size_t remaining = 0;
            const std::size_t size = 0;
            uint8_t* const start;
        };

        static auto read_packet(
            void* opaque,
            uint8_t* buffer,
            int buffer_size
        ) -> int;

        static auto seek(
            void* opaque,
            int64_t offset,
            int whence
        ) -> int64_t;

        buffer_data bd;
        uint8_t* buffer;
        AVIOContext* ctx;
    public:
        io_context(std::span<std::byte> data);

        ~io_context();

        auto data() -> AVIOContext*;
    };

    class packet {
        AVPacket* pkt;
    public:
        packet();

        ~packet();

        auto data() -> AVPacket*;

        auto stream(AVStream* stream) -> bool;

        auto unref() -> void;
    };

    class sws_context {
        SwsContext* ctx;
        int height;
    public:
        sws_context(
            AVCodecContext* codec,
            AVPixelFormat pixel_format
        );

        ~sws_context();

        auto scale(AVFrame* src, AVFrame* dest) -> void;
    };
}
