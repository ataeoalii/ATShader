
#include "ATScene.h"
#include "ATMatrix4.h"
#include "ATTransformation.h"
#include <iostream>
#include <fstream>

const char* FILEPREFIX = "../../../../../";


using namespace std;


ATScene::ATScene()
{
}

ATScene::~ATScene()
{
}

void ATScene::addLightToScene(ATLight light)
{
	lights.push_back(light);
}

void ATScene::addObjectToScene(ATTriangleGroup obj)
{
	objectsInScene.push_back(obj);
}

vector<ATLight> ATScene::getLights() const
{
	return (const vector<ATLight>)lights;
}

vector<ATTriangleGroup> ATScene::getObjects() const
{
	return (const vector<ATTriangleGroup>)objectsInScene;
}

bool ATScene::isWhitespace(char c)
{
	if (c==' ' ||c=='\t' ||c=='\n' ||c=='\v'|| c=='\f' ||c=='\r')
		return true;
	else
		return false;
}

bool ATScene::isValNumber(char c)
{
	if (c=='0' ||c=='1' ||c=='2' ||c=='3'|| c=='4' ||c=='5' ||c=='6' ||c=='7' ||c=='8'|| c=='9' ||c=='.' || c=='-')
		return true;
	else
		return false;
}

void ATScene::splitString(string line, string retVals[], unsigned int count)
{
	char* lineArr = (char*)line.c_str();
	unsigned int start=0, splindex;
	for(unsigned int i = 0; i < count; i++)
	{
		while(isWhitespace(lineArr[start]))
		{
			start++;
		}
		splindex = start;
        
		while(!isWhitespace(lineArr[splindex]) && splindex < line.length())
		{
			splindex++;
		}
		retVals[i] = line.substr(start, splindex);
		start = splindex;
	}
}


/**
 * Creates a vector of ATVector3Ds from the RAW file so they
 * can have all the transformations applied to each one of them.
 */
vector<ATVector4D> ATScene::createVectors(char allCharsFromFile[], unsigned long count, float* totalX, float* totalY, int* numVals)
{
	vector<ATVector4D> atvs;
	unsigned int idx = 0;
	float x, y, z;
	string tempString;
	char c = allCharsFromFile[idx];
	while(idx<count)
	{
		while(isWhitespace(c) || !isValNumber(c))
		{
			if(idx==count-1) return atvs;
			c = allCharsFromFile[++idx];
		}
		while(!isWhitespace(c) && isValNumber(c))
		{
			if(idx==count-1) return atvs;
			tempString+=c;
			c = allCharsFromFile[++idx];
		}
		x = atof(tempString.c_str());
		*totalX+=x;
		tempString = "";
		while(isWhitespace(c) || !isValNumber(c))
		{
			if(idx==count-1) return atvs;
			c = allCharsFromFile[++idx];
		}
		while(!isWhitespace(c) && isValNumber(c))
		{
			if(idx==count-1) return atvs;
			tempString+=c;
			c = allCharsFromFile[++idx];
		}
		y = atof(tempString.c_str());
		*totalY+=y;
		tempString = "";
		while(isWhitespace(c) || !isValNumber(c))
		{
			if(idx==count-1) return atvs;
			c = allCharsFromFile[++idx];
		}
		while(!isWhitespace(c) && isValNumber(c))
		{
			if(idx==count-1) return atvs;
			tempString+=c;
			c = allCharsFromFile[++idx];
		}
		z = atof(tempString.c_str());
		*numVals +=1;
		tempString = "";
		atvs.push_back(ATVector4D(x, y, z, 1.0f));
	}
	return atvs;
    
}


bool ATScene::isLineWhitespace(string line)
{
	char* lineArr = (char *) line.c_str();
	for(unsigned int i=0; i < line.length(); i++)
	{
		if(!isWhitespace(lineArr[i]))
			return false;
	}
	return true;
}

/**
 * 1. Reads in the RAW file and gets all the triangles from it and applies
 *    the transformation matrix to each of the triangles.
 */
