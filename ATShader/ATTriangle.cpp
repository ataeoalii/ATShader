#include "ATTriangle.h"
#include "ATVector4D.h"

/**
* Constructor without normals or textures.
* 
*/
ATTriangle::ATTriangle(ATVector4D v1, ATVector4D v2, ATVector4D v3)
{
	vertA = v1;
	vertB = v2;
	vertC = v3;
	phong = false;
}

/**
* Constructor with normals and textures.
*/
ATTriangle::ATTriangle(ATVector4D v1, ATVector4D v2, ATVector4D v3,
		ATVector3D norm1, ATVector3D norm2, ATVector3D norm3,
		ATVector2D tex1, ATVector2D tex2, ATVector2D tex3)
{
	vertA = v1;
	vertB = v2;
	vertC = v3;

	normA = norm1;
	normB = norm2;
	normC = norm3;

	textA = tex1;
	textB = tex2;
	textC = tex3;
	phong = false;
}

/**
* Default constructor.
*/ 
ATTriangle::ATTriangle()
{
	vertA = ATVector4D();
	vertB = ATVector4D();
	vertC = ATVector4D();
	phong = false;
}