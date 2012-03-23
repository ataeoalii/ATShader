#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include "ATColor.h"
#include "ATRaster3D.h"
#include <math.h>
#include <float.h>
#include "ATTriangleGroup.h"

using namespace std;

ATRaster3D::ATRaster3D(int _width, int _height)
{
	height = _height;
	width = _width;
	rasterPixels = new ATColor[height*width];
	_size = height*width;
	depths = new float[height*width];
	for(int i=0; i < height*width; i++)
	{
		depths[i] = -FLT_MAX;
	}

}

ATRaster3D::ATRaster3D(const ATRaster3D &copy)
{
	height = copy.height;
	width = copy.width;
	_size = copy._size;

	rasterPixels = new ATColor[height*width];
	depths = new float[height*width];

	for(unsigned int i=0; i<height*width; i++)
	{
		rasterPixels[i] = copy.rasterPixels[i];
		depths[i] = copy.depths[i];
	}
}

ATRaster3D::ATRaster3D()
{
	height = 500;
	width = 500;
	rasterPixels = new ATColor[height*width];
	depths = new float[height*width];
	_size = height*width;

	for(int i=0; i < height*width; i++)
	{
		depths[i] = -FLT_MAX;
	}
}

ATRaster3D::~ATRaster3D()
{

}

/**
* Rasterizes a triangle and fills it with the specified color.
*
*/
void ATRaster3D::ATRasterizeTriangle(ATRaster3D raster, ATTriangle tri, ATColor fillColor)
{
	for (int h = 0; h < raster.getHeight(); h++)
	{
		for (int w = 0; w < raster.getWidth(); w++)
		{
			// Calculate barycentric coordinates 
			float lambda1 = 0.0f;
			float lambda2 = 0.0f;
			float lambda3 = 0.0f;
			ATVector4D vect((float)w, (float)h, 1.0f);

			// flatten triangle (just x and y to draw on the screen)

			ATRaster3D::GetBarycentricCoordinates(vect, tri.vertA, tri.vertB, tri.vertC, &lambda1, &lambda2, &lambda3);
		
			if(lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0)
				raster.setPixel(((raster.getHeight()-h)*raster.getWidth() + w), fillColor);
		}
	}
}

/**
* Rasterizes a triangle and fills it with the interpolation of the colors specified at each
* vertex in the triangle.
*
*/
void ATRaster3D::ATRasterizeInterpolatedTriangle(ATRaster3D raster, ATTriangle tri, int minX, int maxX, int minY, int maxY, float alpha)
{
	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			// Calculate barycentric coordinates 
			float lambda1 = 0.0f;
			float lambda2 = 0.0f;
			float lambda3 = 0.0f;
			ATVector4D vect((float)x, (float)y, 1.0f);
			ATRaster3D::GetBarycentricCoordinates(vect, tri.vertA, tri.vertB, tri.vertC, &lambda1, &lambda2, &lambda3);


			if(lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0)
			{
				float zVal = tri.vertA.getZ() * lambda1 +
					tri.vertB.getZ() *lambda2 + 
					tri.vertC.getZ() * lambda3;

				if(y*raster.getWidth() - x >= 0)
				{
					float zBuffer = raster.getDepth(y*raster.getWidth() - x);

					// Z-Buffer and Clip
					if(zVal > zBuffer && zVal < -0.01f)
					{
						raster.setDepth(y*raster.getWidth() - x, zVal);
						ATColor aContr = tri.vertA.getColor();
						aContr.scaleColor(lambda1);

						ATColor bContr = tri.vertB.getColor();
						bContr.scaleColor(lambda2);

						ATColor cContr = tri.vertC.getColor();
						cContr.scaleColor(lambda3);


						ATColor temp = ATColor::addTwoColors(aContr, cContr);
						ATColor fillColor = ATColor::addTwoColors(bContr, temp);
						raster.blendPixel(y*raster.getWidth() - x, fillColor, alpha);
					}

				}
			}
		}
	}
}

/**
* Sets the color of a specific pixel
*/
void ATRaster3D::setPixel(int index, ATColor color)
{
	if(index < _size)
	{
		rasterPixels[index] = color;
	}
}

