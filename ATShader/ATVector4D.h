#ifndef _ATVECTOR4D_H_
#define _ATVECTOR4D_H_
#include "ATColor.h"

class ATVector4D
{
private:
	float x, y, z, w;
	ATColor color;

public:
	ATVector4D();
	ATVector4D(float _x, float _y, float _z);
	ATVector4D(float _x, float _y, float _z, float _w);
	ATVector4D(const ATVector4D &copy);

	float getX();
	float getY();
	float getZ();
	float getW();

	void addVector(ATVector4D atv);
	void scaleVector(float scalar);
	void scaleVectorNotZ(float scalar);
	void multiplyVector(ATVector4D atv);
	static ATVector4D multiplyTwoVectors(ATVector4D atv1, ATVector4D atv2);
	static ATVector4D addTwoVectors(ATVector4D atv1, ATVector4D atv2);
	static ATVector4D subtractTwoVectors(ATVector4D atv1, ATVector4D atv2);
	static ATVector4D crossProduct(ATVector4D atv1, ATVector4D atv2);
	static ATVector4D normalize(ATVector4D atv);
	static ATVector4D mkATVector4D(float _x, float _y, float _z);
	static ATVector4D mkATVector4D(float _x, float _y, float _z, float _w);
	static float dot(ATVector4D atv1, ATVector4D atv2);

	void setColor(float a, float r, float g, float b);
	void setColor(ATColor col);
	ATColor getColor();
};

#endif