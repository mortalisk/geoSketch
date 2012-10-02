#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "basenode.h"
#include "surfacenode.h"

class Deposit : public BaseNode {
private:
    QVector2D flowFrom;
    QVector2D direction;
    float volume;
    SurfaceNode * surfaceNode;
public:
    Deposit(QVector2D flowFrom, QVector2D direction, float volume, SurfaceNode * surfaceNode) : BaseNode("deposit"), flowFrom(flowFrom), direction(direction), volume(volume), surfaceNode(surfaceNode) {

    }

    Deposit(Deposit& deposit) : BaseNode(deposit), flowFrom(flowFrom), direction(direction), volume(volume) {

    }
    Deposit(){}

    virtual Deposit* copy() {
        return new Deposit(*this);
    }

    virtual QString getTypeId() {
        return QString("Deposit");
    }

    virtual void addSubclassJson(QVariantMap &map) {
        QVariantMap flowFromMap;
        flowFromMap["x"] = flowFrom.x();
        flowFromMap["y"] = flowFrom.y();
        map["flowFrom"] = flowFromMap;

        QVariantMap directionMap;
        flowFromMap["x"] = direction.x();
        flowFromMap["y"] = direction.y();
        map["direction"] = directionMap;

        map["volume"] = volume;

    }

    virtual void fromJsonSubclass(QVariantMap map) {
        flowFrom.setX(map["flowFrom"].toMap()["x"].toDouble());
        flowFrom.setY(map["flowFrom"].toMap()["y"].toDouble());
        direction.setX(map["direction"].toMap()["x"].toDouble());
        direction.setY(map["direction"].toMap()["y"].toDouble());
        volume = map["volume"].toDouble();
    }

    void prepareForDrawing();
};

#endif // DEPOSIT_H