/**
* Rasterizes a group of interpolated triangles. This method calculates the min and max
* x and y values in the raster so it does not rasterize the entire screen, but only the 
* portions necessary for the triangles.
*/
void ATRaster3D::ATRasterizeInterpolatedTriangles(ATRaster3D raster, ATTriangleGroup triangles, float alpha)
{
	int minX, maxX, minY, maxY;
	for(unsigned long i=triangles.triangles.size(); i >= 1; i--)
	{
		if(triangles.triangles[i-1].vertA.getX() > triangles.triangles[i-1].vertB.getX())
		{
			minX = (int)triangles.triangles[i-1].vertB.getX();
			maxX = (int)triangles.triangles[i-1].vertA.getX();
		}
		else
		{
			minX = (int)triangles.triangles[i-1].vertA.getX();
			maxX = (int)triangles.triangles[i-1].vertB.getX();
		}

		if(triangles.triangles[i-1].vertC.getX() > maxX)
		{
			maxX = (int)triangles.triangles[i-1].vertC.getX();
		}

		if(triangles.triangles[i-1].vertC.getX() < minX)
		{
			minX = (int)triangles.triangles[i-1].vertC.getX();
		}

		if(triangles.triangles[i-1].vertA.getY() > triangles.triangles[i-1].vertB.getY())
		{
			minY = (int)triangles.triangles[i-1].vertB.getY();
			maxY = (int)triangles.triangles[i-1].vertA.getY();
		}
		else
		{
			minY = (int)triangles.triangles[i-1].vertA.getY();
			maxY = (int)triangles.triangles[i-1].vertB.getY();
		}

		if(triangles.triangles[i-1].vertC.getY() > maxY)
		{
			maxY = (int)triangles.triangles[i-1].vertC.getY();
		}

		if(triangles.triangles[i-1].vertC.getY() < minY)
		{
			minY = (int)triangles.triangles[i-1].vertC.getY();
		}

		if(maxX >= raster.getWidth())
		{
			maxX = raster.getWidth()-1;
		}

		if(maxY >= raster.getHeight())
		{
			maxY = raster.getHeight()-1;
		}

		if(minX <=0)
		{
			minX = 0;
		}

		if(minY <= 0)
		{
			minY = 0;
		}

		if(minX < maxX && minY < maxY)
			ATRaster3D::ATRasterizeInterpolatedTriangle(raster, triangles.triangles[i-1], minX, maxX+1, minY, maxY+1, alpha);
	}
}

/**
* Gets the height of the raster.
*/ 
int ATRaster3D::getHeight()
{
	return height;
}

/**
* Returns the width of the raster.
*
*/
int ATRaster3D::getWidth()
{
	return width;
}


/**
* Gets the barycentric coordinates (using lambda1,2, and 3 as pointers to floats).
*/
void ATRaster3D::GetBarycentricCoordinates(ATVector4D vect, ATVector4D atv1, ATVector4D atv2, ATVector4D atv3, float* lambda1, float* lambda2, float* lambda3)
{
	float lambdaA = ((atv2.getY() - atv3.getY())*(vect.getX() - atv3.getX()) + (atv3.getX() - atv2.getX())*(vect.getY() - atv3.getY())) / 
		((atv2.getY() - atv3.getY())*(atv1.getX() - atv3.getX()) + ((atv3.getX() - atv2.getX()) * (atv1.getY() - atv3.getY())));

	float lambdaB = ((atv3.getY() - atv1.getY())*(vect.getX() - atv3.getX()) + (atv1.getX() - atv3.getX())*(vect.getY() - atv3.getY())) / 
		((atv2.getY() - atv3.getY())*(atv1.getX() - atv3.getX()) + ((atv3.getX() - atv2.getX()) * (atv1.getY() - atv3.getY())));

	float lambdaC = 1.0f - lambdaA - lambdaB;

	if (lambdaA != NULL)
	{
		*lambda1 = lambdaA;
	}
	if (lambdaB != NULL)
	{
		*lambda2 = lambdaB;
	}
	if (lambdaC != NULL)
	{
		*lambda3 = lambdaC;
	}
}

/**
* Returns the array of pixels that make up the raster.
*/
ATColor* ATRaster3D::getPixels()
{
	return rasterPixels;
}

float* ATRaster3D::getDepths()
{
	return depths;
}

float ATRaster3D::getDepth(unsigned int index)
{
 	return depths[index];
}
	
ATColor ATRaster3D::getPixel(int index)
{
	return rasterPixels[index];
}
	
void ATRaster3D::setDepth(unsigned int index, float value)
{
	depths[index] = value;
}

void ATRaster3D::blendPixel(int index, ATColor color, float alpha)
{
	ATColor thisColor = rasterPixels[index];
	color.scaleColor(alpha);
	thisColor.scaleColor(1.0f - alpha);
	ATColor finColor = ATColor::addTwoColors(color, thisColor);
	setPixel(index, finColor);
}


