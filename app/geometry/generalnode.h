#ifndef GENERALNODE_H
#define GENERALNODE_H
#include "basenode.h"

class GeneralNode : public BaseNode
{
    Q_OBJECT
public:
    GeneralNode() {}
    GeneralNode(Shape* shape, QString name);
    GeneralNode(BaseNode & o) : BaseNode(o) {}

    BaseNode* copy() {
        return new GeneralNode(*this);
    }

    QString getTypeId() {
        return QString("GeneralNode");
    }

    void addSubclassJson(QVariantMap &) {};
    void fromJsonSubclass(QVariantMap ) {};
};

#endif // GENERALNODE_H
