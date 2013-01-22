#include "boxnode.h"

#include <iostream>
#include <algorithm>
#include "GL/glew.h"
#include "surface.h"
#include "float.h"
#include "surfacenode.h"
#include "scene.h"

BoxNode::BoxNode() :BaseNode("boxnode")
{

    init();
    setUpSurfaces();
    makeWaterNode();
}

void BoxNode::init() {
    activeSurface = NULL;
    width = 10;
    depth = 10;
    heigth = 10;
    topF = heigth/2;
    bottomF = -topF;
    rightF = width/2;
    leftF = -rightF;
    farF = -depth/2;
    nearF = -farF;

    /*           H ___________ G
                 /|          /|
                / |         / |
               /  |        /  |
              /  E|______ /___|F
             /___/______ /   /
           D|   /       |C  /
            |  /        |  /(depth)
    (height)| /         | /
            |/__________|/
           A    (width)  B
       (0,0,0)

      */
    Vector3 A (leftF,bottomF,nearF);
    Vector3 B (rightF,bottomF,nearF);
    Vector3 C (rightF,topF,nearF);
    Vector3 D (leftF,topF,nearF);

    Vector3 E (leftF,bottomF,farF);
    Vector3 F (rightF,bottomF,farF);
    Vector3 G (rightF,topF,farF);
    Vector3 H (leftF,topF,farF);

    frontNode = new SideNode(A, B, C, D);
    backNode = new SideNode(F, E, H, G );
    topNode = new SideNode(D, C, G, H);
    bottomNode = new SideNode(E, F, B, A);
    leftNode = new SideNode(E, A, D, H);
    rightNode = new SideNode(B, F, G, C);

}

BoxNode::BoxNode(BoxNode& o): BaseNode(o) {
    frontNode = new SideNode(*o.frontNode);
    backNode = new SideNode(*o.backNode);
    leftNode = new SideNode(*o.leftNode);
    rightNode = new SideNode(*o.rightNode);
    topNode = new SideNode(*o.topNode);
    bottomNode = new SideNode(*o.bottomNode);

    setUpSurfaces();
    makeWaterNode();

    QVector<BaseNode*> tmp = children;
    children.clear();
    foreach(BaseNode* c , tmp) {
        children.push_back(c);
        updateCurrentBelowNode();
    }
    waterNode->below = currentBelowNode;
    waterNode->elevateTo(o.getSeaLevel());
    waterNode->visible = o.waterNode->visible;

    activeSurface = NULL;
}

void BoxNode::setUpSurfaces() {
    frontNode->setOpposite(backNode);
    leftNode->setOpposite(rightNode);
    topNode->setOpposite(bottomNode);

    frontNode->setLeft(leftNode);
    leftNode->setLeft(backNode);
    backNode->setLeft(rightNode);
    rightNode->setLeft(frontNode);

    surfaces.push_back(frontNode);
    surfaces.push_back(backNode);
    surfaces.push_back(leftNode);
    surfaces.push_back(rightNode);
    surfaces.push_back(topNode);
    surfaces.push_back(bottomNode);

}

void BoxNode::makeWaterNode() {
    Spline front;
    Spline right;
    Spline back;
    Spline left;
    front.addPoint(frontNode->lowerLeft);
    front.addPoint(frontNode->lowerRigth);
    right.addPoint(rightNode->lowerLeft);
    right.addPoint(rightNode->lowerRigth);
    back.addPoint(backNode->lowerLeft);
    back.addPoint(backNode->lowerRigth);
    left.addPoint(leftNode->lowerLeft);
    left.addPoint(leftNode->lowerRigth);

    bottomDummyNode = new SurfaceNode("BottomDummy", front, right, back, left, NULL);
    currentBelowNode = new SurfaceNode(*bottomDummyNode);

    waterNode = new SurfaceNode(*bottomDummyNode);
    waterNode->elevateTo(5);
    waterNode->below = bottomDummyNode;
    waterNode->setActive(false);
    waterNode->visible = false;
    waterNode->diffuse = QVector4D(0.3,0.3,1.0,0.5);

}

float BoxNode::getDepth() {
    return depth;
}

float BoxNode::getWidth() {
    return width;
}

float BoxNode::getHeight() {
    return heigth;
}

