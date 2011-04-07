/*
 * Splines.cpp
 *
 *  Created on: 9. mai 2010
 *      Author: evi080
 */

#include "Splines.h"

Splines::Splines(int px, float tx): speed(0.01), displayList(0) {
	t = tx;
	p = px;
	// TODO Auto-generated constructor stub

}

Splines::~Splines() {
	// TODO Auto-generated destructor stub
}

void Splines::generatePath() {
	int oldP = p;
	float oldT = t;
	float oldSpeed = speed;
	p = 0; t = 0; speed = 0.1;

	Vector3 oldPos = position;
	Vector3 oldUp = up;
	Vector3 oldForward = forward;

	float colorBlue[4] = { 1.0, 0.0, 0.0, 1.0 };
	float colorPurple[4] = { 1.0, 0.0, 0.0, 1.0 };
	float colorBrown[4] = { 0.0, 0.0, 0.0, 1.0 };

	glDeleteLists(displayList, 1);
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);

	glPushAttrib(GL_LIGHTING_BIT);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorBrown);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorPurple);
	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLE_STRIP);


	alongLine();
	Vector3 prePos = position;
	Vector3 preUp = up;


	for (int i = 0; i < positions.size()/speed + 1; ++i) {
		alongLine();
		Vector3 forw = prePos -position;
		Vector3 right = forw.cross(preUp).normalize()*20;
		Vector3 posr = prePos + right;
		Vector3 posl = prePos - right;
		glNormal3f(preUp.x(),preUp.y(),preUp.z());
		glVertex3d(posr.x(), posr.y(), posr.z());
		glVertex3d(posl.x(), posl.y(), posl.z());

		prePos = position;
		preUp = up;
	}
	glEnd();
	glEnable(GL_CULL_FACE);
	glPopAttrib();


    glEndList();
	up = oldUp;
	forward = oldForward;
	position = oldPos;
	speed = oldSpeed;
	t = oldT;
	p = oldP;

}

void Splines::alongLine() {
	if (positions.size() <4) {
		return;
	}
	if (t>1 ) { p++; t-=1.0;}
	if (p >= positions.size()) p = 0;

	Vector3 p0 = positions[p];
	Vector3 p1 = positions[(p+1)%positions.size()];
	Vector3 p2 = positions[(p+2)%positions.size()];
	Vector3 p3 = positions[(p+3)%positions.size()];

	position = katmullRom(t, p0, p1, p2, p3);

	Vector3 f0 = forwards[p];
	Vector3 f1 = forwards[(p+1)%positions.size()];
	Vector3 f2 = forwards[(p+2)%positions.size()];
	Vector3 f3 = forwards[(p+3)%positions.size()];

	forward = katmullRom(t, f0, f1, f2, f3);

	Vector3 u0 = updirs[p];
	Vector3 u1 = updirs[(p+1)%positions.size()];
	Vector3 u2 = updirs[(p+2)%positions.size()];
	Vector3 u3 = updirs[(p+3)%positions.size()];

	up = katmullRom(t, u0, u1, u2, u3);

	t += speed;
}

Vector3 Splines::katmullRom(float t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3) {
	float t2 = t * t;
	float t3 = t2 * t;
	Vector3 pos = ( ( p1 * 2.0f) +
			( -p0 + p2 ) * t +
			( p0 * 2.0f - p1 * 5.0f + p2 * 4 - p3 ) * t2 +
			( -p0 + p1 * 3.0f - p2 * 3.0f + p3 ) * t3 ) * 0.5f;
	return pos;
}

std::ostream & operator<<(std::ostream& o, Splines & s) {
	int size = s.positions.size();
	o << size << std::endl;
	for (int i = 0; i< size;++i) {
		o << s.positions[i] << std::endl;
		o << s.forwards[i] << std::endl;
		o << s.updirs[i] << std::endl;
	}
	return o;
}

std::istream & operator>>(std::istream& o, Splines & s) {
	int num = 0;
	o >> num;
	for (int i = 0; i< num;++i) {
		Vector3 pos, forward, up;
		o >> pos;
		o >> forward;
		o >> up;
		s.positions.push_back(pos);
		s.forwards.push_back(forward);
		s.updirs.push_back(up);
	}
	return o;
}

