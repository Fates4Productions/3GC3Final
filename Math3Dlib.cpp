#include "Math3DLib.h"
#include <stdlib.h>
#include <math.h>

point3D::point3D(){
	x = 0;
	y = 0;
	z = 0;
}

point3D::point3D(float inX, float inY, float inZ){
	x = inX;
	y = inY;
	z = inZ;
}

vec3D::vec3D(){
	x = rand()%10-rand()%10;
	y = rand()%10-rand()%10;
	z = rand()%20+15;
}

vec3D::vec3D(float inX, float inY, float inZ){
	x = inX;
	y = inY;
	z = inZ;
}

float vec3D::magnitude(){
	return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}

colour::colour(){
	r = rand()%101/100.0;
	g = rand()%101/100.0;
	b = rand()%101/100.0;
}

colour::colour(float inR, float inG, float inB){
	r = inR;
	g = inG;
	b = inB;
}

Math3DLib::Math3DLib(){
}

float Math3DLib::distance(point3D start, point3D end){
	return sqrt(pow(end.x-start.x,2)+pow(end.y-start.y,2)+pow(end.z-start.z,2));
}

float Math3DLib::fastDistance(point3D start, point3D end){
	return pow(end.x-start.x,2)+pow(end.y-start.y,2)+pow(end.z-start.z,2);
}

float Math3DLib::length(vec3D v){
	return v.magnitude();
}

vec3D Math3DLib::normalize(vec3D v){
	return vec3D(v.x/v.magnitude(),v.y/v.magnitude(),v.z/v.magnitude());
}

vec3D Math3DLib::vectorMultiply(vec3D v, float scale){
	return vec3D(scale*v.x,scale*v.y,scale*v.z);
}

vec3D Math3DLib::createVector(point3D start, point3D end){
	return vec3D(end.x-start.x,end.y-start.y,end.z-start.z);
}

point3D Math3DLib::movePoint(point3D p, vec3D v){
	return point3D(p.x+v.x, p.y+v.y, p.z+v.z);
}

