#ifndef GENERALNODE_H
#define GENERALNODE_H
#include "basenode.h"

class GeneralNode : public BaseNode
{
public:
    GeneralNode(Shape* shape, QString name);
    GeneralNode(BaseNode & o) : BaseNode(o) {}

    BaseNode* copy() {
        return new GeneralNode(*this);
    }
};

#endif // GENERALNODE_H
