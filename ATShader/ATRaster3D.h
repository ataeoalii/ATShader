#ifndef _ATRaster3D_H_
#define _ATRaster3D_H_
#include "ATColor.h"
#include "ATTriangle.h"
#include "ATVector4D.h"
#include "ATVector3D.h"
#include "ATLight.h"
#include "ATScene.h"
#include "ATTextureMap.h"
#include <stdint.h>
#include <iostream>
#include <vector>


using namespace std;

class ATTriangleGroup;
class ATScene;


class ATRaster3D
{

private:
	int width, height, _size;
	ATColor* rasterPixels;
	float* depths;
	
public:
	
	ATRaster3D();
	ATRaster3D(const ATRaster3D &copy);
	~ATRaster3D();
	ATRaster3D(int _width, int _height);

	int getHeight();
	int getWidth();
	ATColor* getPixels();
	
	float* getDepths();
	
	float getDepth(unsigned int index);
	
	void setDepth(unsigned int index, float value);

	void setPixel(int index, ATColor color);
	ATColor getPixel(int index);

	void blendPixel(int index, ATColor color, float alpha);

	static void ATRasterizeTriangle(ATRaster3D raster, ATTriangle tri, ATColor fillColor);

	static void ATRasterizeInterpolatedTriangle(ATRaster3D raster, ATTriangle tri, int minX, int maxX, int minY, int maxY, float alpha);

	static void ATRasterizeInterpolatedTriangles(ATRaster3D raster, ATTriangleGroup triangles, float alpha);

	static void GetBarycentricCoordinates(ATVector4D vect, ATVector4D atv1, ATVector4D atv2, ATVector4D atv3, 
											float* lambda1, float* lambda2, float* lambda3);


	static void ATSaveToPPM(ATColor pixels[], int width, int height, char* path);

	static void ATSaveToBMP(ATColor pixels[], int width, int height, char* path);

	static ATColor calculateColor(ATColor ambientColor, ATColor materialColor, ATColor specularColor, const vector<ATLight> &lights,
								ATVector3D normal, ATVector4D point, float shininess);

	static void PhongInterpolateTriangle(ATTextureMap* textureRaster, bool textured, ATRaster3D* raster, ATColor ambientColor, ATColor materialColor, ATColor specularColor, const vector<ATLight> &lights, ATTriangle tri, int minX, int maxX, int minY, int maxY, float alpha, float shininess);

	static void PhongInterpolateScene(ATRaster3D* raster, const ATScene* scene, ATColor ambientColor);


};
#endif