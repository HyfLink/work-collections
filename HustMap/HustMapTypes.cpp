#include "HustMapTypes.h"
#include <QFile>
#include <sstream>

extern std::istream &operator>>(std::istream &is, Stop &stop) { return is >> stop.rx() >> stop.ry(); }
extern std::ostream &operator<<(std::ostream &os, const Stop &stop) { return os << stop.x() << ' ' << stop.y(); }
extern std::istream &operator>>(std::istream &is, Line &line) {
    line.clear();
    std::string buffer{};
    std::getline(is, buffer);
    std::stringstream isstream{ buffer };
    auto              first = std::istream_iterator<Index>{ isstream };
    auto              last  = std::istream_iterator<Index>{};
    std::copy(first, last, std::back_inserter(line));
    std::for_each(line.begin(), line.end(), [](Index &idx) { --idx; });
    return is;
}

extern std::ostream &operator<<(std::ostream &os, const Line &line) {
    auto result = std::ostream_iterator<Index>{ os, " " };
    std::copy(line.begin(), line.end(), result);
    return os;
}

extern std::istream &operator>>(std::istream &is, StopVec &Stops) {
    size_t count = 0;
    is >> count;

    Stops.clear();
    Stops.reserve(count);

    auto first = std::istream_iterator<Stop>{ is };
    auto last  = std::istream_iterator<Stop>{};
    std::copy(first, last, std::back_inserter(Stops));

    return is;
}

extern std::ostream &operator<<(std::ostream &os, const StopVec &Stops) {
    auto result = std::ostream_iterator<Stop>{ os, "\n" };
    std::copy(Stops.begin(), Stops.end(), result);
    return os;
}

extern std::istream &operator>>(std::istream &is, LineVec &lines) {
    std::string buffer;
    size_t      count = 0;
    is >> count;

    lines.clear();
    lines.reserve(count);
    std::getline(is, buffer);

    auto first = std::istream_iterator<Line>{ is };
    auto last  = std::istream_iterator<Line>{};
    std::copy(first, last, std::back_inserter(lines));

    return is;
}

extern std::ostream &operator<<(std::ostream &os, const LineVec &lines) {
    auto result = std::ostream_iterator<Line>{ os, "\n" };
    std::copy(lines.begin(), lines.end(), result);
    return os;
}

extern std::istream &operator>>(std::istream &is, Organization &org) {
    std::string name;
    char        comma;
    size_t      x, y;

    for (org.clear(); is >> name >> x >> comma >> y;) {
        org.insert({ std::move(name), Coord{ static_cast<qreal>(x), static_cast<qreal>(y) } });
    }

    return is;
}

extern std::ostream &operator<<(std::ostream &os, const Organization &org) {
    for (const auto &[name, pos] : org) { os << name << ' ' << pos.x() << ',' << pos.y() << '\n'; }
    return os;
}

auto StopVec::from_qfile(const std::string &path) -> StopVec {
    StopVec result;
    QFile   file(path.c_str());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    std::istringstream{ file.readAll().data() } >> result;
    return result;
}

auto StopVec::from_stream(std::istream &is) -> StopVec {
    StopVec result;
    is >> result;
    return result;
}

auto StopVec::find_closest_stop(Coord p) const -> Index {
    return std::min_element(begin(), end(),
                            [p](const Stop &x, const Stop &y) {
                                const int x1 = static_cast<int>(x.x() - p.x()), x2 = static_cast<int>(y.x() - p.x());
                                const int y1 = static_cast<int>(x.y() - p.y()), y2 = static_cast<int>(y.y() - p.y());
                                return x1 * x1 + y1 * y1 < x2 * x2 + y2 * y2;
                            }) -
           begin();
}

auto LineVec::from_qfile(const std::string &path) -> LineVec {
    LineVec result;
    QFile   file(path.c_str());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    std::istringstream{ file.readAll().data() } >> result;
    return result;
}

auto LineVec::from_stream(std::istream &is) -> LineVec {
    LineVec result;
    is >> result;
    return result;
}

auto LineVec::find_lines_by_stop(Index stop) const -> std::vector<Index> {
    std::vector<Index> result;
    for (size_t i = 0; i < size(); ++i) {
        if (std::binary_search(at(i).begin(), at(i).end(), stop)) { result.push_back(i); }
    }
    return result;
}

auto Organization::from_qfile(const std::string &path) -> Organization {
    Organization result;
    QFile        file(path.c_str());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    std::istringstream{ file.readAll().data() } >> result;
    return result;
}

auto Organization::from_stream(std::istream &is) -> Organization {
    Organization result;
    is >> result;
    return result;
}

auto Organization::to_qstring() const -> QString {
    QString result;
    for (const auto &[name, pos] : *this) {
        result.append(name.c_str());
        result.append('\n');
    }
    return result;
}

auto Organization::to_qstring(const_iterator it) -> QString {
    QString result;
    result.append(it->first.c_str());
    result.append('\n');
    return result;
}
