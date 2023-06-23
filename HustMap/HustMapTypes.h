#pragma once

#include <QColor>
#include <QPointF>
#include <map>
#include <vector>

using Index = size_t;
using Coord = QPointF;

struct Stop : public Coord {
};
struct Line : public std::vector<Index> {
};
struct StopVec : public std::vector<Stop> {
    using std::vector<Stop>::vector;

    [[nodiscard]] auto find_closest_stop(Coord) const -> Index;
    static auto from_qfile(const std::string&) -> StopVec;
    static auto from_stream(std::istream&) -> StopVec;
};
struct LineVec : public std::vector<Line> {
    using std::vector<Line>::vector;

    [[nodiscard]] auto find_lines_by_stop(Index stop) const -> std::vector<Index>;
    static auto from_qfile(const std::string&) -> LineVec;
    static auto from_stream(std::istream&) -> LineVec;
};
struct StopRoute : public std::vector<Index> {
    using std::vector<Index>::vector;
    size_t transfers { 0 };
    QColor color { rand() % 256, rand() % 256, rand() % 256 };
};
struct LineRoute : public std::vector<Index> {
    using std::vector<Index>::vector;
};
struct Organization : public std::map<std::string, Coord> {
    using std::map<std::string, Coord>::map;
    static auto from_qfile(const std::string&) -> Organization;
    static auto from_stream(std::istream&) -> Organization;
    [[nodiscard]] auto to_qstring() const -> QString;
    [[nodiscard]] static auto to_qstring(const_iterator it) -> QString;
};

extern std::istream& operator>>(std::istream& is, Stop& Stop);
extern std::ostream& operator<<(std::ostream& os, const Stop& Stop);
extern std::istream& operator>>(std::istream& is, Line& line);
extern std::ostream& operator<<(std::ostream& os, const Line& line);
extern std::istream& operator>>(std::istream& is, StopVec& Stops);
extern std::ostream& operator<<(std::ostream& os, const StopVec& Stops);
extern std::istream& operator>>(std::istream& is, LineVec& lines);
extern std::ostream& operator<<(std::ostream& os, const LineVec& lines);
extern std::istream& operator>>(std::istream& is, Organization& org);
extern std::ostream& operator<<(std::ostream& os, const Organization& org);
