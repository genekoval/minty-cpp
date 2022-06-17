#include "date.h"

#include <date/date.h>
#include <date/tz.h>

using std::chrono::duration_cast;

namespace {
    constexpr auto unit_string = std::array {
        "year",
        "month",
        "week",
        "day",
        "hour",
        "minute",
        "second"
    };

    auto write_unit_string(std::ostream& os, int64_t count, int index) -> void {
        os << count << " " << unit_string[index];
        if (count != 1) os << "s";
    }
}

namespace minty::cli::output {
    auto format_date(const clock::time_point& date) -> std::string {
        const auto* zone = date::current_zone();
        const auto zoned = date::make_zoned(zone, date);

        auto out = std::ostringstream();
        out << date::format("%A, %B %d, %Y at %r %Z", zoned);

        return out.str();
    }

    auto format_duration(const clock::time_point& from) -> std::string {
        const auto d = clock::now() - from;

        const auto years = duration_cast<std::chrono::years>(d);
        const auto months = duration_cast<std::chrono::months>(d - years);
        const auto weeks =
            duration_cast<std::chrono::weeks>(d - years - months);
        const auto days =
            duration_cast<std::chrono::days>(d - years - months - weeks);
        const auto hours =
            duration_cast<std::chrono::hours>(
                d - years - months - weeks - days
            );
        const auto minutes =
            duration_cast<std::chrono::minutes>(
                d - years - months - weeks - days - hours
            );
        const auto seconds =
            duration_cast<std::chrono::seconds>(
                d - years - months - weeks - days - hours - minutes
            );

        const auto durations = std::array {
            years.count(),
            months.count(),
            weeks.count(),
            days.count(),
            hours.count(),
            minutes.count(),
            seconds.count()
        };

        const auto begin = durations.begin();
        const auto end = durations.end();

        const auto result = std::find_if(
            begin,
            end,
            [](auto i) { return i > 0; }
        );

        if (result == end) {
            return "just now";
        }

        const auto index = std::distance(begin, result);

        auto os = std::ostringstream();

        write_unit_string(os, *result, index);

        const auto next = std::next(result);

        if (next != end) {
            os << ", ";
            write_unit_string(os, *next, index + 1);
        }

        os << " ago";
        return os.str();
    }

    auto parse_date(const std::string& date) -> clock::time_point {
        auto in = std::istringstream(date);
        auto result = std::chrono::system_clock::time_point();

        date::from_stream(in, "%Y-%m-%d %H:%M:%S%z", result);

        return result;
    }
}
