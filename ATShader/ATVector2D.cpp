#include "ATVector2D.h"
#include <math.h>

/**
* Default constructor.
*/
ATVector2D::ATVector2D()
{
	x = 0;
	y = 0;
}

/**
* Constructor that sets x and y.
*/
ATVector2D::ATVector2D(float _x, float _y)
{
	x = _x;
	y = _y;
}


/**
* Returns x.
*/
float ATVector2D::getX()
{
	return x;
}

/**
* Returns y.
*/
float ATVector2D::getY()
{
	return y;
}

/**
* Adds a vector to this vector.
*/
void ATVector2D::addVector(ATVector2D atv)
{
	x+=atv.x;
	y+=atv.y;
}

/**
* Scales a vector by a scalar.
*/
void ATVector2D::scaleVector(float scalar)
{
	x*=scalar;
	y*=scalar;
}

/**
* Adds two vectors together to form another vector.
*/
ATVector2D ATVector2D::addTwoVectors(ATVector2D atv1, ATVector2D atv2)
{
	ATVector2D vec(atv1.x + atv2.x, atv1.y + atv2.y);
	return vec;
}

/**
* Creates a 2D vector with x and y.
*/
ATVector2D ATVector2D::mkATVector2D(float _x, float _y)
{
	ATVector2D vec(_x,_y);
	return vec;
}

/**
* Subtracts one vector from another.
*/
 ATVector2D ATVector2D::subtractTwoVectors(ATVector2D atv1, ATVector2D atv2)
 {
	 float x = atv1.x - atv2.x;
	 float y = atv1.y - atv2.y;

	 ATVector2D atv(x, y);
	 return atv;
 }

/**
* Normalizes a vector.
*/
ATVector2D ATVector2D::normalize(ATVector2D atv)
{
	ATVector2D atvA(atv.getX()/atv.magnitude(), atv.getY()/atv.magnitude());
	return atvA;
}

float ATVector2D::magnitude()
{
	return sqrtf((getX()*getX()) + (getY()*getY()));
}

float ATVector2D::dot(ATVector2D atv1, ATVector2D atv2)
{
	float costheta = cosf(0); //TODO: get theta
	float ret = atv1.magnitude()*atv2.magnitude()*costheta;
	return ret;
}