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
    if(m_Movie) {
        // qDebug() << m_Movie->frameRect();
        auto tmp = m_Movie->frameRect();
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

void GraphUnit::setMovie(QMovie *movie) {
    prepareGeometryChange();
    QObject::disconnect(mConnection);  // disconnect old object
    m_Movie = movie;
    if(m_Movie) {
        qDebug() << "indeed scale size"
                 << QRandomGenerator::global()->generate() << Qt::endl;
        mConnection =
            QObject::connect(m_Movie, &QMovie::frameChanged, [=] { update(); });
        m_Movie->setScaledSize(
            QSize(4 * GraphInfo::unitSize, 4 * GraphInfo::unitSize));
        m_Movie->start();
    }
}

void GraphUnit::paintAroundLoop(QPainter *painter) {
    if(m_Movie != nullptr) {
        // qDebug() << "indeed paint around loop"
        //          << QRandomGenerator::global()->generate() << Qt::endl;
        painter->drawPixmap(-m_Movie->frameRect().bottomRight() / 2,
                            m_Movie->currentPixmap(), m_Movie->frameRect());
    }
}

void GraphUnit::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                      QWidget *) {
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
    if(m_status->canMove() || m_status->canAttack()) {
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
