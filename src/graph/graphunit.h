#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H

#include "../basic/info.h"
#include "../basic/status.h"
#include "unitdialog.h"
#include <QDebug>
#include <QGraphicsObject>
#include <QGraphicsScene>

#include <QGraphicsProxyWidget>
#include <QTimer>

class GraphUnit : public QGraphicsObject {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
  public:
    const UnitStatus *    m_status;
    UnitDialog *          w;
    QGraphicsProxyWidget *dialogWidget;

    QTimer *timer;
    GraphUnit(UnitStatus *status, const QPointF &pos)
        : QGraphicsObject(), m_status(status), w(nullptr),dialogWidget(nullptr),timer(nullptr) {
        w     = new UnitDialog(status, nullptr);
        timer = new QTimer(this);
        this->setPos(pos);
        this->setFlag(ItemIsSelectable, true);
        this->setAcceptHoverEvents(true);
        qDebug() << "New unit " << m_status->m_uid << Qt::endl;
    }
    ~GraphUnit() { delete w; }
    QRectF       boundingRect() const override;
    QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
               QWidget *) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

  signals:
  public slots:
};

#endif  // GRAPHUNIT_H