#include "HustScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

constexpr int R = 3, D = 2 * R;

HustScene::HustScene(const StopVec &stops, const LineVec &lines, QObject *parent)
    : QGraphicsScene(parent), m_hustmap(":/assets/assets/hustmap.png"), m_stops(stops), m_lines(lines) {
    initialize_hustmap();
    clear();
}

void HustScene::initialize_hustmap() {
    QPainter painter(&m_hustmap);

    painter.setPen(QPen(QColor(64, 64, 64, 64), D));
    for (const auto &p : m_stops) {
        painter.drawEllipse(static_cast<int>(p.x()) - R, static_cast<int>(p.y()) - R, D, D);
    }

    for (const auto &line : m_lines) {
        painter.setPen(QPen(QColor(rand() % 256, rand() % 256, rand() % 256, 255), 1));
        for (auto it = line.begin(); it < line.end() - 1; ++it) { painter.drawLine(m_stops[*it], m_stops[*(it + 1)]); }
    }
}

void HustScene::clear() {
    QGraphicsScene::clear();
    QGraphicsScene::addPixmap(m_hustmap);
}

void HustScene::drawPostion(Coord pos, const QColor &color) {
    QGraphicsScene::addEllipse(pos.x() - R, pos.y() - R, D, D, QPen(color, D));
}

void HustScene::drawLine(Coord x, Coord y, const QColor &color) {
    QGraphicsScene::addLine(x.x(), x.y(), y.x(), y.y(), QPen(color, R));
}

void HustScene::drawRoute(const std::vector<Coord> &route, const QColor &color) {
    for (auto it = route.begin(); it < route.end(); ++it) { drawPostion(*it, color); }
    for (auto it = route.begin(); it < route.end() - 1; ++it) { drawLine(*it, *(it + 1), color); }
}

void HustScene::drawRoute(const std::vector<Index> &route, const QColor &color) {
    for (auto it = route.begin(); it < route.end(); ++it) { drawPostion(m_stops[*it], color); }
    for (auto it = route.begin(); it < route.end() - 1; ++it) { drawLine(m_stops[*it], m_stops[*(it + 1)], color); }
}
