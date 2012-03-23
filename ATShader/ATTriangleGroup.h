#pragma once
#include "ATTriangle.h"
#include "ATColor.h"
#include "ATTextureMap.h"
#include "ATMatrix4.h"
#include <vector>

using namespace std;


class ATTriangleGroup
{
public:
	vector<ATTriangle> triangles;
	bool raw, textured;
	ATTextureMap textureRaster;
	float alpha;
	float shininess;
	ATColor specularColor;
	ATColor materialColor;
    ATMatrix4 modelView;

	ATTriangleGroup(bool rawType);

	void addTriangle(ATTriangle tri);
	
	bool isRaw();
	void setType(bool rawType);

	void clear();
};

