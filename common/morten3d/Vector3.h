/*
 * Vector3.h
 *
 *  Created on: 16. mars 2010
 *      Author: mbe036
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

/*
 * Vector3.h
 *
 *  Created on: 11 Sep 2009
 *      Author: mbe036
 */

#include "math.h"
#include <iostream>

class Vector3 {
private:
	double xvalue, yvalue, zvalue;

public:
	Vector3(double xv = 0, double yv = 0, double zv = 0) :
		xvalue(xv), yvalue(yv), zvalue(zv) {
	}

	double x() const {
		return xvalue;
	}

	double y() const {
		return yvalue;
	}

	double z() const {
		return zvalue;
	}

	Vector3 cross(const Vector3 & v) const {
		Vector3 r(y() * v.z() - z() * v.y(), z() * v.x() - x() * v.z(), x()
				* v.y() - y() * v.x());
		return r;
	}

	Vector3 operator+(const Vector3 & v) const {
		return Vector3(xvalue + v.xvalue, yvalue + v.yvalue, zvalue + v.zvalue);
	}


	void operator=(const Vector3 & v) {
			xvalue = v.x();
			yvalue = v.y();
			zvalue = v.z();
	}

	void operator+=(const Vector3 & v) {
				xvalue += v.x();
				yvalue += v.y();
				zvalue += v.z();
	}

	Vector3 operator-() const {
		return Vector3(-xvalue, -yvalue, -zvalue);
	}

	Vector3 operator*(double v) const {
		return Vector3(xvalue * v, yvalue * v, zvalue * v);
	}

	double operator*(const Vector3 & v) const {
		return xvalue * v.xvalue + yvalue * v.yvalue + zvalue * v.zvalue;
	}

	Vector3 operator-(const Vector3 & v) const {
		return Vector3(xvalue - v.xvalue, yvalue - v.yvalue, zvalue - v.zvalue);
	}
	//	Vector3 operator-(){
	//		return Vector3(-xvalue, -yvalue, -zvalue);
	//	}
	Vector3 operator/(double s) const {
		return Vector3(xvalue / s, yvalue / s, zvalue / s);
	}
	double lenght() const {
		return sqrt(xvalue * xvalue + yvalue * yvalue + zvalue * zvalue);
	}
	Vector3 normalize() const {
		double l = lenght();
		if (l > 0.001)
			return (*this) / l;
		else
			return (*this);
	}

	/**
	 * mutiplies this vector vith a 3x3 matrix in a 1D array, assumes matrix is 9 long,
	 * returns a new vector which is the result
	 */
	Vector3 multMatrix(double* m) const {
		return Vector3(//
				x() * m[0] + y() * m[1] + z() * m[2],//
				x() * m[3] + y() * m[4] + z() * m[5],//
				x() * m[6] + y() * m[7] + z() * m[8]//
		);
	}

	Vector3 rotate(const Vector3 & axis, double angle) const {
		double s = sin(angle);
		double c = cos(angle);
		Vector3 u = axis.normalize();

		double m[9] = {
		//
				u.x() * u.x() + (1 - u.x() * u.x()) * c, //
				u.x() * u.y() * (1 - c) - u.z() * s, //
				u.x() * u.z() * (1 - c) + u.y() * s, //
				u.x() * u.y() * (1 - c) + u.z() * s, //
				u.y() * u.y() + (1 - u.y() * u.y()) * c, //
				u.y() * u.z() * (1 - c) - u.x() * s, //
				u.x() * u.z() * (1 - c) - u.y() * s, //
				u.y() * u.z() * (1 - c) + u.x() * s, //
				u.z() * u.z() + (1 - u.z() * u.z()) * c //

				};

		return multMatrix(m);

	}
	friend Vector3 operator*(double r, Vector3& v);
	friend Vector3 operator*(float r, Vector3& v);
	friend std::ostream & operator<<(std::ostream& os, Vector3& v);
	friend std::istream & operator>>(std::istream& os, Vector3& v);
};
std::ostream & operator<<(std::ostream& os, Vector3& v);
std::istream & operator>>(std::istream& os, Vector3& v);
Vector3 operator*(double r, Vector3& v);
Vector3 operator*(float r, Vector3& v);

#endif /* VECTOR3_H_ */
