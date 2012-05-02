/*
 * Camera.h
 *
 *  Created on: 17. mars 2010
 *      Author: mbe036
 */

#ifndef CAMERA_H_
#define CAMERA_H_
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector3.h"
#include <iostream>
#include <vector>

#define PI2 (M_PI * 2)

class Camera {
public:
    Camera();
    Camera(Vector3 pos, Vector3 forward, Vector3 up);
    virtual ~Camera();
    enum TrackMode {FIRST_PERSON, SPHERE_TRACK, BOX_TRACK};
    TrackMode trackMode;
    Vector3 trackCenter;
    Vector3 trackUp;
    float trackDistance;
    float trackAngleXZ;
    float trackAngleXY;
    Vector3 position;
    Vector3 forward;
    Vector3 up;
    float fov;

    void updateCamera();

    void setTrackMode(TrackMode mode, Vector3 lookAt, Vector3 position) {
        trackMode = mode;
        trackCenter = lookAt;
        this->position = position;
        this->forward = lookAt - position;
        this->up = Vector3(0,1,0);
        Vector3 a = lookAt - position;
        trackAngleXZ = atan(a.z()/a.x());
        if (a.z() < 0.0) trackAngleXZ += M_PI;
        trackAngleXY = atan(a.y()/a.x());
        if (a.y() < 0.0) trackAngleXZ += M_PI;
        trackDistance = a.lenght();
        fix();
    }

    Vector3 looksAt() {
        return (position + forward * 100);
    }

    void goForward(double length) {
        if (trackMode == SPHERE_TRACK) {
            trackDistance += length;
        } else {
            position = (position + (forward * length));
        }
        fix();
    }

    void goUp(double length) {
        if (trackMode == SPHERE_TRACK) {
            trackAngleXY += length;
        }else {
            position = position + (up * length);
        }
        fix();
    }

    void goRight(double length) {
        if (trackMode == SPHERE_TRACK) {
            trackAngleXZ += length;
        } else {
            Vector3 right = forward.cross(up);
            position = position + (right * length);
        }
        fix();

    }

    void turn(double angle) {
        if (trackMode == SPHERE_TRACK) {
            trackAngleXZ += angle;
        } else {
            forward = forward.rotate(Vector3(0,1,0), angle).normalize();
            up = up.rotate(Vector3(0,1,0), angle).normalize();
        }
        fix();

    }

    void pitch(double angle) {
        if (trackMode == SPHERE_TRACK) {
            trackAngleXY += angle;
        } else {
            Vector3 right = forward.cross(up);
            forward = forward.rotate(right, angle).normalize();
            up = up.rotate(right, angle);
        }
        fix();

    }

    void yaw(double angle) {
        if (trackMode == SPHERE_TRACK) {

        } else {
            up = up.rotate(forward, angle);
        }
        fix();

    }

    void fix() {

        if (trackAngleXZ > PI2) {
            trackAngleXZ -= PI2;
        }
        if (trackAngleXZ < PI2) {
            trackAngleXZ += PI2;
        }
        if (trackAngleXY > -0.1) {
            trackAngleXY = -0.1;
        }
        if (trackAngleXY < -M_PI) {
            trackAngleXY = -M_PI;
        }

        if (trackMode == SPHERE_TRACK) {
            Vector3 posAtSphere(sin(trackAngleXZ)*fabs(sin(trackAngleXY)), cos(trackAngleXY), cos(trackAngleXZ)*fabs(sin(trackAngleXY)));
            position = trackCenter + posAtSphere*trackDistance;
            forward = (trackCenter-position).normalize();
            up = Vector3(0,1 , 0);
        }

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
