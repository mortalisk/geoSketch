#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "basenode.h"
#include "surfacenode.h"
#include "isurfacefeature.h"

class Deposit : public BaseNode, public ISurfaceFeature {
private:
    int times;
    QVector2D flowFrom;
    QVector2D direction;
    float volume;
    SurfaceNode * surfaceNode;
    QVector<QVector<Vector3> > samples;
    QVector<QVector<float> > deposit1;
    QVector<QVector<float> > deposit2;
    bool alternateDeposits;
    bool depositing;
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

    void exchange(QVector<QVector<float> > * previousDeposit, QVector<QVector<float> > * deposited, int xinc, int yinc);

    virtual void repositionOnSurface(SurfaceNode &surfacenode);

    virtual void doTransformSurface(QVector<QVector<Vector3> > &rows, float resolution, float size);

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

    bool isDepositing() {
        return depositing;
    }

    void setDepositing(bool b) {
        depositing = b;
    }

    void prepareForDrawing();
};

#endif // DEPOSIT_H
