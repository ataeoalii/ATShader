#include "ATMatrix4.h"
#include "ATVector4D.h"
#include <math.h>

ATMatrix4::ATMatrix4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
{
	_matrix[0][0] = m00;
	_matrix[0][1] = m01;
	_matrix[0][2] = m02;
	_matrix[0][3] = m03;
	_matrix[1][0] = m10;
	_matrix[1][1] = m11;
	_matrix[1][2] = m12;
	_matrix[1][3] = m13;
	_matrix[2][0] = m20;
	_matrix[2][1] = m21;
	_matrix[2][2] = m22;
	_matrix[2][3] = m23;
	_matrix[3][0] = m30;
	_matrix[3][1] = m31;
	_matrix[3][2] = m32;
	_matrix[3][3] = m33;
}

ATMatrix4::ATMatrix4()
{
	ATMatrix4::MkIdentityMatrix();
}

ATMatrix4::ATMatrix4(const ATMatrix4 &copy)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			_matrix[i][j] = copy._matrix[i][j];
		}
	}	
}


ATMatrix4 ATMatrix4::MkIdentityMatrix()
{
	ATMatrix4 atm(1,0,0,0,
				  0,1,0,0,
				  0,0,1,0,
				  0,0,0,1);
	return atm;
}

ATMatrix4 ATMatrix4::ConcatMatrices(ATMatrix4 atm1, ATMatrix4 atm2)
{
	ATMatrix4 atm;
	atm._matrix[0][0] = atm1._matrix[0][0]*atm2._matrix[0][0] + 
						atm1._matrix[0][1]*atm2._matrix[1][0] + 
						atm1._matrix[0][2]*atm2._matrix[2][0] + 
						atm1._matrix[0][3]*atm2._matrix[3][0];

	atm._matrix[0][1] = atm1._matrix[0][0]*atm2._matrix[0][1] + 
						atm1._matrix[0][1]*atm2._matrix[1][1] + 
						atm1._matrix[0][2]*atm2._matrix[2][1] + 
						atm1._matrix[0][3]*atm2._matrix[3][1];

	atm._matrix[0][2] = atm1._matrix[0][0]*atm2._matrix[0][2] + 
						atm1._matrix[0][1]*atm2._matrix[1][2] + 
						atm1._matrix[0][2]*atm2._matrix[2][2] + 
						atm1._matrix[0][3]*atm2._matrix[3][2];

	atm._matrix[0][3] = atm1._matrix[0][0]*atm2._matrix[0][3] + 
						atm1._matrix[0][1]*atm2._matrix[1][3] + 
						atm1._matrix[0][2]*atm2._matrix[2][3] + 
						atm1._matrix[0][3]*atm2._matrix[3][3];

	atm._matrix[1][0] = atm1._matrix[1][0]*atm2._matrix[0][0] + 
						atm1._matrix[1][1]*atm2._matrix[1][0] + 
						atm1._matrix[1][2]*atm2._matrix[2][0] + 
						atm1._matrix[1][3]*atm2._matrix[3][0];

	atm._matrix[1][1] = atm1._matrix[1][0]*atm2._matrix[0][1] + 
						atm1._matrix[1][1]*atm2._matrix[1][1] + 
						atm1._matrix[1][2]*atm2._matrix[2][1] + 
						atm1._matrix[1][3]*atm2._matrix[3][1];

	atm._matrix[1][2] = atm1._matrix[1][0]*atm2._matrix[0][2] + 
						atm1._matrix[1][1]*atm2._matrix[1][2] + 
						atm1._matrix[1][2]*atm2._matrix[2][2] + 
						atm1._matrix[1][3]*atm2._matrix[3][2];

	atm._matrix[1][3] = atm1._matrix[1][0]*atm2._matrix[0][3] + 
						atm1._matrix[1][1]*atm2._matrix[1][3] + 
						atm1._matrix[1][2]*atm2._matrix[2][3] + 
						atm1._matrix[1][3]*atm2._matrix[3][3];

	atm._matrix[2][0] = atm1._matrix[2][0]*atm2._matrix[0][0] + 
						atm1._matrix[2][1]*atm2._matrix[1][0] + 
						atm1._matrix[2][2]*atm2._matrix[2][0] + 
						atm1._matrix[2][3]*atm2._matrix[3][0];

	atm._matrix[2][1] = atm1._matrix[2][0]*atm2._matrix[0][1] + 
						atm1._matrix[2][1]*atm2._matrix[1][1] + 
						atm1._matrix[2][2]*atm2._matrix[2][1] + 
						atm1._matrix[2][3]*atm2._matrix[3][1];

	atm._matrix[2][2] = atm1._matrix[2][0]*atm2._matrix[0][2] + 
						atm1._matrix[2][1]*atm2._matrix[1][2] + 
						atm1._matrix[2][2]*atm2._matrix[2][2] + 
						atm1._matrix[2][3]*atm2._matrix[3][2];

	atm._matrix[2][3] = atm1._matrix[2][0]*atm2._matrix[0][3] + 
						atm1._matrix[2][1]*atm2._matrix[1][3] + 
						atm1._matrix[2][2]*atm2._matrix[2][3] + 
						atm1._matrix[2][3]*atm2._matrix[3][3];

	atm._matrix[3][0] = atm1._matrix[3][0]*atm2._matrix[0][0] + 
						atm1._matrix[3][1]*atm2._matrix[1][0] + 
						atm1._matrix[3][2]*atm2._matrix[2][0] + 
						atm1._matrix[3][3]*atm2._matrix[3][0];

	atm._matrix[3][1] = atm1._matrix[3][0]*atm2._matrix[0][1] + 
						atm1._matrix[3][1]*atm2._matrix[1][1] + 
						atm1._matrix[3][2]*atm2._matrix[2][1] + 
						atm1._matrix[3][3]*atm2._matrix[3][1];

	atm._matrix[3][2] = atm1._matrix[3][0]*atm2._matrix[0][2] + 
						atm1._matrix[3][1]*atm2._matrix[1][2] + 
						atm1._matrix[3][2]*atm2._matrix[2][2] + 
						atm1._matrix[3][3]*atm2._matrix[3][2];

	atm._matrix[3][3] = atm1._matrix[3][0]*atm2._matrix[0][3] + 
						atm1._matrix[3][1]*atm2._matrix[1][3] + 
						atm1._matrix[3][2]*atm2._matrix[2][3] + 
						atm1._matrix[3][3]*atm2._matrix[3][3];
	return atm;	
}
	

