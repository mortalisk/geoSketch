/*
 * Splines.h
 *
 *  Created on: 9. mai 2010
 *      Author: evi080
 */

#ifndef SPLINES_H_
#define SPLINES_H_

#include <GL/gl.h>
#include "Vector3.h"
#include <vector>
#include <iostream>

class Splines {
public:
	Splines(int px, float tx);
	virtual ~Splines();
	void alongLine();
	std::vector< Vector3 > positions;
	std::vector< Vector3 > forwards;
	std::vector< Vector3 > updirs;
	std::vector< Vector3 > pathPos;
	float t;
        unsigned int p;
	float speed;
	Vector3 up;
	Vector3 forward;
	Vector3 position;
	GLuint displayList;
	Vector3 katmullRom(float t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3);
	void generatePath();
	friend std::ostream & operator<<(std::ostream& o, Splines & s);
	friend std::istream & operator>>(std::istream& o, Splines & s);
};

std::ostream & operator<<(std::ostream& o, Splines & s);
std::istream & operator>>(std::istream& o, Splines & s);

#endif /* SPLINES_H_ */
