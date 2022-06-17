#include "../date.h"
#include "../model.h"

namespace style = minty::cli::output::style;

namespace {
    template <typename T>
    struct row {
        const std::string_view label;
        const T& value;
    };

    template <typename... Rows>
    class metadata {
        using alignment_type = std::size_t;
        using row_container = std::tuple<Rows...>;

        template <std::size_t... I>
        static auto compute_alignment(
            const row_container& rows,
            std::index_sequence<I...>
        ) -> alignment_type {
            auto result = static_cast<alignment_type>(0);

            ((result = std::max(result, std::get<I>(rows).label.size())), ...);

            return result;
        }

        const row_container rows;
        const alignment_type alignment;

        auto print_label(std::FILE* f, std::string_view label) -> void {
            fmt::print(
                f,
                style::label,
                "{:{}}",
                label,
                alignment
            );

            fmt::print(f, " {} ", style::interpunct);
        }

        template <typename T>
        auto print_value(std::FILE* f, const T& value) -> void {
            fmt::print(f, "{}\n", value);
        }

        template <typename T>
        auto print_row(
            std::FILE* f,
            std::string_view label,
            const T& value
        ) -> void {
            print_label(f, label);
            print_value(f, value);
        }

        template <typename T>
        auto print_row(
            std::FILE* f,
            std::string_view label,
            const std::vector<T>& value
        ) -> void {
            if (value.empty()) return;

            print_row(f, label, value.front());

            for (auto i = 1ul; i < value.size(); ++i) {
                const auto& current = value[i];
                const auto indent = alignment + 3;

                for (auto i = 0ul; i < indent; ++i) {
                    fmt::print(f, " ");
                }

                print_value(f, current);
            }
        }

        template <typename T>
        auto print_row(
            std::FILE* f,
            std::string_view label,
            const std::optional<T>& value
        ) -> void{
            if (value) print_row(f, label, *value);
        }

        template <typename Row>
        auto print_row(std::FILE* f, const Row& row) -> void {
            print_row(f, row.label, row.value);
        }

        template <std::size_t... I>
        auto print(std::FILE* f, std::index_sequence<I...>) -> void {
            (print_row(f, std::get<I>(rows)), ...);
        }
    public:
        metadata(Rows&&... rows) :
            rows(std::forward<Rows>(rows)...),
            alignment(compute_alignment(
                this->rows,
                std::index_sequence_for<Rows...>{}
            ))
        {}

        auto print(std::FILE* f) -> void {
            print(f, std::index_sequence_for<Rows...>{});
        }
    };
}

namespace minty::cli::output {
    auto human_readable<core::tag>::print(
        std::FILE* f,
        const core::tag& tag
    ) -> void {
        fmt::print(f, fmt::emphasis::bold, "{}\n\n", tag.name);

        if (!tag.aliases.empty()) {
            for (const auto& alias : tag.aliases) {
                fmt::print(f, "- {}\n", alias);
            }

            fmt::print(f, "\n");
        }

        if (tag.description) {
            fmt::print(f, style::secondary, style::divider);
            fmt::print(f, "{}\n", tag.description.value());
            fmt::print(f, style::secondary, style::divider);
        }

        if (!tag.sources.empty()) {
            fmt::print(f, "\n");

            for (const auto& source : tag.sources) {
                fmt::print(f, "- ");
                fmt::print(f, style::link, "{}\n", source.url);
            }

            fmt::print(f, "\n");
        }

        const auto date = parse_date(tag.date_created);
        const auto formatted = format_date(date);
        const auto duration = format_duration(date);
        const auto created = std::vector<std::string_view> {
            duration,
            formatted
        };

        metadata(
            row {"ID", tag.id},
            row {"Posts", tag.post_count},
            row {"Created", created}
        ).print(f);
    }
}
