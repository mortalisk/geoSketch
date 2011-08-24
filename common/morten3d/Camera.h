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
        enum TrackMode {FIRST_PERSON, SPHERE_TRACK, BOX_TRACK};
        TrackMode trackMode;
        Vector3 trackCenter;
        Vector3 trackUp;
        float trackSize;


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
                reTrack();
	}

	void goUp(double length) {
		position = position + (up * length);
                reTrack();
	}

	void goRight(double length) {
		Vector3 right = forward.cross(up);
		position = position + (right * length);
                reTrack();
	}

	void turn(double angle) {
		forward = forward.rotate(Vector3(0,1,0), angle).normalize();
		up = up.rotate(Vector3(0,1,0), angle).normalize();
		fix();
                reTrack();
	}

	void pitch(double angle) {
		Vector3 right = forward.cross(up);
		forward = forward.rotate(right, angle).normalize();
		up = up.rotate(right, angle);
		fix();
                reTrack();
	}

	void yaw(double angle) {
		up = up.rotate(forward, angle);
		fix();
                reTrack();
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

        void reTrack() {
            if (trackMode == SPHERE_TRACK) {
                forward = (trackCenter - position).normalize();
                float dist = (trackCenter - position).lenght();
                position += forward*(dist-trackSize);
            }
            //std::cout << trackCenter << std::endl;
        }

};

#endif /* CAMERA_H_ */
