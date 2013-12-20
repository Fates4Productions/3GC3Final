#include "Walls.h"

using namespace std;

walls::walls(point3D inblPoint, float inSize, float inHeight, bool inOrientation)
{
	this->blPosition = inblPoint;
	this->tlPosition = point3D(inblPoint.x, inblPoint.y, inblPoint.z + inHeight);

	if(inOrientation) //walls along y
	{
		this->brPosition = point3D(inblPoint.x + inSize, inblPoint.y, inblPoint.z);
		this->trPosition = point3D(inblPoint.x + inSize, inblPoint.y, inblPoint.z + inHeight);
	}
	else //walls along x
	{
		this->brPosition = point3D(inblPoint.x, inblPoint.y + inSize, inblPoint.z);
		this->trPosition = point3D(inblPoint.x, inblPoint.y + inSize, inblPoint.z + inHeight);
	}

	this->fSize = inSize;
	this->fHeight = inHeight;
	this->bOrientation = inOrientation;

}

void walls::setPosition(float x, float y, float z)
{

	this->blPosition.x = x;
	this->blPosition.y = y;
	this->blPosition.z = z;

}

void walls::setSize(float newSize)
{

	this->fSize = newSize;

}
void walls::setHeight(float newHeight)
{

	this->fHeight = newHeight;

}

void walls::setOrientation(bool newOrientation)
{

	this->bOrientation = newOrientation;

}

//Getters
point3D walls::getBL()
{
	return this->blPosition;
}
point3D walls::getTL()
{
	return this->tlPosition;
}point3D walls::getBR()
{
	return this->brPosition;
}point3D walls::getTR()
{
	return this->trPosition;
}

bool walls::getOrientation()
{

	return this->bOrientation;

}