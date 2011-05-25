#include "testarrays.h"
#include "geometry/box.h"
#include "geometry/arrays.h"

TestArrays::TestArrays(QObject *parent) :
        QObject(parent)
{
}

void TestArrays::makeArrayShouldMakeArrayOfCorrectSize() {
//    Box box(5,5,5);
//    vertex* array;
//    int size = 0;
//    array = Arrays::makeArray(box, size);
//    QVERIFY2(array != NULL, "array should not be null");
//    QVERIFY2(size == 36, "size should be 36 for a Box");
//    for (int i = 0; i < size; ++i) {
//        QVERIFY2(array[i].x <=5 && array[i].x >=-5 ,"values in array should not be bigger than 5 or less than -5");
//        QVERIFY2(array[i].y <=5 && array[i].y >=-5 ,"values in array should not be bigger than 5 or less than -5");
//        QVERIFY2(array[i].z <=5 && array[i].z >=-5 ,"values in array should not be bigger than 5 or less than -5");
//    }
}
