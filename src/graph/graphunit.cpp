#include "graphunit.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QLabel>
#include <QLayout>
#include <QPainter>
#include <QRandomGenerator>

QRectF GraphUnit::boundingRect() const {
    if(m_loopMovie) {
        // qDebug() << m_loopMovie->frameRect();
        auto tmp = m_loopMovie->frameRect();
        tmp.moveCenter(QPoint(0, 0));
        return tmp;
    }
    else
        return QRectF(-GraphInfo::unitSize - GraphInfo::penWidth,
                      -GraphInfo::unitSize - GraphInfo::penWidth,
                      2 * (GraphInfo::unitSize + GraphInfo::penWidth),
                      2 * (GraphInfo::unitSize + GraphInfo::penWidth));
}

QPainterPath GraphUnit::shape() const {
    QPainterPath path;
    path.addEllipse({0, 0}, GraphInfo::unitSize, GraphInfo::unitSize);
    return path;
}

// about movie:
// https://forum.qt.io/topic/123784/animated-gif-in-qgraphicsscene-qgraphicsview/5

void GraphUnit::setMovie(QMovie *unitMovie, QMovie *loopMovie) {
    prepareGeometryChange();
    QObject::disconnect(mConnection1);  // disconnect old object

    m_loopMovie = loopMovie;
    if(m_loopMovie) {
        // qDebug() << "indeed scale size"
        //          << QRandomGenerator::global()->generate() << Qt::endl;
        mConnection1 = QObject::connect(m_loopMovie, &QMovie::frameChanged,
                                        [=] { update(); });
        m_loopMovie->setScaledSize(
            QSize(4 * GraphInfo::unitSize, 4 * GraphInfo::unitSize));
        m_loopMovie->start();
    }
    QObject::disconnect(mConnection2);  // disconnect old object
    m_unitMovie = unitMovie;
    if(m_unitMovie) {
        // qDebug() << "indeed scale size"
        //          << QRandomGenerator::global()->generate() << Qt::endl;
        mConnection2 = QObject::connect(m_unitMovie, &QMovie::frameChanged,
                                        [=] { update(); });
        m_unitMovie->setScaledSize(
            QSize(3 * GraphInfo::unitSize, 3 * GraphInfo::unitSize));
        m_unitMovie->start();
    }
}

void GraphUnit::paintAroundLoop(QPainter *painter) {
    if(m_loopMovie != nullptr) {
        // qDebug() << "indeed paint around loop"
        //          << QRandomGenerator::global()->generate() << Qt::endl;
        painter->drawPixmap(-m_loopMovie->frameRect().bottomRight() / 2,
                            m_loopMovie->currentPixmap(),
                            m_loopMovie->frameRect());
    }
}

void GraphUnit::paintUnit(QPainter *painter) {
    if(m_unitMovie != nullptr) {
        painter->drawPixmap(-m_unitMovie->frameRect().bottomRight() / 2,
                            m_unitMovie->currentPixmap(),
                            m_unitMovie->frameRect());
    }
}

void GraphUnit::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                      QWidget *) {
    if(m_status->m_info->image == "") {
        // 没有图的权宜之策
        if(m_status->isAlive()) {
            if(m_status->m_player == 1) {
                painter->setBrush(Qt::blue);
            }
            else {
                painter->setBrush(Qt::yellow);
            }
        }
        else {
            painter->setBrush(Qt::black);
        }
        painter->setPen(QPen(Qt::black, GraphInfo::penWidth));
        painter->drawEllipse({0, 0}, GraphInfo::unitSize, GraphInfo::unitSize);
        painter->setFont(QFont("Microsoft YaHei", 30, 2));
        painter->drawText(QPoint(0, 0), QString::number(m_status->m_type));
    }
    else {
        paintUnit(painter);
    }
    if((m_status->canMove() || m_status->canAttack()) && m_status->isAlive()) {
        paintAroundLoop(painter);
    }
    // painter->drawRoundedRect(-100, -100, 200, 200, 50, 50);
    // painter->fillRect(0, 0, 100, 100, Qt::green);
}

void GraphUnit::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "press unit: " << this->m_status->m_uid << Qt::endl;
    QGraphicsObject::mousePressEvent(event);
    event->ignore();
}

void GraphUnit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "press unit: " << this->m_status->m_uid << Qt::endl;
    QGraphicsObject::mouseReleaseEvent(event);
    event->ignore();
}

void GraphUnit::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    qDebug() << "hover!" << nowCoord();
    auto v = scene()->views().first();
    if(dialogWidget == nullptr) {
        dialogWidget = scene()->addWidget(w);
        dialogWidget->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        dialogWidget->setZValue(100);
        dialogWidget->hide();
    }
    timer->singleShot(500, [=]() {
        dialogWidget->setPos(this->mapToScene(100, 100));
        w->updateInfo();
        w->show();
        // w->move(v->mapToGlobal(v->mapFromScene(this->mapToScene(100, 100))));
        // w->show();
    });
    QGraphicsObject::hoverEnterEvent(event);
}

void GraphUnit::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    qDebug() << "hover leave" << nowCoord();
    // dialogWidget->hide();
    w->hide();
    QGraphicsObject::hoverLeaveEvent(event);
}
