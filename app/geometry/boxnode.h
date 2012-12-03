#ifndef BOXNODE_H
#define BOXNODE_H

#include "basenode.h"
#include "surface.h"
#include "sidenode.h"
#include "surfacenode.h"

class BoxNode : public BaseNode
{
    Q_OBJECT
public:
    BoxNode();
    BoxNode(BoxNode& o);

    void init();

    ~BoxNode() {
        foreach(SideNode* p, surfaces) {
            delete p;
        }
    }

    void setUpSurfaces();

    void makeWaterNode();

    float width, depth, heigth;
    SideNode * frontNode,  * backNode,  * leftNode,  * rightNode,  * topNode,  * bottomNode;
    float topF, bottomF ,rightF ,leftF ,farF , nearF;
    QVector<SideNode*> surfaces;
    SideNode * activeSurface;
    SurfaceNode * bottomDummyNode;
    SurfaceNode * currentBelowNode;
    SurfaceNode * waterNode;
    Scene * scene;
    float getWidth();
    float getHeight();
    float getDepth();
    void addPoint(Vector3 from, Vector3 direction);
    void determineActionOnStoppedDrawing();
    void draw();
    void drawSelf();
    void makeSuggestionFor(SideNode* side);
    bool includeShapeInExport() {return false;}

    void clearSketch() {
        frontNode->clearSketch();

        backNode->clearSketch();

        leftNode->clearSketch();

        rightNode->clearSketch();
    }

    void updateCurrentBelowNode();

    void setSeaLevel(float y);

    float getSeaLevel();

    QVector<Vector3> intersectionPoints(Vector3 from, Vector3 direction);
    float intersectionPoint(Vector3 from, Vector3 direction);

    BoxNode * copy() {
        BoxNode * node = new BoxNode(*this);


        return node;
    }

    void childDeleted(BaseNode *child);

    QString getTypeId() {
        return QString("BoxNode");
    }

    void addSubclassJson(QVariantMap& map);

    void fromJsonSubclass(QVariantMap map);

    void addSubclassActions(QToolBar *);

public slots:
    void makeLayer();
signals:
    void madeNode(BaseNode *);
};

#endif // BOXNODE_H
