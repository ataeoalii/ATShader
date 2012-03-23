#ifndef _ATTRANSFORMATION_H_
#define _ATTRANSFORMATION_H_

#include <string>
#include <vector>
#include <iostream>
#include "ATTriangle.h"
#include "ATColor.h"
#include "ATMatrix4.h"

using namespace std;

class ATTransformation
{
public:

	static void colorTransform(ATMatrix4* atm, string values[]);
	static ATMatrix4 rotateTransform(ATMatrix4 matrix, string values[]);
	static ATMatrix4 translateTransform(ATMatrix4 matrix, string values[]);
	static ATMatrix4 scaleTransform(ATMatrix4 matrix, string values[]);
};

#endif