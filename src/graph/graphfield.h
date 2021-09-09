#ifndef GRAPHFIELD_H
#define GRAPHFIELD_H

#include "../basic/status.h"
#include "graphblock.h"
#include "graphunit.h"

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

#define blocks(point) m_blocks[(point).x()][(point).y()]

class GraphField : public QGraphicsScene {
    Q_OBJECT
  public:
    const GameInfo &               m_gameInfo;
    QVector<QVector<GraphBlock *>> m_blocks;
    QVector<GraphUnit *>           m_units;
    GraphBlock *                   m_nowCheckedBlock;

    GraphField(const GameInfo &                 gameInfo,
               QVector<QVector<BlockStatus *>> &blockStatus,
               QVector<UnitStatus *> &          unitStatus);
    ~GraphField();
    qint32 width() const { return m_gameInfo.map_size.x(); }
    qint32 height() const { return m_gameInfo.map_size.y(); }
    void   showUnitAttackLabel(qint32 uid, qreal delta);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    // 辅助函数先放在这里

  signals:
    void checkStateChange(QPoint coord, bool state);
    void moveRangeChange(qint32 uid, QPoint coord, bool state);
    // 暂时未用
    void userNewUnit(QPoint coord);
    void userMoveUnit(qint32 uid, QPoint destCoord);
    void userAttackUnit(qint32 uid, QPoint destCoord);
    void userShowMoveRange(qint32 uid);
    void userHideMoveRange();
    void needUpdateDetail();
    void finishOrder();
    // 把 uid 的 unit 移动到 (posx, posy)
  protected slots:
    void moveUnit(GraphUnit *graphUnit, const QVector<QPoint> &path);
    void attackUnit(GraphUnit *graphUnit, qint32 tarid,
                    QPair<qreal, qreal> delta);
  public slots:
    void newUnit(UnitStatus *unitStatus);
    void dieUnit(qint32 uid);
    void moveUnit(qint32 uid, const QVector<QPoint> &path);
    void attackUnit(qint32 uid, qint32 tarid, QPair<qreal, qreal> delta);
    void attackCamp(qint32 uid, QPoint coord, QPair<qreal, qreal> delta);
    void showMoveRange(qint32 uid, QVector<QPoint> range);
    void hideMoveRange();
    void onBlockClicked(QPoint coord);
};

#endif