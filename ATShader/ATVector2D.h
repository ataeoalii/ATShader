#ifndef _ATVECTOR2D_H_
#define _ATVECTOR2D_H_



class ATVector2D
{
private:
	float x, y;
	

public:
	ATVector2D();
	ATVector2D(float _x, float _y);

	float getX();
	float getY();

	void addVector(ATVector2D atv);
	void scaleVector(float scalar);
	void multiplyVector(ATVector2D atv);
	static ATVector2D multiplyTwoVectors(ATVector2D atv1, ATVector2D atv2);
	static ATVector2D addTwoVectors(ATVector2D atv1, ATVector2D atv2);
	static ATVector2D subtractTwoVectors(ATVector2D atv1, ATVector2D atv2);
	static ATVector2D normalize(ATVector2D atv);

	static ATVector2D mkATVector2D(float _x, float _y);

	static float dot(ATVector2D atv1, ATVector2D atv2);

	float magnitude();


};

#endif