float BoxNode::intersectionPoint(Vector3 from, Vector3 direction) {
    float dist = FLT_MAX;
    float ss, tt;
    foreach (BaseNode * s, surfaces) {
        QVector<Vector3> points = s->intersectionPoints(from, direction,ss,tt);
        if (points.size() >0) {
            Vector3& p = points[0];
            float d = (p-from).lenght();
            if (d < dist) dist = d;
        }
    }
    return dist;
}

QVector<Vector3> BoxNode::intersectionPoints(Vector3 from, Vector3 direction) {
    QVector<Vector3> p;
    float d = intersectionPoint(from, direction);
    if (d < FLT_MAX) {
        p.append(from + (direction.normalize()*d));
    }
    return p;
}

void BoxNode::setSeaLevel(float y) {
    waterNode->elevateTo(y);
    waterNode->visible = true;

}

float BoxNode::getSeaLevel() {
    return waterNode->front.getLeftPoint().y();
}

void BoxNode::addPoint(Vector3 from, Vector3 direction) {

    // we must find the nearest intersection point
    float candidateDistance = FLT_MAX;
    Vector3 candidatePoint;
    QVector2D uvCandidate;
    SideNode * candidate = NULL;
    float ss, tt;
    foreach (SideNode * s, surfaces) {
        QVector<Vector3> points = s->intersectionPoints(from, direction,ss,tt);
        if (points.size() >0 ) {
                float dist = (points[0]-from).lenght();
                if ((activeSurface == NULL || s == activeSurface) && dist < candidateDistance) {
                    candidateDistance = dist;
                    candidatePoint = points[0];
                    uvCandidate = QVector2D(ss, tt);
                    candidate=s;
               }

        }
    }

    if (candidate) {
        candidate->sketchingSpline.addPoint(candidatePoint);
        candidate->uvSketchingSpline.addPoint(uvCandidate);


        activeSurface = candidate;
    }
}

void BoxNode::determineActionOnStoppedDrawing() {
        if (activeSurface == topNode||activeSurface == bottomNode){
            activeSurface->spline.clear();
            activeSurface->sketchingSpline.clear();
            activeSurface->uvSpline.clear();
            activeSurface->uvSketchingSpline.clear();
            activeSurface = NULL;
            return;
        }
	if (activeSurface) {
            activeSurface->correctSketchingDirection();

            if (activeSurface->uvSpline.getPoints().size() == 0) {
                activeSurface->moveSketchingPointsToSpline();
            } else {
                activeSurface->doOversketch();
            }

            activeSurface->makeSuggestionLines();

            makeSuggestionFor(activeSurface);

            activeSurface = NULL;
	}

    frontNode->redrawPoints();

    leftNode->redrawPoints();

    rightNode->redrawPoints();

    backNode->redrawPoints();

}

void BoxNode::makeSuggestionFor(SideNode* side) {


    if (side->opposite->uvSpline.isSuggestion && side->left->uvSpline.isSuggestion && side->right->uvSpline.isSuggestion) {
        side->opposite->spline.clear();
        side->opposite->uvSpline.clear();
        // project points from this side to opposite
        Vector3 direction = side->opposite->lowerRigth - side->lowerLeft;
        side->opposite->projectPoints(direction, side->uvSpline.getPoints());

        side->opposite->uvSpline.isSuggestion = true;
    }

    QVector2D left = side->uvSpline.getLeftPoint();
    QVector2D right = side->uvSpline.getRightPoint();
    QVector2D leftOpposite = side->opposite->uvSpline.getLeftPoint();
    QVector2D rightOpposite = side->opposite->uvSpline.getRightPoint();

    side->left->addInterpolatedSuggestion(rightOpposite.y(), left.y());
    side->right->addInterpolatedSuggestion(right.y(), leftOpposite.y());

    frontNode->ensureLeftToRigth();
    rightNode->ensureLeftToRigth();
    backNode->ensureLeftToRigth();
    leftNode->ensureLeftToRigth();

    side->uvSpline.isSuggestion = false;

}

