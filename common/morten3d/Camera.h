/*
 * Camera.h
 *
 *  Created on: 17. mars 2010
 *      Author: mbe036
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vector3.h"
#include <iostream>
#include <vector>
#include "Splines.h"

class Camera {
public:
	Camera();
	Camera(Vector3 pos, Vector3 forward, Vector3 up);
	virtual ~Camera();

	Vector3 position;
	Vector3 forward;
	Vector3 up;
	float fov;
	Splines * spline;

	void updateCamera();

	Vector3 looksAt() {
		return (position + forward * 100);
	}

	void goForward(double length) {
		position = (position + (forward * length));
	}

	void goUp(double length) {
		position = position + (up * length);
	}

	void goRight(double length) {
		Vector3 right = forward.cross(up);
		position = position + (right * length);
	}

	void turn(double angle) {
		forward = forward.rotate(Vector3(0,1,0), angle).normalize();
		up = up.rotate(Vector3(0,1,0), angle).normalize();
		fix();
	}

	void pitch(double angle) {
		Vector3 right = forward.cross(up);
		forward = forward.rotate(right, angle).normalize();
		up = up.rotate(right, angle);
		fix();
	}

	void yaw(double angle) {
		up = up.rotate(forward, angle);
		fix();
	}

	void fix() {
		double cos = (forward * up) / forward.lenght() * up.lenght();
		up = (up + (forward * -cos)).normalize();
		//double aftercos = forward * up / forward.lenght() * up.lenght();
		//			std::cout << "Camera changed forward:" << forward << " up:" << up
		//					<< std::endl << " cos:" << cos << " after:" << aftercos
		//					<< std::endl;
		//assert(dataInvariant());


	}

};

#endif /* CAMERA_H_ */