ATMatrix4 ATMatrix4::MkScaleMatrix(float scaleX, float scaleY, float scaleZ)
{
	ATMatrix4 atm(scaleX, 0, 0, 0,
				  0, scaleY, 0, 0,
				  0, 0, scaleZ, 0,
				  0, 0, 0, 1);

	return atm;
}


ATMatrix4 ATMatrix4::MkTranslateMatrix(float transX, float transY, float transZ)
{
	ATMatrix4 atm(1, 0, 0, transX,
				  0, 1, 0, transY,
				  0, 0, 1, transZ, 
				  0, 0, 0, 1);
	return atm;
}

void ATMatrix4::XRotateMatrix(float theta)
{
	float costheta = cosf(theta);
	float sintheta = sinf(theta);

	_matrix[0][0]= 1;
	_matrix[0][1]= 0;
	_matrix[0][2]= 0;
	_matrix[0][3]= 0;

	_matrix[1][0]= 0;
	_matrix[1][1]= costheta;
	_matrix[1][2]= -sintheta;
	_matrix[1][3]= 0;

	_matrix[2][0]= 0;
	_matrix[2][1]= sintheta;
	_matrix[2][2]= costheta;
	_matrix[2][3]= 0;

	_matrix[3][0]= 0;
	_matrix[3][1]= 0;
	_matrix[3][2]= 0;
	_matrix[3][3]= 1;
}