void ATRaster3D::ATSaveToPPM(ATColor pixels[], int width, int height, char* path)
{
    // Open a file for writing
    FILE* f = fopen(path, "w");
    if (f == NULL)
    {
        printf("Couldn't open file %s for writing", path);
        return;
    }
    
    // Write out information required by plain PPM header definition
    fprintf(f, "P3\n");
    fprintf(f, "%i %i\n", width, height);
    fprintf(f, "%i\n", 255);
    
    // Write out pixel information
    
	int index = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            ATColor color = pixels[index];
			fprintf(f, "%i ", color.getredB());
            fprintf(f, "%i ", color.getgreenB());
			fprintf(f, "%i ", color.getblueB());
			index++;
        }
        fprintf(f, "\n");
    }
}


void ATRaster3D::ATSaveToBMP(ATColor rasterPixels[], int rasterWidth, int rasterHeight, char* path)
{
    //BMP (Windows V3)
    //Offset    Size    Description
    //0         2       the magic number used to identify the BMP file: 0x42 0x4D (Hex code points for B and M in big-endian order)
    //2         4       the size of the BMP file in bytes
    //6         2       reserved; actual value depends on the application that creates the image
    //8         2       reserved; actual value depends on the application that creates the image
    //10        4       the offset, i.e. starting address, of the byte where the bitmap data can be found.
    //14        4       the size of this header (40 bytes)
    //18        4       the bitmap width in pixels (signed integer).
    //22        4       the bitmap height in pixels (signed integer).
    //26        2       the number of color planes being used. Must be set to 1.
    //28        2       the number of bits per pixel, which is the color samplesPerPixel of the image. Typical values are 1, 4, 8, 16, 24 and 32.
    //30        4       the compression method being used. See the next table for a list of possible values.
    //34        4       the image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.
    //38        4       the horizontal resolution of the image. (pixel per meter, signed integer)
    //42        4       the vertical resolution of the image. (pixel per meter, signed integer)
    //46        4       the number of colors in the color palette, or 0 to default to 2n.
    //50        4       the number of important colors used, or 0 when every color is important; generally ignored.

    //Open file for writing
    FILE* file = fopen(path, "wb");
    if (file == NULL)
        return;
    
    //Define header data
    uint16_t magicNumber = 0x4D42;
    uint16_t reserved0 = 0;//0x4D41;
    uint16_t reserved1 = 0;//0x5454;
    uint32_t dataOffset = 54;
    uint32_t infoHeaderSize = 40;
    uint32_t width = rasterWidth;
    uint32_t height = rasterHeight;
    uint16_t colorPlanes = 1;
    uint16_t bitsPerPixel = 32;
    uint32_t compression = 0;
    uint32_t dataSize = width * height * bitsPerPixel / 8;
    uint32_t horizontalResolution = 2835;
    uint32_t verticalResolution = 2835;
    uint32_t paletteColorCount = 0;
    uint32_t importantPaletteColorCount = 0;
    uint32_t fileSize = 54 + dataSize;
    
    //Write BMP header (Windows V3, 32bbp)
    fwrite(&magicNumber, sizeof(magicNumber), 1, file);
    fwrite(&fileSize, sizeof(fileSize), 1, file);
    fwrite(&reserved0, sizeof(reserved0), 1, file);
    fwrite(&reserved1, sizeof(reserved1), 1, file);
    fwrite(&dataOffset, sizeof(dataOffset), 1, file);
    fwrite(&infoHeaderSize, sizeof(infoHeaderSize), 1, file);
    fwrite(&width, sizeof(width), 1, file);
    fwrite(&height, sizeof(height), 1, file);
    fwrite(&colorPlanes, sizeof(colorPlanes), 1, file);
    fwrite(&bitsPerPixel, sizeof(bitsPerPixel), 1, file);
    fwrite(&compression, sizeof(compression), 1, file);
    fwrite(&dataSize, sizeof(dataSize), 1, file);
    fwrite(&horizontalResolution, sizeof(horizontalResolution), 1, file);
    fwrite(&verticalResolution, sizeof(verticalResolution), 1, file);
    fwrite(&paletteColorCount, sizeof(paletteColorCount), 1, file);
    fwrite(&importantPaletteColorCount, sizeof(importantPaletteColorCount), 1, file);
    
    //Write BMP body (XXRRGGBB)
    for (int y = rasterHeight - 1; y >= 0; y--)
    {
        for (int x = 0; x < rasterWidth; x++)
        {
			uint32_t color = rasterPixels[y*rasterWidth + x].getColor();

            fwrite(&color, 1, sizeof(color), file);
        }
    }
    
    //Cleanup
    fclose(file);
}


 void ATRaster3D::PhongInterpolateScene(ATRaster3D* raster, const ATScene* scene, ATColor ambientColor)
 {
	 for(unsigned int j=0; j<scene->getObjects().size(); j++)
	 {

		 ATTriangleGroup atg = scene->getObjects().at(j);


		 for (unsigned int i=0; i< atg.triangles.size(); i++)
		 {
		 	int minX = INT_MAX;
			int minY = INT_MAX;
			int maxX = -INT_MAX;
			int maxY = -INT_MAX;

			if(atg.triangles[i].vertA.getX() > atg.triangles[i].vertB.getX())
			{
				minX = (int)atg.triangles[i].vertB.getX();
				maxX = (int)atg.triangles[i].vertA.getX();
			}
			else
			{
				minX = (int)atg.triangles[i].vertA.getX();
				maxX = (int)atg.triangles[i].vertB.getX();
			}

			if(atg.triangles[i].vertC.getX() > maxX)
			{
				maxX = (int)atg.triangles[i].vertC.getX();
			}

			if(atg.triangles[i].vertC.getX() < minX)
			{
				minX = (int)atg.triangles[i].vertC.getX();
			}

			if(atg.triangles[i].vertA.getY() > atg.triangles[i].vertB.getY())
			{
				minY = (int)atg.triangles[i].vertB.getY();
				maxY = (int)atg.triangles[i].vertA.getY();
			}
			else
			{
				minY = (int)atg.triangles[i].vertA.getY();
				maxY = (int)atg.triangles[i].vertB.getY();
			}

			if(atg.triangles[i].vertC.getY() > maxY)
			{
				maxY = (int)atg.triangles[i].vertC.getY();
			}

			if(atg.triangles[i].vertC.getY() < minY)
			{
				minY = (int)atg.triangles[i].vertC.getY();
			}

			if(maxX >= raster->getWidth())
			{
				maxX = raster->getWidth()-1;
			}

			if(maxY >= raster->getHeight())
			{
				maxY = raster->getHeight()-1;
			}

			if(minX <=0)
			{
				minX = 0;
			}

			if(minY <= 0)
			{
				minY = 0;
			}

			if(minX < maxX && minY < maxY && atg.triangles[i].phong)
			{
				ATRaster3D::PhongInterpolateTriangle((&atg.textureRaster), atg.textured, raster, ambientColor, atg.materialColor, atg.specularColor,
					scene->getLights(), atg.triangles[i], minX, maxX+1, minY, maxY+1, atg.alpha, atg.shininess);
			}
		}
	 }
 }

 void ATRaster3D::PhongInterpolateTriangle(ATTextureMap* textureRaster, bool textured, ATRaster3D* raster, ATColor ambientColor,
	 ATColor materialColor, ATColor specularColor, const vector<ATLight> &lights, ATTriangle tri, int minX, int maxX, int minY, 
	 int maxY, float alpha, float shininess)
 {
	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			// Calculate barycentric coordinates 
			float lambda1 = 0.0f;
			float lambda2 = 0.0f;
			float lambda3 = 0.0f;
			ATVector4D vect((float)x, (float)y, 1.0f);
			ATRaster3D::GetBarycentricCoordinates(vect, tri.vertA, tri.vertB, tri.vertC, &lambda1, &lambda2, &lambda3);


			if(lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0)
			{
				float zVal = tri.vertA.getZ() * lambda1 +
					tri.vertB.getZ() *lambda2 + 
					tri.vertC.getZ() * lambda3;

				if(y*(raster->getWidth()) - x >= 0)
				{
					float zBuffer = raster->getDepth(y*(raster->getWidth()) - x);

					// Z-Buffer and Clip
					if(zVal > zBuffer && zVal < -0.01f)
					{
						raster->setDepth(y*(raster->getWidth()) - x, zVal);
						ATVector3D aContr = tri.normA;
						aContr.scaleVector(lambda1);

						ATVector3D bContr = tri.normB;
						bContr.scaleVector(lambda2);

						ATVector3D cContr = tri.normC;
						cContr.scaleVector(lambda3);

						// use barycentric coords to find where the point is in eye coords
						ATVector4D point(lambda1*tri.eyeA.getX() + lambda2*tri.eyeB.getX() + lambda3*tri.eyeC.getX(),
										 lambda1*tri.eyeA.getY() + lambda2*tri.eyeB.getY() + lambda3*tri.eyeC.getY(),
										 lambda1*tri.eyeA.getZ() + lambda2*tri.eyeB.getZ() + lambda3*tri.eyeC.getZ(), 1.0f );

						ATColor ambColor;

						if(textured)
						{
							int dim = textureRaster->getHeight() * textureRaster->getWidth() / 6;
							ATVector2D textIndex(lambda1*tri.textA.getX() + lambda2*tri.textB.getX() + lambda3*tri.textC.getX(),
								lambda1*tri.textA.getX() + lambda2*tri.textB.getY() + lambda3*tri.textC.getY());


							//int index = (int)(textIndex.getY()*(textureRaster->getWidth()) - textIndex.getX());
							int index = 8000 + (int)(((y*(raster->getWidth()) - x)*dim) / (point.getZ() * raster->getHeight()*raster->getWidth()));
							materialColor = textureRaster->getPixel(index);
							ambColor = ATColor::multiply(ambientColor, materialColor);
						}
						else
						{
							ambColor = ATColor::multiply(ambientColor, materialColor);
						}
						ATVector3D normal = ATVector3D::addTwoVectors(aContr, ATVector3D::addTwoVectors(bContr, cContr));
						ATColor fillColor = ATRaster3D::calculateColor(ambColor, materialColor, specularColor, lights, normal, point, shininess);

						raster->blendPixel(y*(raster->getWidth()) - x, fillColor, alpha);
						
					}
				}
			}
		}
	}
 }


