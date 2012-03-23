#pragma once
#include"ATVector4D.h"
#include "ATColor.h"

class ATLight
{
private:
	ATColor color;
	ATVector4D position;
public:
	ATLight(ATVector4D pos, ATColor col);
	~ATLight(void);

	ATColor getColor();
	ATVector4D getPosition();
	void setPosition(float x, float y, float z);
	void setColor(float a, float r, float g, float b);
};

