#pragma once

#include "HustMapTypes.h"
#include <QGraphicsScene>

class HustScene : public QGraphicsScene {
    Q_OBJECT
public:
    HustScene(const StopVec &stops, const LineVec &lines, QObject *parent = nullptr);
    ~HustScene() override = default;

    void clear();
    void drawPostion(Coord pos, const QColor & = QColor(255, 64, 64, 128));
    void drawLine(Coord x, Coord y, const QColor & = QColor(255, 64, 64, 128));
    void drawRoute(const std::vector<Coord> &routes, const QColor & = QColor(255, 64, 64, 128));
    void drawRoute(const std::vector<Index> &routes, const QColor & = QColor(255, 64, 64, 128));

signals:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void initialize_hustmap();

private:
    QPixmap        m_hustmap;
    const StopVec &m_stops;
    const LineVec &m_lines;
};
