#ifndef POLICYTREE_H
#define POLICYTREE_H

#include <QObject>


// 预留的政策树
class PolicyTree : public QObject {
    Q_OBJECT
  public:
    PolicyTree();
};

#endif  // POLICYTREE_H