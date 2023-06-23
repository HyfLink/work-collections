#ifndef HUSTMAP_H
#define HUSTMAP_H

#include "HustScene.h"
#include "LeastTransferRoutes.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class HustMap;
}
QT_END_NAMESPACE

class HustMap : public QMainWindow {
    Q_OBJECT

public:
    HustMap(QWidget *parent = nullptr);
    ~HustMap() override;

private slots:
    void onMousePressEvent(QGraphicsSceneMouseEvent *event);
    void onEditingFinished();

private:
    void flush();

private:
    Ui::HustMap        *ui;
    StopVec             m_stops;
    LineVec             m_lines;
    Organization        m_organization;
    HustScene          *m_scene;
    LeastTransferRoutes m_routes_planner;

    Coord                               src, dst;
    std::vector<StopRoute>              m_routes;
    std::vector<Organization::iterator> m_searches;
};
#endif // HUSTMAP_H
