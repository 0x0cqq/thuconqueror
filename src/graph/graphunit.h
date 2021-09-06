#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H

#include "../basic/status.h"
#include "unitdialog.h"
#include <QDebug>
#include <QGraphicsObject>
#include <QGraphicsScene>

#include <QGraphicsProxyWidget>
#include <QMovie>
#include <QTimer>

class GraphUnit : public QGraphicsObject {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
  public:
    QMovie *                m_Movie;
    const UnitStatus *      m_status;
    UnitDialog *            w;
    QGraphicsProxyWidget *  dialogWidget;
    QMetaObject::Connection mConnection;
    QTimer *                timer;
    GraphUnit(UnitStatus *status, const QPointF = QPoint(0, 0))
        : QGraphicsObject(), m_status(status), w(nullptr),
          dialogWidget(nullptr), timer(nullptr) {
        w     = new UnitDialog(status, nullptr);
        timer = new QTimer(this);
        this->setPos(getBlockCenter(nowCoord()));
        this->setFlag(ItemIsSelectable, true);
        this->setAcceptHoverEvents(true);
        qDebug() << "New unit " << m_status->m_uid << Qt::endl;
        setMovie(new QMovie(":/images/loop.gif"));
        connect(m_status, &UnitStatus::unitStateChanged, this,
                [=]() { this->update(this->boundingRect()); });
    }
    ~GraphUnit() { delete w; }
    QRectF       boundingRect() const override;
    void         setMovie(QMovie *movie);
    QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
               QWidget *) override;
    void paintAroundLoop(QPainter *painter);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

  signals:
  public slots:
};

#endif  // GRAPHUNIT_H