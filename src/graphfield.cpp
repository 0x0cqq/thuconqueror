#include "graphfield.h"
#include "graphblock.h"

GraphField::GraphField() : QGraphicsScene() {
    width = 24, height = 16;
    this->setSceneRect(
        QRectF(-1 * GraphInfo::blockSize, -qSqrt(3) / 2 * GraphInfo::blockSize,
               (1.5 * width + 0.5) * GraphInfo::blockSize,
               (qSqrt(3) * (height + 0.5)) * GraphInfo::blockSize));
    this->addRect(this->sceneRect());
    blocks.resize(width + 2);
    for(int i = 1; i <= width; i++) {
        blocks[i].resize(height + 2);
        for(int j = 1; j <= height; j++) {
            blocks[i][j] = new GraphBlock(QPoint{i, j}, getBlockCenter(i, j));
            this->addItem(blocks[i][j]);
        }
    }
    for(int i = 1; i <= width; i++) {
        for(int j = 1; j <= height; j++) {
            connect(blocks[i][j], &GraphBlock::checkChanged, this,
                    [this](QPoint coord, bool nowState) {
                        if(nowState == true) {
                            this->checkBlock(coord);
                        }
                    });
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

QPointF GraphField::getBlockCenter(QPoint coord) const {
    return getBlockCenter(coord.x(),coord.y());
}

void GraphField::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "scene: " << event->scenePos() << Qt::endl;
    QGraphicsScene::mousePressEvent(event);
}

void GraphField::moveUnit(qint32 uid, QVector<QPoint> path) {}

void GraphField::checkBlock(QPoint coord) {
    m_nowCheckedBlock = coord;
    for(int i = 1; i <= width; i++) {
        for(int j = 1; j <= height; j++) {
            if(coord.x() == i && coord.y() == j) {
                continue;
            }
            else {
                blocks[i][j]->changeCheck(false);
            }
        }
    }
}

void GraphField::unCheckBlock(QPoint coord) {}
