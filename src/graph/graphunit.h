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
    QMovie *                m_loopMovie;
    QMovie *                m_unitMovie;
    const UnitStatus *      m_status;
    UnitDialog *            w;
    QGraphicsProxyWidget *  dialogWidget;
    QMetaObject::Connection mConnection1; // 和 loop
    QMetaObject::Connection mConnection2; // 和 l

    QTimer *                timer;
    GraphUnit(UnitStatus *status, const QPointF = QPoint(0, 0))
        : QGraphicsObject(),m_status(status), w(nullptr),
          dialogWidget(nullptr), timer(nullptr){
        w     = new UnitDialog(status, nullptr);
        timer = new QTimer(this);
        this->setPos(getBlockCenter(nowCoord()));
        this->setFlag(ItemIsSelectable, true);
        this->setAcceptHoverEvents(true);
        qDebug() << "New unit " << m_status->m_uid << Qt::endl;
        setMovie(new QMovie(m_status->m_info->image),
                 new QMovie(":/images/loop.gif"));
        connect(m_status, &UnitStatus::unitStateChanged, this,
                [=]() { this->update(this->boundingRect()); });
    }
    ~GraphUnit() { delete w; }
    QRectF       boundingRect() const override;
    void         setMovie(QMovie *unitMovie, QMovie *loopMovie);
    QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
               QWidget *) override;
    void paintUnit(QPainter *painter);
    void paintAroundLoop(QPainter *painter);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

  signals:
  public slots:
};

#endif  // GRAPHUNIT_H