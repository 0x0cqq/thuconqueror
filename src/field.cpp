#include "field.h"

Field::Field(const QPoint &fieldSize) : fieldInfo(fieldSize) {
    // create blocks
    for(int i = 1; i <= width(); i++){
        QVector<Block *> tmp;
        for(int j = 1;j <= height();j++){
            tmp.append(new Block(QPoint{i,j},this));
        }
        blocks.append(tmp);
    }
}