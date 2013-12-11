#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <Windows.h>
#include "Math3DLib.h"
#include <stdlib.h>
#include <stdio.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <list>
#include <string>

using namespace std;

class particle
{

private:
	point3D pPosition;
	colour cColour;
	int iSize;
	point3D pScale;
	vec3D vRotation;
	string sShape;
	string sMaterial;

public:
	static std::list<particle> particleList;

	particle::particle();
	particle::particle(float xp, float yp, float zp);
	particle::particle(point3D inPosition, colour inColour, int inSize, point3D pScale, vec3D inRotation, string inShape, string inMaterial);

	//Set Functions
	void setPosition(float x, float y, float z);
	void setColour(float r, float g, float b);
	void setSize(int newSize);
	void setScale(float x, float y, float z);
	void setRotation(vec3D newRot);
	void setShape(string newShape);
	void setMaterial(string newMaterial);

	//Getters
	point3D getPosition();
	colour getColour();
	int getSize();
	point3D getScale();
	vec3D getRotation();
	string getShape();
	string getMaterial();

};

#endif