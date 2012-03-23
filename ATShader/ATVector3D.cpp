#include "ATVector3D.h"
#include <math.h>


ATVector3D::ATVector3D()
{
	x = 0;
	y = 0;
	z = 1;
}

ATVector3D::ATVector3D(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

ATVector3D::ATVector3D(const ATVector3D &copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
}


float ATVector3D::getX()
{
	return x;
}

float ATVector3D::getY()
{
	return y;
}

float ATVector3D::getZ()
{
	return z;
}



void ATVector3D::addVector(ATVector3D atv)
{
	x+=atv.x;
	y+=atv.y;
}

void ATVector3D::scaleVector(float scalar)
{
	x*=scalar;
	y*=scalar;
	z*=scalar;
}


ATVector3D ATVector3D::addTwoVectors(ATVector3D atv1, ATVector3D atv2)
{
	ATVector3D vec(atv1.x + atv2.x, atv1.y + atv2.y, atv1.z + atv2.z);
	return vec;
}

ATVector3D ATVector3D::mkATVector3D(float _x, float _y, float _z)
{
	ATVector3D vec(_x,_y, _z);
	return vec;
}

ATVector3D ATVector3D::mkATVector3D(float _x, float _y)
{
	ATVector3D vec(_x,_y, 1.0f);
	return vec;
}



 ATVector3D ATVector3D::subtractTwoVectors(ATVector3D atv1, ATVector3D atv2)
 {
	 float x = atv1.x - atv2.x;
	 float y = atv1.y - atv2.y;
	 float z = atv1.z - atv2.z;

	 ATVector3D atv(x, y, z);
	 return atv;
 }

ATVector3D ATVector3D::crossProduct(ATVector3D atv1, ATVector3D atv2)
{
	float x = atv1.getY()*atv2.getZ() - atv1.getZ()*atv2.getY();
	float y = atv1.getZ()*atv2.getX() - atv1.getX()*atv2.getZ();
	float z = atv1.getX()*atv2.getY() - atv1.getY()*atv2.getX();
	ATVector3D atv(x, y, z);

	return atv;
}

ATVector3D ATVector3D::normalize(ATVector3D atv)
{
	float magnitude = sqrtf((atv.getX()*atv.getX()) + (atv.getY()*atv.getY()) + (atv.getZ()*atv.getZ()));
	ATVector3D atvA(atv.getX()/magnitude, atv.getY()/magnitude, atv.getZ()/magnitude);
	return atvA;
}
