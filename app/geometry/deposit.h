#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "basenode.h"
#include "surfacenode.h"
#include "isurfacefeature.h"

class Deposit : public BaseNode, public ISurfaceFeature {
    Q_OBJECT
private:
    int times;
    int targetTimes;
    QVector2D flowFrom;
    QVector2D direction;
    QVector<QVector<Vector3> > samples;
    QVector<QVector<float> > deposit1;
    QVector<QVector<float> > deposit2;
    QVector<QVector<int> > manhattan;
    bool alternateDeposits;
    bool depositing;
public:

    SurfaceNode * surfaceNode;
    Deposit(QVector2D flowFrom, QVector2D direction, SurfaceNode * surfaceNode) : BaseNode("deposit"), flowFrom(flowFrom), direction(direction), surfaceNode(surfaceNode) {
        diffuse = QVector4D(0.2, 0.3, 0.1, 1.0);
        times = 0;
    }

    Deposit(Deposit& deposit) : BaseNode(deposit), flowFrom(deposit.flowFrom), direction(deposit.direction), targetTimes(deposit.targetTimes), depositing(deposit.depositing), times(0) {

    }
    Deposit(){}

    virtual Deposit* copy() {
        return new Deposit(*this);
    }

    virtual QString getTypeId() {
        return QString("Deposit");
    }

    void exchange(QVector<QVector<float> > * previousDeposit, QVector<QVector<float> > * deposited, int xinc, int yinc, int fromx, int fromy);

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

        map["times"] = targetTimes;

    }

    virtual void fromJsonSubclass(QVariantMap map) {
        flowFrom.setX(map["flowFrom"].toMap()["x"].toDouble());
        flowFrom.setY(map["flowFrom"].toMap()["y"].toDouble());
        direction.setX(map["direction"].toMap()["x"].toDouble());
        direction.setY(map["direction"].toMap()["y"].toDouble());
        targetTimes = map["times"].toInt();
        times = 0;
        depositing = false;
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
