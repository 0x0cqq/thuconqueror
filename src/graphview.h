#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include "graphfield.h"
#include <QGraphicsView>

class GraphView : public QGraphicsView {
    Q_OBJECT
    void resizeEvent(QResizeEvent *event) override {
        if(this->scene() != nullptr) {
            this->fitInView(this->scene()->sceneRect(), Qt::KeepAspectRatio);
        }
        this->QGraphicsView::resizeEvent(event);
    }
};

#endif