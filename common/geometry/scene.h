#ifndef SCENE_H
#define SCENE_H
#include <QVector>
#include "geometry/shape.h"
#include "geometry/box.h"
#include "morten3d/Camera.h"
#include "arrays.h"

class Scene
{
private:
    QVector<Shape*> shapes;

public:
    Camera camera;
    Scene() {
        add(new Box(5,5,5));
    }

    void add(Shape* shape) {
        shapes.append(shape);
        shape->array = Arrays::makeArray(*shape,shape->arraySize);
    }

    QVector<Shape*>& getShapes(){
        return shapes;
    }

    ~Scene() {
        foreach(Shape* shape , shapes) {
            delete shape;
        }
    }

};

#endif // SCENE_H
