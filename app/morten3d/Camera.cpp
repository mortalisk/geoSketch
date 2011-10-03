/*
 * Camera.cpp
 *
 *  Created on: 17. mars 2010
 *      Author: mbe036
 */

#include "Camera.h"
#include "Vector3.h"

Camera::Camera() :
        position(10, 10, 10), forward(-1, -1, -1), up(-1, 1, -1), fov(M_PI_4) {
	forward = forward.normalize();
	up = up.normalize();
	fix();
        trackMode = FIRST_PERSON;
}

Camera::Camera(Vector3 pos, Vector3 forw, Vector3 ups) :
	position(pos), forward(forw), up(ups), fov(M_PI_2) {
	fix();
}

Camera::~Camera() {
	// nothing to do
}

void Camera::updateCamera() {
}
