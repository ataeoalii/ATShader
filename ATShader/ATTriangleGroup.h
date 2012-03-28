#ifndef _ATTRIANGLEGROUP_H_
#define _ATTRIANGLEGROUP_H_
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
	string* textureMap;
    int textWidth;
    int textHeight;
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

#endif