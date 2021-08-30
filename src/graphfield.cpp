#include "graphfield.h"
#include "graphblock.h"

GraphField::GraphField() : QGraphicsScene() {
    width = 24, height = 16;
    this->setSceneRect(QRectF(-1 * GraphInfo::blockSize, -qSqrt(3)/2 * GraphInfo::blockSize,
                              (1.5 * width + 0.5) * GraphInfo::blockSize,
                              (qSqrt(3) * (height + 0.5)) * GraphInfo::blockSize));
    for(int i = 1; i <= width; i++) {
        for(int j = 1; j <= height; j++) {
            this->addItem(new GraphBlock(getBlockCenter(i,j)));
        }
    }
}

QPointF GraphField::getBlockCenter(qint32 r, qint32 c) const {
    Q_ASSERT(1 <= r && r <= width);
    Q_ASSERT(1 <= c && c <= height);
    return QPointF(1.5 * (r - 1),
                   qSqrt(3) * (c - 1) + (r % 2 == 0 ? qSqrt(3) / 2 : 0)) *
        GraphInfo::blockSize;
}

void GraphField::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "event: " << event->pos() << Qt::endl;
    qDebug() << "scene: " << event->scenePos() << Qt::endl;
    QGraphicsScene::mousePressEvent(event);
}