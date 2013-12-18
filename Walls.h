#ifndef _WALLS_H_
#define _WALLS_H_

#include <Windows.h>
#include "Math3DLib.h"
#include "Particle.h"
#include <stdlib.h>
#include <stdio.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <list>
#include <string>

using namespace std;

class walls
{

private:

	point3D tlPosition;
	point3D trPosition;
	point3D brPosition;
	point3D blPosition;
	float fSize;
	float fHeight;
	colour cColour;
	string sMaterial;
	bool bOrientation;

public:
	static std::list<walls> wallsList;

	walls::walls();
	walls::walls(point3D inblPoint, float inSize, float inHeight, colour inColour, string inMateral, bool inOrientation);

	//Set Functions
	void setPosition(float x, float y, float z);
	void setColour(float r, float g, float b);
	void setSize(float newSize);
	void setHeight(float newHeight);
	void setMaterial(string newMaterial);
	void setOrientation(bool newOrientation);

	//Getters
	point3D getPosition();
	colour getColour();
	float getSize();
	float getHeight();
	string getMaterial();
	bool getOrientation();
}

#endif