ATTriangleGroup ATScene::readRawFile( string filename, float* totalX, float* totalY, int* numVals)
{
	ATTriangleGroup temptriangles(true);
	ATVector3D v1, v2, v3;
	long length;
    cout << "Reading Triangle data from " << filename << "\n";
    ifstream rawFile(filename.c_str());
    if(!rawFile)
    {
        cerr << "Your raw file could not be found!";
        cin.get();
    } 
    else
    {
        rawFile.seekg(0, ios::end);   
        length = rawFile.tellg();          
        rawFile.seekg(0, std::ios::beg);   
        char* allCharsFromFile = new char[length];   
        rawFile.read(allCharsFromFile, length);      
        rawFile.close();                   
        
        // create all vectors from the file
        vector<ATVector4D> atvs = createVectors(allCharsFromFile, length, totalX, totalY, numVals);
        
        delete allCharsFromFile;
        // use the vectors to create triangles
        for(unsigned int c = 0; c < atvs.size()/3; c++)
        {
            ATTriangle att(atvs[c*3], atvs[c*3+1], atvs[c*3+2]);
            
            ATVector4D norm4 = ATVector4D::crossProduct(
                                                        ATVector4D::subtractTwoVectors(att.vertB,  att.vertA),
                                                        ATVector4D::subtractTwoVectors(att.vertC,  att.vertA));
            
            ATVector3D norm(norm4.getX(), norm4.getY(), norm4.getZ());
            
            temptriangles.addTriangle(ATTriangle(atvs[c*3], atvs[c*3+1], atvs[c*3+2], norm, norm, norm, ATVector2D(0,0), ATVector2D(0,0), ATVector2D(0,0)));
        }
        
    }
	return temptriangles;
}



/**
 * Creates transformation matrix based off lines from the scene file.
 */
ATMatrix4 ATScene::createTransformationMatrix(int* textured, string* textureMap, ATMatrix4 tempCamera, string transformation, ATColor* materialColor, ATColor* specularColor, float* alpha)
{
	int index = 0;
	while (ATScene::isWhitespace(transformation[index]))
	{
		index++;
	}
    
	if(transformation[index]=='c')
	{
		string values[5];
		splitString(transformation, values, 5);
        
		float a = atof(values[1].c_str());
		float r = atof(values[2].c_str());
		float g = atof(values[3].c_str());
		float b = atof(values[4].c_str());
        
		materialColor->setColor(a,r,g,b);
		return tempCamera;
	}
	else if(transformation[index]=='p')
	{
		string values[5];
		splitString(transformation, values, 5);
		float r = atof(values[1].c_str());
		float g = atof(values[2].c_str());
		float b = atof(values[3].c_str());
		*alpha = atof(values[4].c_str());
        
		specularColor->setColor(1.0f, r, g, b);
		return tempCamera;
	}
	else if(transformation[index]=='m')
	{
		string values[2];
		splitString(transformation, values, 2);
		(*textureMap) = values[1];
		(*textured) = 1;
		return tempCamera;
	}
	else if(transformation[index]=='t')
	{
		string values[4];
		splitString(transformation, values, 4);
		return ATTransformation::translateTransform(tempCamera, values);
	}
	else if(transformation[index]=='r')
	{
		string values[5];
		splitString(transformation, values, 5);
		return ATTransformation::rotateTransform(tempCamera, values);
	}
	else if(transformation[index]=='s')
	{
		string values[4];
		splitString(transformation, values, 4);
		return ATTransformation::scaleTransform(tempCamera, values);
	}
	else
	{
		// no transformation
		return tempCamera;
	}
}

/**
 *  Reads in an object file and creates vectors containing
 *  the data from the file.
 *
 */
