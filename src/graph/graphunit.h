#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H

#include "../basic/status.h"
#include "bloodbar.h"
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

    const UnitStatus *      m_status;
    UnitDialog *            m_unitDialog;
    BloodBar *              m_bloodBar;
    QGraphicsProxyWidget *  dialogWidget;
    QMetaObject::Connection mConnection1;  // 和 loop
    QMetaObject::Connection mConnection2;  // 和 l

    QTimer *timer;
    GraphUnit(UnitStatus *status, const QPointF = QPoint(0, 0))
        : QGraphicsObject(), m_status(status), m_unitDialog(nullptr),
          m_bloodBar(nullptr), dialogWidget(nullptr), timer(nullptr) {
        this->setZValue(GraphInfo::unitZValue);
        m_unitDialog = new UnitDialog(status, nullptr);
        timer        = new QTimer(this);
        m_bloodBar   = new BloodBar(-GraphInfo::blockSize,GraphInfo::blockSize,20,this);
        m_bloodBar->setPercentage(this->m_status->m_HPnow);
        // this->scene()
        this->setPos(getBlockCenter(nowCoord()));
        this->setFlag(ItemIsSelectable, true);
        this->setAcceptHoverEvents(true);
        qDebug() << "New unit " << m_status->m_uid << Qt::endl;
        setMovie(m_status->m_info->m_unitMovie,m_status->m_info->m_loopMovie);
        connect(m_status, &UnitStatus::unitStateChanged, this,
                [=]() { this->update(this->boundingRect()); });
    }
    ~GraphUnit() { delete m_unitDialog;delete timer; }
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