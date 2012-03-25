#pragma once
#include "ATColor.h"

class ATTextureMap
{
private:
	int width, height;
	ATColor* pixelMap;
public:
	ATTextureMap();
	ATTextureMap(int _width, int _height);
	ATTextureMap(const ATTextureMap &copy);
	~ATTextureMap();
    
    void cleanup();

	void setHeight(int h);
	void setWidth(int w);
	int getHeight();
	int getWidth();

	void setPixel(int index, ATColor color);
	ATColor getPixel(int index);
};

