#include "HustMap.h"
#include "./ui_HustMap.h"
#include "matches.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

HustMap::HustMap(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::HustMap), m_stops(StopVec::from_qfile(":/assets/assets/stops.txt")),
      m_lines(LineVec::from_qfile(":/assets/assets/lines.txt")),
      m_organization(Organization::from_qfile(":/assets/assets/organization.txt")),
      m_scene(new HustScene(m_stops, m_lines, this)), m_routes_planner(m_stops, m_lines) {
    // initialize objects
    ui->setupUi(this);
    ui->gfxview->setScene(m_scene);
    onEditingFinished();

    // connect signals and slots
    connect(ui->searchEdit, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()));
    connect(m_scene, SIGNAL(mousePressEvent(QGraphicsSceneMouseEvent *)), this,
            SLOT(onMousePressEvent(QGraphicsSceneMouseEvent *)));
}

HustMap::~HustMap() { delete ui; }

void HustMap::onMousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        src = event->scenePos();
        ui->srcPos->setText(QString::number(src.x()) + ", " + QString::number(src.y()));
        m_routes = m_routes_planner.route(src, dst);
        flush();
    } else if (event->button() == Qt::RightButton) {
        dst = event->scenePos();
        ui->dstPos->setText(QString::number(dst.x()) + ", " + QString::number(dst.y()));
        m_routes = m_routes_planner.route(src, dst);
        flush();
    }
}

void HustMap::onEditingFinished() {
    const auto text = ui->searchEdit->text().toStdString();

    m_searches.clear();
    for (auto it = m_organization.begin(); it != m_organization.end(); ++it) {
        if (matches(it->first.begin(), it->first.end(), text.begin(), text.end())) { m_searches.push_back(it); }
    }

    flush();
}

void HustMap::flush() {
    m_scene->clear();
    m_scene->drawPostion(src, QColor(0, 0, 255));
    m_scene->drawPostion(dst, QColor(255, 0, 0));

    // draw routes
    if (QPointF::dotProduct(src, src) < 1 || QPointF::dotProduct(dst, dst) < 1) {
        // do nothing
    } else if (m_routes.empty()) {
        ui->numValue->setText(QString::number(1));
        ui->transValue->setText(QString::number(0));
        m_scene->drawLine(src, dst);
    } else {
        ui->numValue->setText(QString::number(m_routes.size()));
        ui->transValue->setText(QString::number(m_routes[0].transfers));
        m_scene->drawLine(src, m_stops[m_routes[0].front()]);
        m_scene->drawLine(dst, m_stops[m_routes[0].back()]);
        for (const auto &r : m_routes) { m_scene->drawRoute(r, r.color); }
    }

    // search results
    QString text;

    for (auto it : m_searches) {
        text.append(it->first.c_str());
        text.append('\n');

        m_scene->drawPostion(it->second, QColor(255, 32, 32));
        m_scene->addSimpleText(it->first.c_str())->setPos(it->second);
    }

    ui->searchResult->setPlainText(text);
}