ATColor ATRaster3D::calculateColor(ATColor ambientColor, ATColor materialColor, ATColor specularColor, 
	const vector<ATLight> &theLights, ATVector3D normal, ATVector4D point, float shininess)
{

/************************************** calculate diffuse **********************************************************/
			// initialize diffuse color
			ATColor diffuseColorA(1.0f, 0.0f, 0.0f, 0.0f);

			// initialize specular color
			ATColor specularColorA(1.0f, 0.0f, 0.0f, 0.0f);


			// for each light, add a little color
			for(unsigned int lightIndex = 0; lightIndex < theLights.size(); lightIndex++)
			{
				ATLight light = theLights[lightIndex];

				// diffuse = Ld * kd scale(l dot n)
				ATVector4D lightA = ATVector4D::normalize(ATVector4D::subtractTwoVectors(light.getPosition(), point));
				
				// load n from file
				ATVector4D normA(normal.getX(), normal.getY(), normal.getZ(), 0.0f);

				// for diffuse lighting
				float aContrib = ATVector4D::dot(ATVector4D::normalize(lightA), ATVector4D::normalize(normA));

				// "clip" lighting (don't want negative light)
				if (aContrib < 0) aContrib = 0.0f;

				ATColor aContribColor = materialColor;
				aContribColor.scaleColor(aContrib);
				aContribColor = ATColor::multiply(light.getColor(), aContribColor);
				
				//add in previous diffuse colors from other lights
				diffuseColorA = ATColor::addTwoColors(aContribColor, diffuseColorA);


/************************************** calculate specular **********************************************************/
				// specular = Ls * ks * (norm(v) dot norm(r)) ^ alpha
				// v = V - P
				// l = L - P
				// r = 2 * (l dot n) * n - 1

				light.setPosition(light.getPosition().getX(), light.getPosition().getY(), light.getPosition().getZ());

				// diffuse = Ld * kd scale(l dot n)
				lightA = ATVector4D::normalize(ATVector4D::subtractTwoVectors(light.getPosition(), point));

				// calculate v = V - P
				ATVector4D vvecA = ATVector4D::subtractTwoVectors(ATVector4D(0.0f, 0.0f, 0.0f, 1.0f), point);

				// 2* l dot n
				float scaleA = 2.0f * (ATVector4D::dot(lightA, ATVector4D::normalize(normA)));

				ATVector4D nA = ATVector4D::normalize(normA);

				if(scaleA < 0) scaleA=0.0f;

				nA.scaleVector(scaleA);

				// calculate r = 2 * l dot n * n - l
				ATVector4D rvecA = ATVector4D::subtractTwoVectors(nA, lightA);
				
				// (norm(v) dot norm(r)) ^ alpha

				float ndotrA = ATVector4D::dot(ATVector4D::normalize(rvecA), ATVector4D::normalize(vvecA));

				float specA = pow(ndotrA, shininess);

				if(specA<0) specA = 0.0f;

				ATColor aSpecContrib = ATColor::multiply(aSpecContrib, light.getColor());
				aSpecContrib.scaleColor(specA);


				// add in previous specular calculations
				specularColorA = ATColor::addTwoColors(aSpecContrib, specularColorA);

				/***********************************************************************************************************************************/

			}

			if(shininess<=0.1)
			{
				specularColorA = ATColor(1.0f, 0.0f, 0.0f, 0.0f);
			}

			// finalColor = ambient + diffuse + specular
			ATColor final = ATColor::addTwoColors(diffuseColorA, ATColor::addTwoColors(specularColorA, ambientColor));

			return final;
 }