void ATMatrix4::YRotateMatrix(float theta)
{
	float costheta = cosf(theta);
	float sintheta = sinf(theta);

	_matrix[0][0]= costheta;
	_matrix[0][1]= 0;
	_matrix[0][2]= sintheta;
	_matrix[0][3]= 0;

	_matrix[1][0]= 0;
	_matrix[1][1]= 1;
	_matrix[1][2]= 0;
	_matrix[1][3]= 0;

	_matrix[2][0]= -sintheta;
	_matrix[2][1]= 0;
	_matrix[2][2]= costheta;
	_matrix[2][3]= 0;

	_matrix[3][0]= 0;
	_matrix[3][1]= 0;
	_matrix[3][2]= 0;
	_matrix[3][3]= 1;
}

void ATMatrix4::ZRotateMatrix(float theta)
{
	float costheta = cosf(theta);
	float sintheta = sinf(theta);

	_matrix[0][0]= costheta;
	_matrix[0][1]= -sintheta;
	_matrix[0][2]= 0;
	_matrix[0][3]= 0;

	_matrix[1][0]= sintheta;
	_matrix[1][1]= costheta;
	_matrix[1][2]= 0;
	_matrix[1][3]= 0;

	_matrix[2][0]= 0;
	_matrix[2][1]= 0;
	_matrix[2][2]= 1;
	_matrix[2][3]= 0;

	_matrix[3][0]= 0;
	_matrix[3][1]= 0;
	_matrix[3][2]= 0;
	_matrix[3][3]= 1;
}


ATMatrix4 ATMatrix4::MkRotateMatrix(float theta, ATVector4D axis)
{	
	float a1 = cosf(theta) + ((axis.getX()*axis.getX())*(1-cosf(theta)));
	float a2 = axis.getX()*axis.getY()*(1-cosf(theta)) - axis.getZ()*sinf(theta);
	float a3 = (axis.getX()*axis.getZ())*(1-cosf(theta)) + ((axis.getY()*sinf(theta)));
	float a4 = 0;

	float b1 = axis.getX()*axis.getY()*(1-cosf(theta)) + axis.getZ()*sinf(theta);
	float b2 = cosf(theta) + axis.getY()*axis.getY()*(1-cosf(theta));
	float b3 = axis.getY()*axis.getZ()*(1-cosf(theta)) - (axis.getX()*sinf(theta));
	float b4 = 0;

	float c1 = axis.getZ()*axis.getX()*(1-cosf(theta)) - axis.getY()*sinf(theta);
	float c2 = axis.getZ()*axis.getY()*(1-cosf(theta)) + axis.getX()*sinf(theta);
	float c3 = cosf(theta) + ((axis.getZ()*axis.getZ())*(1-cosf(theta)));
	float c4 = 0;

	float d1 = 0;
	float d2 = 0;
	float d3 = 0;
	float d4 = 1;

	ATMatrix4 atm(a1, a2, a3, a4,  b1, b2, b3, b4, c1, c2, c3, c4,  d1, d2, d3, d4);

	return atm;
}


ATVector4D ATMatrix4::MultiplyMatrix(ATMatrix4 m1, ATVector4D vec)
{
	float x = m1._matrix[0][0]*vec.getX() + m1._matrix[0][1]*vec.getY() + m1._matrix[0][2]*vec.getZ()+ m1._matrix[0][3]*1.0f;
	float y = m1._matrix[1][0]*vec.getX() + m1._matrix[1][1]*vec.getY() + m1._matrix[1][2]*vec.getZ()+ m1._matrix[1][3]*1.0f;
	float z = m1._matrix[2][0]*vec.getX() + m1._matrix[2][1]*vec.getY() + m1._matrix[2][2]*vec.getZ()+ m1._matrix[2][3]*1.0f;
	float w = m1._matrix[3][0]*vec.getX() + m1._matrix[3][1]*vec.getY() + m1._matrix[3][2]*vec.getZ()+ m1._matrix[3][3]*1.0f;
	return ATVector4D(x, y, z, w);
}


ATVector3D ATMatrix4::MultiplyMatrix3D(ATMatrix4 m1, ATVector3D vec)
{
	float x = m1._matrix[0][0]*vec.getX() + m1._matrix[0][1]*vec.getY() + m1._matrix[0][2]*vec.getZ();
	float y = m1._matrix[1][0]*vec.getX() + m1._matrix[1][1]*vec.getY() + m1._matrix[1][2]*vec.getZ();
	float z = m1._matrix[2][0]*vec.getX() + m1._matrix[2][1]*vec.getY() + m1._matrix[2][2]*vec.getZ();
	return ATVector3D(x, y, z);
}


