#include "arrays.h"
#include <QVector>
#include <QVector3D>
#include "shape.h"

/**
  * Takes a Shape and makes a new float array on the heap. This array
  * can be used as a VBO in openGl. Array Must be deleted later.
  * @param shape the Shape
  * @param array will be set to point to the new array
  * @param size the size of the new array
  */
vertex* Arrays::makeArray(Shape &shape, int& size) {
    QVector<vertex>& vertices = shape.getVertices();
    size = vertices.size();
    return &vertices[0];
}
