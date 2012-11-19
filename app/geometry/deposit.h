#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "basenode.h"
#include "surfacenode.h"
#include "isurfacefeature.h"

class Deposit : public BaseNode, public ISurfaceFeature {
    Q_OBJECT
private:
    float amount;
    float targetAmount;
    float seaLevel;
    QVector2D flowFrom;
    QVector2D direction;
    QVector<QVector<Vector3> > samples;
    QVector<QVector<float> > deposit1;
    QVector<QVector<float> > deposit2;
    QVector<QVector<int> > manhattan;
    bool alternateDeposits;
    bool depositing;
    Deposit * previousDeposit;
    bool hasReset;
public:

    bool isDone() {
        return amount > targetAmount;
    }

    SurfaceNode * surfaceNode;
    Deposit(QVector2D flowFrom, QVector2D direction, float seaLevel, SurfaceNode * surfaceNode, Deposit * previousDeposit) : BaseNode("deposit"), flowFrom(flowFrom), direction(direction), surfaceNode(surfaceNode), previousDeposit(previousDeposit) {
        this->seaLevel = seaLevel;
        diffuse = QVector4D(0.2, 0.3, 0.1, 1.0);
        amount = 0;
        hasReset = false;
    }

    Deposit(Deposit& deposit) : BaseNode(deposit), amount(0), targetAmount(deposit.targetAmount), seaLevel(deposit.seaLevel), flowFrom(deposit.flowFrom), direction(deposit.direction), depositing(deposit.depositing), previousDeposit(previousDeposit) {
        hasReset = false;
    }
    Deposit(){
        hasReset = false;
    }

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

        map["amount"] = targetAmount;
        map["seaLevel"] = seaLevel;

    }

    virtual void fromJsonSubclass(QVariantMap map) {
        flowFrom.setX(map["flowFrom"].toMap()["x"].toDouble());
        flowFrom.setY(map["flowFrom"].toMap()["y"].toDouble());
        direction.setX(map["direction"].toMap()["x"].toDouble());
        direction.setY(map["direction"].toMap()["y"].toDouble());
        targetAmount = map["amount"].toInt();
        amount = 0;
        seaLevel = map["seaLevel"].toDouble();
        depositing = false;
    }

    bool isDepositing() {
        return depositing;
    }

    void setDepositing(bool b) {
        depositing = b;
    }

    void prepareForDrawing();

    void resetSimulation();
};

#endif // DEPOSIT_H
