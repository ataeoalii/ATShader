#ifndef _ATCOLOR_H_
#define _ATCOLOR_H_
#include <stdint.h>

class ATColor
{
private:
	uint8_t redB, greenB, blueB, alphaB;
	
public:
	ATColor(uint32_t colorValueARGB);
	ATColor(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
	ATColor(float a, float r, float g, float b);

	ATColor(const ATColor &copy);

	ATColor();
	float getredF();
	float getgreenF();
	float getblueF();
	float getalphaF();
	uint8_t getredB();
	uint8_t getgreenB();
	uint8_t getblueB();
	uint8_t getalphaB();
	uint32_t getColor();

	void setredB(uint8_t r);
	void setgreenB(uint8_t g);
	void setblueB(uint8_t b);
	void setalphaB(uint8_t a);
	void setColor(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
	void setColor(float a, float r, float g, float b);
	void scaleColor(float scalar);

	static ATColor multiply(ATColor col1, ATColor col2);
	static ATColor addTwoColors(ATColor col1, ATColor col2);
};

#endif