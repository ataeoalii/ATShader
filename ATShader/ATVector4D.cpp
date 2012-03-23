#include "ATVector4D.h"
#include <math.h>


ATVector4D::ATVector4D()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

ATVector4D::ATVector4D(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
	w = 1.0f;
}

ATVector4D::ATVector4D(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

ATVector4D::ATVector4D(const ATVector4D &copy)
{
	this->x = copy.x;
	this->y = copy.y;
	this->z = copy.z;
	this->w = copy.w;
	this->color = copy.color;
}

float ATVector4D::getX()
{
	return x;
}

float ATVector4D::getY()
{
	return y;
}

float ATVector4D::getZ()
{
	return z;
}

float ATVector4D::getW()
{
	return w;
}

void ATVector4D::addVector(ATVector4D atv)
{
	x+=atv.x;
	y+=atv.y;
	z+=atv.z;
}

void ATVector4D::scaleVectorNotZ(float scalar)
{
	x*=scalar;
	y*=scalar;
}

void ATVector4D::scaleVector(float scalar)
{
	x*=scalar;
	y*=scalar;
	z*=scalar;
}


ATVector4D ATVector4D::addTwoVectors(ATVector4D atv1, ATVector4D atv2)
{
	ATVector4D vec(atv1.x + atv2.x, atv1.y + atv2.y, atv1.z + atv2.z);
	return vec;
}

ATVector4D ATVector4D::mkATVector4D(float _x, float _y, float _z)
{
	ATVector4D vec(_x,_y, _z);
	return vec;
}

ATVector4D ATVector4D::mkATVector4D(float _x, float _y, float _z, float _w)
{
	ATVector4D vec(_x,_y, _z, _w);
	return vec;
}


 ATVector4D ATVector4D::subtractTwoVectors(ATVector4D atv1, ATVector4D atv2)
 {
	 float x = atv1.x - atv2.x;
	 float y = atv1.y - atv2.y;
	 float z = atv1.z - atv2.z;

	 ATVector4D atv(x, y, z);
	 return atv;
 }

ATVector4D ATVector4D::crossProduct(ATVector4D atv1, ATVector4D atv2)
{
	float x = atv1.getY()*atv2.getZ() - atv1.getZ()*atv2.getY();
	float y = atv1.getZ()*atv2.getX() - atv1.getX()*atv2.getZ();
	float z = atv1.getX()*atv2.getY() - atv1.getY()*atv2.getX();
	ATVector4D atv(x, y, z);

	return atv;
}

ATVector4D ATVector4D::normalize(ATVector4D atv)
{
	float magnitude = sqrtf((atv.getX()*atv.getX()) + (atv.getY()*atv.getY()) + (atv.getZ()*atv.getZ()));
	ATVector4D atvA(atv.getX()/magnitude, atv.getY()/magnitude, atv.getZ()/magnitude);
	return atvA;
}

void ATVector4D::setColor(float a, float r, float g, float b)
{
	color.setColor(a,r,g,b);
}

void ATVector4D::setColor(ATColor col)
{
	color.setColor(col.getalphaF(), col.getredF(), col.getgreenF(), col.getblueF());
}

ATColor ATVector4D::getColor()
{
	return color;
}

float ATVector4D::dot(ATVector4D atv1, ATVector4D atv2)
{
	float fl = atv1.getX() * atv2.getX() + atv1.getY() * atv2.getY() + atv1.getZ() * atv2.getZ() ;
	return fl;
}