ATVector4D ATMatrix4::MultiplyMatrixZ0(ATMatrix4 m1, ATVector4D vec)
{
	float x = m1._matrix[0][0]*vec.getX() + m1._matrix[0][1]*vec.getY() + m1._matrix[0][2]*vec.getZ()+ m1._matrix[0][3]*1.0f;
	float y = m1._matrix[1][0]*vec.getX() + m1._matrix[1][1]*vec.getY() + m1._matrix[1][2]*vec.getZ()+ m1._matrix[1][3]*1.0f;
	float w = m1._matrix[3][0]*vec.getX() + m1._matrix[3][1]*vec.getY() + m1._matrix[3][2]*vec.getZ()+ m1._matrix[3][3]*1.0f;
	return ATVector4D(x, y, vec.getZ(), w);
}

ATMatrix4 ATMatrix4::MkFrustum(float fovY, float aspectRatio, float nearDist, float farDist)
{
	float l, r, b, t, n, f;
	t = nearDist * tanf(fovY * 3.14159265358979 / 360.0f);
	b = -t;
	l = b * aspectRatio;
	r = t * aspectRatio;
	n = nearDist;
	f = farDist;

	ATMatrix4 atm((2*n / (r-l)),		   0,   (r+l)/(r-l),				 0,
							  0, 2*n / (t-b),   (t+b)/(t-b),		  	   	 0,
							  0,		   0,  -(f+n)/(f-n),    -2*f*n / (f-n),
							  0,		   0,			 -1,				0);
	return atm;
}

ATMatrix4 ATMatrix4::MkPerspective(float fovY, float aspectRatio, float nearDist, float farDist)
{
	float scaleX = tanf(fovY * 0.5f)/ aspectRatio;
	float scaleY = 1.0f / tanf(fovY * 0.5f);
	float scaleZ = -(farDist + nearDist) / (farDist - nearDist);
	float transZ = -2.0f*farDist*nearDist / (farDist - nearDist);


	ATMatrix4 atm(scaleX,	 0.0f,   0.0f,		0.0f,
					0.0f,  scaleY,   0.0f,		0.0f,
					0.0f,	 0.0f, scaleZ,    transZ,
					0.0f,	 0.0f,	-1.0f,		0.0f);
	return atm;
}


ATMatrix4 ATMatrix4::MkViewport(float x, float y, float width, float height)
{
	ATMatrix4 atm = ATMatrix4::MkTranslateMatrix(x, y, 0.0f);
	atm = ATMatrix4::ConcatMatrices(atm, ATMatrix4::MkScaleMatrix(width*0.5f, height*0.5f, 0.5f));
	atm = ATMatrix4::ConcatMatrices(atm, ATMatrix4::MkTranslateMatrix(1.0f, 1.0f, 1.0f));
	return atm;
}

ATMatrix4 ATMatrix4::MkLookAt(ATVector4D eyePt, ATVector4D spotPt, ATVector4D upVector)
{
	ATVector4D look = ATVector4D::normalize(ATVector4D::subtractTwoVectors(spotPt, eyePt));
	ATVector4D right = ATVector4D::normalize(ATVector4D::crossProduct(look, upVector));
	ATVector4D up = ATVector4D::normalize(ATVector4D::crossProduct(right, look));

	ATMatrix4 atm(right.getX(), right.getY(), right.getZ(), 0.0f,
					 up.getX(),	   up.getY(),	 up.getZ(), 0.0f,
				  -look.getX(), -look.getY(), -look.getZ(), 0.0f,
					      0.0f,		 0.0f,		   0.0f,	1.0f);

	ATMatrix4 t = ATMatrix4::MkTranslateMatrix(-eyePt.getX(), -eyePt.getY(), -eyePt.getZ());

	ATMatrix4 atm1 = ATMatrix4::ConcatMatrices(atm, t);

	return atm1;
}
