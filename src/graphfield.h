#ifndef GRAPHFIELD_H
#define GRAPHFIELD_H

#include "graphblock.h"
#include "graphunit.h"
#include "info.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>

// GraphField 类就相当于一个前端。
// 目的是
// 1. 接受用户的输入，发出对应的 Signal （给 Field 用）
// 2. 设置 Slot 函数，接受 Field 类对状态的更新。

// 问题是，要让前端的显示知道多少后端的内容（游戏规则之类的）？肯定是越少越好。
// 但太少也不行，因为前端还得进行“操作”，不可能一点后端都不知道。

// 所以 Signals 的抽象就是：“用户做了什么事”
// 所以 Slots 的抽象就是：“棋盘要怎么办？”
// 但是 其实 Unit 也可以 Catch 一些 Signal ，比如显示

#define blocks(point) blocks[(point).x()][(point).y()]

class GraphField : public QGraphicsScene {
    Q_OBJECT
  public:
    qint32                         width, height;
    QVector<GraphUnit *>           units;
    QPoint                         m_nowCheckedBlock;
    QVector<QVector<GraphBlock *>> blocks;
    GraphField();
    QPointF getBlockCenter(qint32 r, qint32 c) const;
    QPointF getBlockCenter(QPoint coord) const;
    void    mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void    setUnit(GraphUnit *unit) {
        blocks(unit->m_nowCoord)->m_unitOnBlock = unit;
    }
  signals:
    // 把 uid 的 unit 移动到 (posx, posy)
    void unitTryToMove(qint32 uid, QPoint targetPos);
  public slots:
    // 在当前选中的 box 里面新建一个 unit
    void newUnit() {
        // 需要当前位置没有Unit，否则会炸掉的
        if(blocks(m_nowCheckedBlock)->m_unitOnBlock != nullptr) {
            QMessageBox msgBox;
            msgBox.setText("当前界面已经有Unit了!");
            int ret = msgBox.exec();
            return;
        }

        GraphUnit *newUnit = new GraphUnit(units.size(), m_nowCheckedBlock,
                                           getBlockCenter(m_nowCheckedBlock));
        units.append(newUnit);
        this->addItem(newUnit);
        setUnit(newUnit);
    }
    void moveUnit(qint32 uid, QVector<QPoint> path);
    void checkBlock(QPoint coord);
    void unCheckBlock(QPoint coord);
};

#endif