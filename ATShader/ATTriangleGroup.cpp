#include "ATTriangleGroup.h"

ATTriangleGroup::ATTriangleGroup(bool rawType)
{
	raw = rawType;
	textured = false;
	alpha = 1.0f;
	materialColor.setColor(1.0f, 1.0f, 1.0f, 1.0f);
	shininess = 0.0f;
}



void ATTriangleGroup::addTriangle(ATTriangle tri)
{
	triangles.push_back(tri);
}


bool ATTriangleGroup::isRaw()
{
	return raw;
}

void ATTriangleGroup::setType(bool rawType)
{
	raw = rawType;
}

void ATTriangleGroup::clear()
{
	triangles.clear();
	raw = false;
	alpha = 1.0f;
	shininess = 0.0f;
	specularColor.setColor(1.0f, 0.0f, 0.0f, 0.0f);
	materialColor.setColor(1.0f, 1.0f, 1.0f, 1.0f);
	textured = false;
	
}