ATTriangleGroup ATScene::ObjReader(string filename, float *totalX, float *totalY, int *numVals)
{
    ifstream objFile(filename.c_str());
    string line;
	vector<ATVector3D> normals;
	vector<ATVector2D> textures;
	vector<ATVector4D> vectors;
    
	ATTriangleGroup faces(false);
    
    // Read in file and create normals, textures, vectors and faces
    if (objFile.is_open())
    {
        while ( objFile.good() )
        {
            getline (objFile,line);
            if(!line.empty())
            {
                if(line[0]=='v')
                {
                    if(line[1]=='n')
                    {
                        // vn x y z
                        // create new normal ATVector3D
                        
                        string arr[4];
                        
                        splitString(line, arr, 4);
                        
                        float x = atof(arr[1].c_str());
                        float y = atof(arr[2].c_str());
                        float z = atof(arr[3].c_str());
                        
                        ATVector3D tempVec(x,y,z);
                        
                        normals.push_back(tempVec);
                    }
                    else if(line[1]=='t')
                    {
                        // vt u v
                        // create new texture ATVector2D
                        string arr[3];
                        
                        splitString(line, arr, 3);
                        
                        float u = atof(arr[1].c_str());
                        float v = atof(arr[2].c_str());
                        
                        ATVector2D tempVec(u,v);
                        
                        textures.push_back(tempVec);
                    }
                    else if(isWhitespace(line[1]))
                    {
                        // v x y z w
                        // ATVector4D
                        string arr[5];
                        
                        splitString(line, arr, 5);
                        
                        float x = atof(arr[1].c_str());
                        float y = atof(arr[2].c_str());
                        float z = atof(arr[3].c_str());
                        float w = 1.0f;
                        
                        ATVector4D atv4(x, y, z, w);
                        vectors.push_back(atv4);
                    }
                    else
                    {
                        // ignore
                    }
                }
                else if(line[0]=='f')
                {
                    // format = f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
                    // create face from textures, normals, vectors
                    // scan through the line and look for numbers
                    int startIndex = 1, endIndex;
                    
                    // first vector
                    while(line[startIndex]==' ') startIndex++;
                    endIndex = startIndex;
                    while(line[endIndex]!='/') endIndex++;
                    int vectorIndex1  = atoi(line.substr(startIndex, endIndex-startIndex).c_str());
                    
                    // first texture
                    startIndex = ++endIndex;
                    while(line[endIndex]!='/') endIndex++;
                    int textureIndex1 = atoi(line.substr(startIndex, endIndex-startIndex).c_str());
                    
                    // first normal
                    startIndex = ++endIndex;
                    while(line[endIndex]!=' ') endIndex++;
                    int normalIndex1  = atoi(line.substr(startIndex, endIndex-startIndex).c_str()); // afterSlash
                    
                    
                    // second vector
                    startIndex = ++endIndex;
                    while(line[startIndex]==' ') startIndex++;
                    endIndex = startIndex;
                    while(line[endIndex]!='/') endIndex++;
                    int vectorIndex2  = atoi(line.substr(startIndex, endIndex-startIndex).c_str());
                    
                    // second texture
                    startIndex = ++endIndex;
                    while(line[endIndex]!='/') endIndex++;
                    int textureIndex2 = atoi(line.substr(startIndex, endIndex-startIndex).c_str());
                    
                    // second normal
                    startIndex = ++endIndex;
                    while(line[endIndex]!=' ') endIndex++;
                    int normalIndex2  = atoi(line.substr(startIndex, endIndex-startIndex).c_str());
                    
                    // third vector
                    startIndex = ++endIndex;
                    while(line[startIndex]==' ') startIndex++;
                    endIndex = startIndex;
                    while(line[endIndex]!='/') endIndex++;
                    int vectorIndex3  = atoi(line.substr(startIndex, endIndex-startIndex).c_str());
                    
                    // third texture
                    startIndex = ++endIndex;
                    while(line[endIndex]!='/') endIndex++;
                    int textureIndex3 = atoi(line.substr(startIndex, endIndex-startIndex).c_str());
                    
                    // third normal
                    startIndex = ++endIndex;
                    int normalIndex3  = atoi(line.substr(startIndex, line.length()-startIndex).c_str());
                    
                    
                    ATVector2D texture1 = textures[textureIndex1-1];
                    ATVector2D texture2 = textures[textureIndex2-1];
                    ATVector2D texture3 = textures[textureIndex3-1];
                    
                    ATVector3D normal1 = normals[normalIndex1-1];
                    ATVector3D normal2 = normals[normalIndex2-1];
                    ATVector3D normal3 = normals[normalIndex3-1];
                    
                    ATVector4D vector1 = vectors[vectorIndex1-1];
                    ATVector4D vector2 = vectors[vectorIndex2-1];
                    ATVector4D vector3 = vectors[vectorIndex3-1];
                    
                    *totalX+=vector1.getX();
                    *totalY+=vector1.getY();
                    *totalX+=vector2.getX();
                    *totalY+=vector2.getY();
                    *totalX+=vector3.getX();
                    *totalY+=vector3.getY();
                    *numVals+=3;
                    
                    ATTriangle att(vector1, vector2, vector3, normal1, normal2, normal3,  
                                   texture1, texture2, texture3);
                    
                    faces.triangles.push_back(att);
                }
                else
                {
                    // ignore
                }
            }
        }
        objFile.close();
    }
    else
    {
        cerr << filename << " does not exist!" << endl;
    }
	return faces;
}