void BoxNode::childDeleted(BaseNode *child) {
    scene->nodeDeleted(child);
        QVector<BaseNode*> tmp = children;
        children.clear();
        delete currentBelowNode;
        currentBelowNode = new SurfaceNode(*bottomDummyNode);

        foreach(BaseNode* c, tmp) {
            children.push_back(c);
            updateCurrentBelowNode();
            waterNode->below = currentBelowNode;
            waterNode->invalidate();
        }
}

void BoxNode::makeLayer() {

    if (frontNode->uvSpline.getPoints().size() < 1||rightNode->uvSpline.getPoints().size() <1
            ||backNode->uvSpline.getPoints().size() <1||leftNode->uvSpline.getPoints().size() <1)
        return;

    frontNode->redrawPoints();
    backNode->redrawPoints();
    leftNode->redrawPoints();
    rightNode->redrawPoints();

    SurfaceNode * below = currentBelowNode;

    SurfaceNode * n = new SurfaceNode( "Layer", frontNode->spline, rightNode->spline, backNode->spline, leftNode->spline, below);
    children.append(n);
    n->parent = this;

    foreach(SideNode* s, surfaces) {
        s->uvSpline.clear();
        s->uvSketchingSpline.clear();
        s->spline.clear();
        s->sketchingSpline.clear();
        s->spline.isSuggestion = true;
    }

    updateCurrentBelowNode();

    waterNode->below = currentBelowNode;
    waterNode->invalidate();

    emit madeNode( n);
}

void BoxNode::updateCurrentBelowNode() {
    if (children.size() == 0) {
        currentBelowNode = new SurfaceNode(*bottomDummyNode);
    }else {
        SurfaceNode * top = dynamic_cast<SurfaceNode*>(children[children.size()-1]);
        currentBelowNode = new SurfaceNode(*currentBelowNode);
        currentBelowNode->front.updateForBelowNode(top->front);
        currentBelowNode->back.updateForBelowNode(top->back);
        currentBelowNode->left.updateForBelowNode(top->left);
        currentBelowNode->right.updateForBelowNode(top->right);
    }
}

void BoxNode::draw() {

    glPushMatrix();


    drawChildren();
    if (waterNode->visible) {
       //glEnable(GL_CULL_FACE);
        waterNode->draw();

        //glDisable(GL_CULL_FACE);
    }


    //glColor4f(0,0,0,1);
    drawSelf();
    //glDisable(GL_CULL_FACE);

    glPopMatrix();
}

void BoxNode::drawSelf() {

    if (!visible) return;
    //Node::drawSelf();
    glTranslatef(position.x(), position.y(), position.z());

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    foreach(BaseNode * s, surfaces) {
        s->drawSplines();
        s->shape->drawLines(false, activeSurface==s?1:0, 0,0,activeSurface==s?1:0.3);
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    foreach(BaseNode * s, surfaces) {
        glColor4f(0.5,0.5,0.5,0.5);
        s->shape->drawShape(ambient, diffuse);
    }

    glDisable(GL_CULL_FACE);


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}


void BoxNode::addSubclassActions(QToolBar * menu) {
    QList<QAction*> opts;

    if (frontNode->spline.getPoints().size() > 2) {
        QAction * actionMakeLayer = new QAction("Make Layer", menu);
        QObject::connect(actionMakeLayer,SIGNAL(triggered()), this, SLOT(makeLayer()));
        opts.append(actionMakeLayer);
    }

    menu->addActions(opts);
}

void BoxNode::addSubclassJson(QVariantMap &map) {
    map["width"] = width;
    map["depth"] = depth;
    map["heigth"] = heigth;
    map["waterLevel"] = getSeaLevel();
    map["waterVisible"] = waterNode->visible;
}

void BoxNode::fromJsonSubclass(QVariantMap map) {
    surfaces.clear();

    width = map["width"].toFloat();
    depth = map["depth"].toFloat();
    heigth = map["heigth"].toFloat();

    init();
    setUpSurfaces();
    makeWaterNode();

    waterNode->visible = map["waterVisible"].toBool();
    waterNode->elevateTo(map["waterLevel"].toDouble());

    QVector<BaseNode*> tmp = children;
    children.clear();
    foreach(BaseNode* c, tmp) {
        ((SurfaceNode*)c)->below = currentBelowNode;
        children.push_back(c);
        updateCurrentBelowNode();
    }

    waterNode->below = currentBelowNode;
    waterNode->invalidate();


}
