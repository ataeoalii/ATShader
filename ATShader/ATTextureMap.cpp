
#include "ATTextureMap.h"


ATTextureMap::ATTextureMap()
{
	width = 256;
	height = 256;
	pixelMap = new ATColor[width*height];
}

ATTextureMap::ATTextureMap(int _width, int _height)
{
	width = _width;
	height = _height;
	pixelMap = new ATColor[width*height];
}

ATTextureMap::ATTextureMap(const ATTextureMap &copy)
{
	width = copy.width;
	height = copy.height;
	pixelMap = new ATColor[width*height];

	for (int i=0; i<width*height; i++)
	{
		pixelMap[i] = copy.pixelMap[i];
	}
}

ATTextureMap::~ATTextureMap(void)
{
}

void ATTextureMap::setPixel(int index, ATColor color)
{
	pixelMap[index] = color;
}

ATColor ATTextureMap::getPixel(int index)
{
	return pixelMap[index];
}

void ATTextureMap::setHeight(int h)
{
	height = h;
	delete pixelMap;
	pixelMap = new ATColor[width*height];
}

void ATTextureMap::setWidth(int w)
{
	width = w;
}

int ATTextureMap::getHeight()
{
	return height;
}

int ATTextureMap::getWidth()
{
	return width;
}

void ATTextureMap::cleanup()
{
    delete pixelMap;
}