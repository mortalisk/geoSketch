#ifndef SPLINE2D_H
#define SPLINE2D_H

#include <QVector>
#include <QVector2D>
#include <algorithm>
#include "shape.h"
#include <QMap>
#include "util.h"
class node;

class Spline2d
{

    QVector<QVector2D> points;

public:
     int drawFrom, drawUntil;
    bool isSuggestion;
    QMap<float, QVector2D> pointsCache;
    void addPoint(QVector2D point) {
        points.append(point);
        isSuggestion = false;
        pointsCache.clear();
        resetDrawPoints();
    }

    void addPointFront(QVector2D point) {
        points.push_front(point);
        isSuggestion = false;
        pointsCache.clear();
        resetDrawPoints();
    }

    void resetDrawPoints() {
        drawFrom = 0;
        drawUntil = points.size();
    }

    void addAll(const QVector<QVector2D> & a) {
        points += a;
        resetDrawPoints();
    }

    void updateForBelowNode(Spline2d & newLayer) {
        if (!isLeftToRight()) reverse();
        if (!newLayer.isLeftToRight()) newLayer.reverse();
        QVector2D& splinep1 = points[0];
        QVector2D& splinep2 = points[points.size()-1];
        QVector2D& pfromnew = newLayer.points[0];
        QVector<QVector2D> newPoints;
        int intersectionIndex = 0;
        int intersectionIndexTop = 0;
        for (int i = 0; i< points.size()-1; i++) {
            for (int j = 0;j <newLayer.points.size()-1; j++) {
                QVector2D& a = points[i];
                QVector2D& b = points[i+1];
                QVector2D& c = newLayer.points[j];
                QVector2D& d = newLayer.points[j+1];

                double x,y;
                if (lineSegmentIntersection(a.x(),a.y(), b.x(), b.y(), c.x(), c.y(), d.x(), d.y(), &x, &y)) {
                    QVector2D intersect(x,y);

                    if (points[i].y() > newLayer.points[j].y()) {
                        for (int k = intersectionIndex; k <= i; k++) {
                            newPoints.push_back(points[k]);
                        }
                    }else {
                        for (int k = intersectionIndexTop; k <= j; k++) {
                            newPoints.push_back(newLayer.points[k]);
                        }
                    }
                    newPoints.push_back(intersect);


                    intersectionIndex = i+1;
                    intersectionIndexTop = j+1;
                }

            }
        }

        if(points[points.size()-1].y() > newLayer.points[newLayer.points.size()-1].y()) {
            for (int i = intersectionIndex; i< points.size(); i++) {
                newPoints.push_back(points[i]);
            }
        } else {
            for (int i = intersectionIndexTop; i< newLayer.points.size(); i++) {
                newPoints.push_back(newLayer.points[i]);
            }
        }

        points = newPoints;
    }

    const QVector<QVector2D>& getPoints() {
        return points;
    }

    void elevateTo(float y) {
        for(int i=0; i<points.size(); i++) {
            points[i] = QVector2D(points[i].x(), y);
        }
    }

    void setPoint(int i, QVector2D value) {
        points[i] = value;
        pointsCache.clear();
        resetDrawPoints();
    }

    void reverse() {
        std::reverse(points.begin(),points.end());
        pointsCache.clear();
    }

    void clear() {
        points.clear();
        pointsCache.clear();
        resetDrawPoints();
    }

    void smooth();
    QVector2D getPoint(float at);

    void changeLastPoint(QVector2D pos) {
        points[points.size()-1] = pos;
        pointsCache.clear();
    }

    QVector2D lastPoint() {
        if (points.size() > 1) {
        return points[points.size()-2];
        }else {
            QVector2D r(-10000,-10000);
            return r;
        }
    }

    int length() {
        return points.size();
    }

    bool isLeftToRight() {
        if (points.size() == 0) return false;
        return points[0].x() < points[points.size()-1].x();
    }

    QVector2D getLeftPoint() {
        if (isLeftToRight())
            return points[0];
        else if(points.size()>0)
            return points[points.size()-1];
        else {
            std::cout << "getLeftPoint called, but there are no points" << std::endl;
            return QVector2D();
        }
    }

    QVector2D getRightPoint() {
        if (isLeftToRight())
            return points[points.size()-1];
        else if (points.size() > 0)
            return points[0];
        else {
            std::cout << "getLeftPoint called, but there are no points" << std::endl;
            return QVector2D();
        }
    }

    int findNearestPoint(QVector2D first);

    QVariantMap toJson() {
        QVariantMap map;
        QVariantList pointsList = Vector2DListToVariantList(points);
        map["points"] = pointsList;
        return map;
    }

    void fromJson(QVariantMap map) {
        points.clear();
        points = variantToVector2DVector(map["points"]);
    }

    Spline2d();
};
#endif // SPLINE2D_H
