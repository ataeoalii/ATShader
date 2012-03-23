#ifndef _ATMATRIX_H_
#define _ATMATRIX_H_
#include "ATVector3D.h"
#include "ATVector4D.h"
class ATMatrix4
{
	
private:
	float _matrix[4][4];

public:
	ATMatrix4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

	ATMatrix4();

	ATMatrix4(const ATMatrix4 &copy);

	static ATMatrix4 MkIdentityMatrix();

	static ATMatrix4 MkTranslateMatrix(float transX, float transY, float transZ);

	static ATMatrix4 MkRotateMatrix(float theta, ATVector4D axis);
	
	void XRotateMatrix(float theta);
	void YRotateMatrix(float theta);
	void ZRotateMatrix(float theta);
	
	static ATMatrix4 MkScaleMatrix(float scaleX, float scaleY, float scaleZ);

	static ATMatrix4 ConcatMatrices(ATMatrix4 m1, ATMatrix4 m2);

	static ATVector4D MultiplyMatrix(ATMatrix4 m1, ATVector4D vec);
	static ATVector3D MultiplyMatrix3D(ATMatrix4 m1, ATVector3D vec);
	static ATVector4D MultiplyMatrixZ0(ATMatrix4 m1, ATVector4D vec);

	static ATMatrix4 MkFrustum(float fovY, float aspectRatio, float nearDist, float farDist);
	static ATMatrix4 MkViewport(float x, float y, float width, float height);
	static ATMatrix4 MkPerspective(float fovY, float aspectRatio, float nearDist, float farDist);
	static ATMatrix4 MkLookAt(ATVector4D eyePt, ATVector4D spotPt, ATVector4D upVector);

};

#endif