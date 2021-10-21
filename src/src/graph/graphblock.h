#ifndef GRAPHBLOCK_H
#define GRAPHBLOCK_H

#include "../basic/status.h"
#include "bloodbar.h"
#include <QDebug>
#include <QGraphicsObject>
#include <QMovie>
#include <QPainter>
#include <QPixmap>

// 地图上的 格子
class GraphBlock : public QGraphicsObject {
    Q_OBJECT
  public:
    QMovie *                m_fire_movie;
    BloodBar *              m_blockCampBlood;
    const BlockStatus *     m_status;
    bool                    m_isChecked;
    bool                    m_isMoveRange;
    QMetaObject::Connection mConnection;
    QPainterPath            path;
    // GraphBlock() : QGraphicsObject() {}
    GraphBlock(BlockStatus *status, QPointF pos, QGraphicsItem *parent = nullptr)
        : QGraphicsObject(parent), m_fire_movie(nullptr), m_blockCampBlood(nullptr),
          m_status(status), m_isChecked(false), m_isMoveRange(false) {
        this->setZValue(GraphInfo::blockZValue);

        this->setPos(pos);
        // qDebug() << "pos" << m_status->m_coord << "image" <<
        // m_status->m_info->image;
        connect(this, &GraphBlock::checkChanged, this,
                [&]() { this->update(this->boundingRect()); });
        connect(this, &GraphBlock::moveRangeChanged, this,
                [&]() { this->update(this->boundingRect()); });
        // add blood bar? and movie?
        if(this->m_status->m_type & campBlock) {
            m_blockCampBlood = new BloodBar(GraphInfo::blockSize / 2,
                                            GraphInfo::blockSize, 10, this);
            m_blockCampBlood->setPercentage(this->m_status->m_HPnow);
            m_fire_movie = new QMovie(":/images/fire.gif");
            m_fire_movie->start();
            setMovie();
        }
        // according to
        // https://stackoverflow.com/questions/43826317/how-to-optimize-qgraphicsviews-performance
        path.addPolygon(GraphInfo::blockPoly);
    }
    ~GraphBlock() {
        m_fire_movie->deleteLater();
        m_blockCampBlood->deleteLater();
    }
    // QPoint coord() const {return m_status->m_coord;}
    // qint32 unitOnBlock() { return m_status->m_unitOnBlock;}
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void         setMovie();
    void         paintFire(QPainter *painter);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  signals:
    void blockClicked(QPoint coord);
    void checkChanged(QPoint coord, bool nowState);
    void moveRangeChanged(QPoint coord, bool nowState);
  public slots:
    void changeCheck(QPoint coord, bool isChecked);
    void changeMoveRange(qint32 uid, QPoint coord, bool is_moveRange);
};

#endif  // FIELDBLOCK_H