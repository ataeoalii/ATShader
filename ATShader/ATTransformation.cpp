#include "ATTransformation.h"
#include "ATMatrix4.h"

//TODO: change matrix to vector for color
void ATTransformation::colorTransform(ATMatrix4* atm, string values[])
{
	// set color of vertex A
	float a = (float)atof(values[1].c_str());
	float b = (float)atof(values[2].c_str());
	float c = (float)atof(values[3].c_str());
	// set color of vertex B
	float d = (float)atof(values[4].c_str());
	float e = (float)atof(values[5].c_str());
	float f = (float)atof(values[6].c_str());
	// set color of vertex C
	float g = (float)atof(values[7].c_str());
	float h = (float)atof(values[8].c_str());
	float i = (float)atof(values[9].c_str());

	*atm = ATMatrix4(1.0f, a, b, c, 1.0f, d, e, f, 1.0f, g, h, i, 0.0f, 0.0f, 0.0f, 0.0f);

}

ATMatrix4 ATTransformation::rotateTransform(ATMatrix4 matrix, string values[])
{
	float theta = (float)atof(values[1].c_str());
	float x = (float)atof(values[2].c_str());
	float y = (float)atof(values[3].c_str());
	float z = (float)atof(values[4].c_str());
	ATVector4D axis(x, y, z, 1.0f);

	ATMatrix4 atm = ATMatrix4::MkRotateMatrix(theta, axis);

	ATMatrix4 atm1 = ATMatrix4::ConcatMatrices(matrix, atm);

	return atm1;
}

ATMatrix4 ATTransformation::translateTransform(ATMatrix4 matrix, string values[])
{
	float transX = (float)atof(values[1].c_str());
	float transY = (float)atof(values[2].c_str());
	float transZ = (float)atof(values[3].c_str());
	
	ATMatrix4 atm = ATMatrix4::ConcatMatrices(matrix, ATMatrix4::MkTranslateMatrix(transX, transY, transZ));
	return atm;
}

ATMatrix4 ATTransformation::scaleTransform(ATMatrix4 matrix,string values[])
{
	float scaleX = (float)atof(values[1].c_str());
	float scaleY = (float)atof(values[2].c_str());
	float scaleZ = (float)atof(values[3].c_str());
	
	ATMatrix4 atm = ATMatrix4::ConcatMatrices(matrix, ATMatrix4::MkScaleMatrix(scaleX, scaleY, scaleZ));

	return atm;

}