/**
 * Reads in scene file and sets it up for phong lighting.
 */
void ATScene::sceneReaderPhong(char* filename)
{
	ifstream file(filename);
	printf("Reading in file %s\n", filename);
	
	string line;  // line to be read in from the scene file
	
	string gFile; // RAW file containing triangle data
	
    
	int numVals = 0;
	
	float totalX=0.0f, totalY=0.0f;
    
	float eyeX, eyeY, eyeZ;
	float spotX, spotY, spotZ;
	float upX, upY, upZ;
	float fov, aspectRatio, nearDist, farDist;
    
    string* textureMap = new string();
	ATTriangleGroup group(true);
	int* textured = new int(0);
    
	ATColor materialColor(1.0f, 1.0f, 1.0f, 1.0f);
	ATColor specularColor(1.0f, 0.0f, 0.0f, 0.0f);
	float alpha = 0.0f;
	ambientColor = ATColor(1.0f, 1.0f, 1.0f, 1.0f);
    
	camera = ATMatrix4::MkIdentityMatrix();
	ATMatrix4 tempCamera = ATMatrix4::MkIdentityMatrix();
	ATVector4D eyePt(0.0f, 0.0f, 0.0f, 0.0f);
    
    
	if (file.is_open())
	{
		// U3
		getline(file, line);  
		if(line.compare("U4")!=0)
		{
			cerr << "Incompatible file type!" << endl;
			
		}
        
		// Specifies the pixel width, pixel height
		getline(file, line);
		string widthHeight[2];
		splitString(line, widthHeight, 2);
		width = atoi(widthHeight[0].c_str()); // pixel width
		height = atoi(widthHeight[1].c_str()); //pixel height
        
		// eye coordinates
		getline(file, line);
		string eyeCoord[3];
		splitString(line, eyeCoord, 3);
		eyeX = atof(eyeCoord[0].c_str());
		eyeY = atof(eyeCoord[1].c_str());
		eyeZ = atof(eyeCoord[2].c_str());
		eyePt = ATVector4D::mkATVector4D(eyeX, eyeY, eyeZ, 1.0f);
        
		// spot coordinates
		getline(file, line);
		string spotCoord[3];
		splitString(line, spotCoord, 3);
		spotX = atof(spotCoord[0].c_str());
		spotY = atof(spotCoord[1].c_str());
		spotZ = atof(spotCoord[2].c_str());
		ATVector4D spotPt(spotX, spotY, spotZ, 1.0f);
		
		// up vector coordinates
		getline(file, line);
		string upCoord[3];
		splitString(line, upCoord, 3);
		upX = atof(upCoord[0].c_str());
		upY = atof(upCoord[1].c_str());
		upZ = atof(upCoord[2].c_str());
		ATVector4D upVec(upX, upY, upZ, 1.0f);
        
		// create the frustum from fov, aspectRatio, nearDist, farDist
		getline(file, line);
		string frustum[4];
		splitString(line, frustum, 4);
		fov = atof(frustum[0].c_str());
		aspectRatio = atof(frustum[1].c_str()); // width to height of near plane
		nearDist = atof(frustum[2].c_str());
		farDist = atof(frustum[3].c_str());
		
        
        
		// CREATE THE CAMERA MATRIX
		camera = ATMatrix4::MkLookAt(eyePt, spotPt, upVec);
        
		getline(file, line);
		string vals [3];
		splitString(line, vals, 3);
		
		float r = atof(vals[0].c_str());
		float g = atof(vals[1].c_str());
		float b = atof(vals[2].c_str());
        
		// create ambient light
		ambientColor.setColor(1.0f, r, g, b);
        
		// PASS IN A COPY OF THE CAMERA MATRIX
		tempCamera = camera;
		
        
		// creates the array of transformations to do on the triangles in the raw file
		while(file.good())
		{
			getline(file, line);
			if(!line.empty())
			{
				if(line[0]=='g')
				{
					if(!gFile.empty())
					{
                        
                        
                        
                        
                        
                        
                        
                        /* DO THIS IN OPEN GL*/
                        
                        //                        int culledtriangles = 0;
                        
                        //                        // TAKE THE CAMERA MATRIX AND MULTIPLY BY ALL TRIANGLES IN THE G FILE
                        //                        for (unsigned int tri = 0; tri < group.triangles.size(); tri++)
                        //                        {	
                        //                            // Multiplies the camera matrix by each vertex in the triangle
                        //                            group.triangles[tri].vertA = ATMatrix4::MultiplyMatrix(tempCamera, group.triangles[tri].vertA);
                        //                            group.triangles[tri].vertB = ATMatrix4::MultiplyMatrix(tempCamera, group.triangles[tri].vertB);
                        //                            group.triangles[tri].vertC = ATMatrix4::MultiplyMatrix(tempCamera, group.triangles[tri].vertC);
                        //                            
                        //                            // Transform the normals as well
                        //                            group.triangles[tri].normA = ATMatrix4::MultiplyMatrix3D(tempCamera, group.triangles[tri].normA);
                        //                            group.triangles[tri].normB = ATMatrix4::MultiplyMatrix3D(tempCamera, group.triangles[tri].normB);
                        //                            group.triangles[tri].normC = ATMatrix4::MultiplyMatrix3D(tempCamera, group.triangles[tri].normC);
                        //                            
                        //                            // Do some culling
                        //                            ATVector4D triangleNormal = ATVector4D::crossProduct(
                        //                                                                                 ATVector4D::subtractTwoVectors(group.triangles[tri].vertB,  group.triangles[tri].vertA),
                        //                                                                                 ATVector4D::subtractTwoVectors(group.triangles[tri].vertC,  group.triangles[tri].vertA));
                        //                            
                        //                            if(ATVector4D::dot(triangleNormal, group.triangles[tri].vertA) <= 0.0f)
                        //                            {
                        //                                group.triangles[tri].phong = true;
                        //                                group.triangles[tri].eyeA = group.triangles[tri].vertA;
                        //                                group.triangles[tri].eyeB = group.triangles[tri].vertB;
                        //                                group.triangles[tri].eyeC = group.triangles[tri].vertC;
                        //                                
                        //                            }
                        //                            else
                        //                            {
                        //                                culledtriangles++;
                        //                            }
                        //                            
                        //                        }
                        
                        group.modelView = tempCamera;
                        group.alpha = materialColor.getalphaF();
                        group.materialColor = materialColor;
                        group.specularColor = specularColor;
                        group.shininess = alpha;
                        if((*textured)==1) group.textured = true;
                        else group.textured = false;
                        if (group.textured)
                            group.textureMap = textureMap;
                        this->addObjectToScene(group);
                        
                        
                        
                        
                        
                        
                        
                        
                        // reset everything in the group after adding the group to the scene
                        
                        
                        
                        
						// CREATE NEW CAMERA THAT COPIES THE ORIGINAL ONE
						tempCamera = camera;
                        
						// Flush out diffuse color
						materialColor.setColor(1.0f, 1.0f, 1.0f, 1.0f);
                        
						// flush out specular color
						specularColor.setColor(1.0f, 0.0f, 0.0f, 0.0f);
						group.alpha = 1.0f;
						alpha = 0.0f;
						(*textured) = 0;
					}
                    
					// SETS THE NEW G FILE
					string retVals[2];
					splitString(line, retVals, 2);
					gFile = retVals[1];
					group.clear();
					
					string gfi = gFile.substr(gFile.length()-4, 3);
					if(gFile.substr(gFile.length()-3, 3)=="raw")
					{
						cout << "reading raw file" << endl;
						// GET ALL TRIANGLES FROM THE G FILE
                        char gname[100];
                        strcpy(gname, FILEPREFIX);
                        strcat(gname, gFile.c_str());
                        ifstream file(gname);
						group = readRawFile(gname, &totalX, &totalY, &numVals);
                        
					}
					else if(gFile.substr(gFile.length()-3, 3)=="obj")
					{
						
                        char gname[100];
                        strcpy(gname, FILEPREFIX);
                        strcat(gname, gFile.c_str());
                        ifstream file(gname);
                        cout << "reading obj file" << gname << endl;
						group = ObjReader(gname, &totalX, &totalY, &numVals);
                        
					}
					else
					{
						cerr << "File is neither raw nor obj" << endl;
					}
				}
				else if(line[0]=='l')
				{
					// diffuse lights in world coords
					string lightVals[7];
					splitString(line, lightVals, 7);
					float x = atof(lightVals[1].c_str());
					float y = atof(lightVals[2].c_str());
					float z = atof(lightVals[3].c_str());
                    
					float r = atof(lightVals[4].c_str());
					float g = atof(lightVals[5].c_str());
					float b = atof(lightVals[6].c_str());
                    
					ATLight newLight(ATVector4D(x, y, z, 1.0f), ATColor(1.0f, r, g, b));
					this->addLightToScene(newLight);
				}
				else
				{
					int* text = new int(0);
					tempCamera = createTransformationMatrix(text, textureMap, tempCamera, line, &materialColor, &specularColor, &alpha);
					if((*text) == 1) (*textured) = 1;
					delete text;
                    
				} //end else
			} //end if line is empty
		}//end while
	} // end if file is open
	else
	{
		cerr << "File not found!";
		cin.get();
		return;
	}
    
    
    
    
	int culledtriangles = 0;
    
    /* DO THIS IN OPEN GL*/
	
    //	// TAKE THE CAMERA MATRIX AND MULTIPLY BY ALL TRIANGLES IN THE G FILE
    //	for (unsigned int tri = 0; tri < group.triangles.size(); tri++)
    //	{	
    //		// Multiplies the modelview matrix by each vertex in the triangle
    //		group.triangles[tri].vertA = ATMatrix4::MultiplyMatrix(tempCamera, group.triangles[tri].vertA);
    //		group.triangles[tri].vertB = ATMatrix4::MultiplyMatrix(tempCamera, group.triangles[tri].vertB);
    //		group.triangles[tri].vertC = ATMatrix4::MultiplyMatrix(tempCamera, group.triangles[tri].vertC);
    //        
    //		// Transform the normals as well
    //		group.triangles[tri].normA = ATMatrix4::MultiplyMatrix3D(tempCamera, group.triangles[tri].normA);
    //		group.triangles[tri].normB = ATMatrix4::MultiplyMatrix3D(tempCamera, group.triangles[tri].normB);
    //		group.triangles[tri].normC = ATMatrix4::MultiplyMatrix3D(tempCamera, group.triangles[tri].normC);
    //        
    //		// Do some culling
    //		ATVector4D triangleNormal = ATVector4D::crossProduct(
    //                                                             ATVector4D::subtractTwoVectors(group.triangles[tri].vertB,  group.triangles[tri].vertA),
    //                                                             ATVector4D::subtractTwoVectors(group.triangles[tri].vertC,  group.triangles[tri].vertA));
    //        
    //		if(ATVector4D::dot(triangleNormal, group.triangles[tri].vertA) <= 0.0f)
    //		{
    //			group.triangles[tri].phong = true;
    //			group.triangles[tri].eyeA = group.triangles[tri].vertA;
    //			group.triangles[tri].eyeB = group.triangles[tri].vertB;
    //			group.triangles[tri].eyeC = group.triangles[tri].vertC;
    //            
    //		}
    //		else
    //		{
    //			culledtriangles++;
    //		}
    //        
    //	}
    
    group.modelView = tempCamera;
	group.alpha = materialColor.getalphaF();
	group.materialColor = materialColor;
	group.specularColor = specularColor;
	group.shininess = alpha;
	if((*textured)==1) group.textured = true;
	else group.textured = false;
	if (group.textured)
		group.textureMap = textureMap;
	this->addObjectToScene(group);
    
	cout << "culled triangles: " << culledtriangles << endl;
    
    
    
    
	delete textured;
    
    
    
    /* DO THIS IN OPEN GL*/
    
    //	// for each light, transform the light into eye coordinates
    //	for(unsigned int lightIndex = 0; lightIndex < this->lights.size(); lightIndex++)
    //	{
    //		// transform light into eye coords
    //		ATVector4D lpos = ATMatrix4::MultiplyMatrix(camera, this->lights[lightIndex].getPosition());
    //		lights[lightIndex].setPosition(lpos.getX(), lpos.getY(), lpos.getZ());
    //	}
    
	// transform all triangles by the perspective, viewport matrices
	perspective = ATMatrix4::MkPerspective(fov, aspectRatio, nearDist, farDist);
    viewport = ATMatrix4::MkViewport(0, 0, width, height);
    
    
    
    /* DO THIS IN OPEN GL*/
    
    //	for(unsigned int a = 0; a < objectsInScene.size(); a++)
    //	{
    //		for(unsigned int b=0; b < objectsInScene[a].triangles.size(); b++)
    //		{
    //			ATColor vertAColor = objectsInScene[a].triangles[b].vertA.getColor();
    //			ATColor vertBColor = objectsInScene[a].triangles[b].vertB.getColor();
    //			ATColor vertCColor = objectsInScene[a].triangles[b].vertC.getColor();
    //			objectsInScene[a].triangles[b].vertA = ATMatrix4::MultiplyMatrixZ0(perspective, objectsInScene[a].triangles[b].vertA );
    //			objectsInScene[a].triangles[b].vertB = ATMatrix4::MultiplyMatrixZ0(perspective, objectsInScene[a].triangles[b].vertB );
    //			objectsInScene[a].triangles[b].vertC = ATMatrix4::MultiplyMatrixZ0(perspective, objectsInScene[a].triangles[b].vertC );
    //            
    //			objectsInScene[a].triangles[b].vertA.scaleVectorNotZ(1/objectsInScene[a].triangles[b].vertA.getZ());
    //			objectsInScene[a].triangles[b].vertB.scaleVectorNotZ(1/objectsInScene[a].triangles[b].vertB.getZ());
    //			objectsInScene[a].triangles[b].vertC.scaleVectorNotZ(1/objectsInScene[a].triangles[b].vertC.getZ());
    //            
    //			objectsInScene[a].triangles[b].vertA = ATMatrix4::MultiplyMatrix(viewport, objectsInScene[a].triangles[b].vertA );
    //			objectsInScene[a].triangles[b].vertB = ATMatrix4::MultiplyMatrix(viewport, objectsInScene[a].triangles[b].vertB );
    //			objectsInScene[a].triangles[b].vertC = ATMatrix4::MultiplyMatrix(viewport, objectsInScene[a].triangles[b].vertC );
    //            
    //			objectsInScene[a].triangles[b].vertA.setColor(vertAColor);
    //			objectsInScene[a].triangles[b].vertB.setColor(vertBColor);
    //			objectsInScene[a].triangles[b].vertC.setColor(vertCColor);
    //		}
    //	}
    
    
	cout << "writing to raster" << endl;
    
    
    /* DO THIS IN OPEN GL*/
    
    //	// create the raster
    //	ATRaster3D raster(width, height);
    //	//rasterize scene
    //	ATRaster3D::PhongInterpolateScene(&raster, (const ATScene*) this, ambientColor);
    //	// save to ppm and bmp
    //	ATRaster3D::ATSaveToPPM(raster.getPixels(), raster.getWidth(), raster.getHeight(), (char*)"scene.ppm");
    //	ATRaster3D::ATSaveToBMP(raster.getPixels(), raster.getWidth(), raster.getHeight(), (char*)"scene.bmp");
    
    
	cout << "Excellent work my friend!\nData has been saved and written to scene.ppm and scene.bmp!\n";
    
}