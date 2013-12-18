#include "Walls.h"

static std::list<walls> wallsList;

walls::walls()
{

}

walls::walls(point3D inblPoint, float inSize, float inHeight, colour inColour, string inMateral, bool inOrientation)
{
	this->blPosition = inblPoint;
	this->tlPosition = point3D(inblPoint.x, inblPoint.y, inblPoint.z + inHeight);

	if(inOrientation) //walls along x
	{
		this->brPosition = point3D(inblPoint.x + inSize, inblPoint.y, inblPoint.z);
		this->brPosition = point3D(inblPoint.x + inSize, inblPoint.y, inblPoint.z + inHeight);
	}
	else //walls along y
	{
		this->brPosition = point3D(inblPoint.x, inblPoint.y + inSize, inblPoint.z);
		this->brPosition = point3D(inblPoint.x, inblPoint.y + inSize, inblPoint.z + inHeight);
	}

	this->fSize = inSize;
	this->fHeight = inHeight;
	this->cColour = inColour;
	this->sMaterial = inMateral;
	this->bOrientation = inOrientation;

}

void walls::setPosition(float x, float y, float z)
{

	this->blPosition.x = x;
	this->blPosition.y = y;
	this->blPosition.z = z;

}
void walls::setColour(float r, float g, float b)
{

	this->cColour.r = r;
	this->cColour.g = g;
	this->cColour.b = b;

}
void walls::setSize(float newSize)
{

	this->fSize = newSize;

}
void walls::setHeight(float newHeight)
{

	this->fHeight = newHeight;

}
void walls::setMaterial(string newMaterial)
{

	this->sMaterial = newMaterial;

}
void walls::setOrientation(bool newOrientation)
{

	this->bOrientation = newOrientation;

}

//Getters
point3D walls::getPosition()
{

	return this->blPosition;

}
colour walls::getColour()
{

	return this->cColour;

}
float walls::getSize()
{

	return this->fSize;

}
float walls::getHeight()
{

	return this->fHeight;
}
string walls::getMaterial()
{

	return this->sMaterial;

}
bool walls::getOrientation()
{

	return this->bOrientation;

}