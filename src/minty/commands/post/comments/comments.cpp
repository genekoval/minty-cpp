#include "../commands.h"
#include "../../../client.h"
#include "../../../parser/parser.h"

#include <date/date.h>
#include <date/tz.h>
#include <ext/string.h>
#include <fmt/color.h>

using namespace commline;

namespace {
    namespace internal {
        namespace color {
            constexpr auto secondary = fmt::color::dim_gray;
        }

        class printer {
            const int indent_amount;
            const int spacing;

            auto indent(int amount) const -> void {
                for (auto i = 0; i < amount; ++i) {
                    fmt::print(
                        fmt::fg(color::secondary),
                        "{}\u2502 ",
                        std::string(spacing, ' ')
                    );
                }
            }
        public:
            printer(int indent_amount, int spacing) :
                indent_amount(indent_amount),
                spacing(spacing)
            {}

            auto newlines(int amount) const -> void {
                for (auto i = 0; i < amount; ++i) {
                    indent(indent_amount);
                    fmt::print("\n");
                }
            }

            template <typename ...Args>
            auto print(
                std::string_view format_string,
                Args&&... args
            ) const -> void {
                indent(indent_amount);
                fmt::print(
                    fmt::runtime(format_string),
                    std::forward<Args>(args)...
                );
            }

            template <typename ...Args>
            auto print_secondary(
                std::string_view format_string,
                Args&&... args
            ) const -> void {
                indent(indent_amount);
                fmt::print(
                    fmt::fg(color::secondary),
                    format_string,
                    std::forward<Args>(args)...
                );
            }

            auto separator() const -> void {
                if (indent_amount <= 0) return;
                indent(indent_amount - 1);
                fmt::print("{}", std::string(spacing, ' '));

                fmt::print(fmt::fg(color::secondary), "\u250c");

                for (auto i = 0; i < 5; ++i) {
                    fmt::print(fmt::fg(color::secondary), "\u2500");
                }

                fmt::print("\n");
            }
        };

        auto format_date(
            const std::string& date_string,
            const date::time_zone* zone
        ) -> std::string {
            auto in = std::istringstream(date_string);
            auto time = std::chrono::system_clock::time_point();

            date::from_stream(in, "%Y-%m-%d %H:%M:%S%z", time);

            const auto zoned = date::make_zoned(zone, time);

            auto out = std::ostringstream();
            out << date::format("%A, %B %d, %Y at %r %Z", zoned);

            return out.str();
        }

        auto comments(
            const app& app,
            int spacing,
            long width,
            const date::time_zone* time_zone,
            const UUID::uuid& post
        ) -> void {
            auto api = minty::cli::client();

            const auto comment_list = api.get_comments(post);

            for (const auto& comment : comment_list) {
                const auto p = printer(comment.indent, spacing);

                p.separator();
                p.print_secondary("{}\n", comment.id);
                p.print_secondary("{}\n", format_date(
                    comment.date_created,
                    time_zone
                ));

                auto range = ext::string_range(comment.content, "\n");

                for (const auto& line : range) {
                    if (line.empty()) {
                        p.print("\n");
                        continue;
                    }

                    auto index = 0;
                    auto size = static_cast<long>(line.size());

                    while (size > 0) {
                        auto len = std::min(width, size);

                        // Remove spaces from the beginning of the line
                        while (std::isspace(line[index])) ++index;

                        // Avoid breaking words
                        if (len < size && !std::isspace(line[len + index])) {
                            while (!std::isspace(line[len + index])) --len;
                        }

                        p.print("{}\n", line.substr(index, len));
                        index += len;
                        size -= len;
                    }
                }

                p.newlines(1);
            }
        }
    }
}

namespace commline {
    template <>
    struct parser<const date::time_zone*> {
        static auto parse(std::string_view argument) -> const date::time_zone* {
            return date::locate_zone(argument);
        }
    };
}

namespace minty::subcommands::post {
    auto comments() -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Get a post's comments",
            options(
                option<int>(
                    {"s", "spacing"},
                    "Amount of spaces to indent comments",
                    "number",
                    2
                ),
                option<long>(
                    {"w", "width"},
                    "Max width of comment content",
                    "number",
                    80
                ),
                option<const date::time_zone*>(
                    {"z", "zone"},
                    "Time zone to use for comment timestamps",
                    "time zone",
                    date::current_zone()
                )
            ),
            arguments(
                required<UUID::uuid>("post")
            ),
            internal::comments
        );
    }
}
