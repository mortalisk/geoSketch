/*
 * Vector3.cpp
 *
 *  Created on: 11 Sep 2009
 *      Author: mbe036
 */

#include "Vector3.h"
#include <string>
#include <iostream>

std::ostream & operator<<(std::ostream& os, Vector3& v) {
	os << "Vector3 x: " << v.x() << " y: " << v.y() << " z: " << v.z();
	return os;
}
std::istream & operator>>(std::istream& os, Vector3& v) {
	std::string dummy;
	os >> dummy >> dummy >> v.xvalue >> dummy >> v.yvalue >> dummy >> v.zvalue;
	return os;
}

Vector3 operator*(double r, Vector3& v) {
	return Vector3(v.xvalue*r, v.yvalue*r, v.zvalue * r);
}

Vector3 operator*(float r,Vector3& v) {
	return Vector3(v.xvalue*r, v.yvalue*r, v.zvalue * r);
}
