#pragma once
#include "ATLight.h"
#include "ATTriangleGroup.h"
#include "ATMatrix4.h"
#include <vector>

using namespace std;


class ATScene
{
public:
	vector<ATTriangleGroup> objectsInScene;
	vector<ATLight> lights;
    ATMatrix4 camera;
    ATMatrix4 perspective;
    ATMatrix4 viewport;
    ATColor ambientColor;
    int width, height;
	
    ATScene();
	
    ~ATScene();
	
    void addLightToScene(ATLight light);

	void addObjectToScene(ATTriangleGroup obj);

	vector<ATLight> getLights() const;

	vector<ATTriangleGroup> getObjects() const;
    
    bool isWhitespace(char c);
    
    bool isValNumber(char c);
    
    void splitString(string line, string retVals[], unsigned int count);
    
    vector<ATVector4D> createVectors(char allCharsFromFile[], unsigned long count, float* totalX, float* totalY, int* numVals);
    
    bool isLineWhitespace(string line);
    
    ATTriangleGroup readRawFile( string filename, float* totalX, float* totalY, int* numVals);
    
    ATTextureMap readInBMPTexture(string filename);
    
    ATTextureMap readInPPMTexture(string filename);
    
    ATMatrix4 createTransformationMatrix(int* textured, ATTextureMap* textureRaster, ATMatrix4 tempCamera, string transformation, ATColor* materialColor, ATColor* specularColor, float* alpha);
    
    ATTriangleGroup ObjReader(string filename, float *totalX, float *totalY, int *numVals);
    
    void sceneReaderPhong(char* filename);
    
};

