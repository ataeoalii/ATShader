#ifndef _ATTRIANGLE_H_
#define _ATTRIANGLE_H_
#include "ATVector4D.h"
#include "ATVector3D.h"
#include "ATVector2D.h"
#include "ATColor.h"

class ATTriangle
{

public:
	ATVector4D vertA, vertB, vertC;
	ATVector3D normA, normB, normC;
	ATVector2D textA, textB, textC;

	// Used only for phong interpolation
	bool phong;
	ATVector4D eyeA, eyeB, eyeC;

	ATTriangle(ATVector4D v1, ATVector4D v2, ATVector4D v3);
	ATTriangle(ATVector4D v1, ATVector4D v2, ATVector4D v3,
		ATVector3D norm1, ATVector3D norm2, ATVector3D norm3,
		ATVector2D tex1, ATVector2D tex2, ATVector2D tex3);
	ATTriangle();

};

#endif