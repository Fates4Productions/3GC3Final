#ifndef _H_Math3DLib_
#define _H_Math3DLib_

class point3D{
public:
	point3D();
	point3D(float inX, float inY, float inZ);
	float x;
	float y;
	float z;
};

class vec3D{
public:
	vec3D();
	vec3D(float inX, float inY, float inZ);
	float x;
	float y;
	float z;
	float magnitude();
};

class colour{
public:
	colour();
	colour(float inR, float inG, float inB);
	float r;
	float g;
	float b;
};


static class Math3DLib{
public:
	Math3DLib();
	static float distance(point3D start, point3D end);
	static float fastDistance(point3D start, point3D end);
	static float length(vec3D v);
	static vec3D normalize(vec3D v);
	static vec3D vectorMultiply(vec3D v, float scale);
	static vec3D createVector(point3D start, point3D end);
	static point3D movePoint(point3D p, vec3D v);
};


#endif