//
//  main.cpp
//  ATShader
//
// This program creates a 3D scene with OpenGL with Phong
// Shading as well as 2D Textures in PPM format.
//
//  Created by Andrew Taeoalii on 3/9/12.
//  Copyright 2012 Apple Inc. All rights reserved.
//



#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <GLUT/glut.h>
#include <fstream>
#include "ATMatrix4.h"
#include "ATVector4D.h"
#include "ATScene.h"

const char* FPREFIX = "../../../../../";

using namespace std;

void IdleFunction();
void ApplicationDisplay();



GLuint program;
ATMatrix4 modelViewMatrix;
ATMatrix4 projectionMatrix;
ATScene scene;
GLuint textureID;


void readInPPMtexture(string filename, int* width, int* height, int* maxNumber, ATColor** texels);


/**
 * Assuming P3 format where RGB values are integers between 0 and maxNum.
 */
void readInPPMtexture(string filename, int* width, int* height, int* maxNumber, ATColor** texels)
{
    FILE* file;
    char fname[200];
    strcpy(fname, FPREFIX);
    strcat(fname, filename.c_str());
    file = fopen(fname, "r");
    
    char* magNumber;
    fscanf(file, "%s", &magNumber);
    fscanf(file, "%d %d", width, height);
    fscanf(file, "%d", maxNumber);
    
    (*texels) = new ATColor[(*width)*(*height)*3];
    uint8_t temp, tempr, tempg, tempb;
    int idx = 0;
    while (fscanf(file, "%d", &temp) != EOF)
    {
        if(idx%3==0)
        {
            tempr = temp;
        }
        else if(idx%3==1)
        {
            tempg = temp;
        }
        else
        {
            tempb = temp;
            (*texels)[idx/3] = ATColor(255, tempr, tempg, tempb);
        }
        idx++;
    }
    

    // terminate
    fclose (file);
    
}


int main (int argc, const char * argv[])
{
    
    cout << "\n\n" << endl;
    char* cwd = getcwd(NULL, 0);
    printf("Current Dir%s\n", cwd);
    char* filename = (char*)"../../../../../scene.u3d";
    
    // read in scene file
    scene.sceneReaderPhong(filename);
    
    ifstream vertexShaderFile("../../../../../ATShader/ATVertexShader.sdr");
	printf("Reading in file ATVertexShader.sdr\n");
    
    string vertStream((istreambuf_iterator<char>(vertexShaderFile)), istreambuf_iterator<char>());
    
    const GLchar* vertexShaderCode = (const GLchar*)vertStream.c_str();
    
    vertexShaderFile.close();
    
    ifstream fragmentShaderFile("../../../../../ATShader/ATFragmentShader.sdr");
	printf("Reading in file ATFragmentShader.sdr\n");
    
    string fragStream((istreambuf_iterator<char>(fragmentShaderFile)), istreambuf_iterator<char>());
    const GLchar* fragmentShaderCode = (const GLchar*)fragStream.c_str();
    
    glutInit(&argc, (char**)argv);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(scene.width, scene.height);
    glutCreateWindow("ATShader");

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    
    
    glutIdleFunc(IdleFunction);
    glutDisplayFunc(ApplicationDisplay);

    
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    
    glCompileShader(vertexShader);
    
    //check output
    GLint vertexCompileLogLength;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &vertexCompileLogLength);
    
    if(vertexCompileLogLength > 0)
    {
        GLchar* vertexCompileLog = (GLchar*)malloc(vertexCompileLogLength);
        glGetShaderInfoLog(vertexShader, vertexCompileLogLength, &vertexCompileLogLength, vertexCompileLog);
        printf("Vertex Shader Compile Log: \n%s", vertexCompileLog);
        free(vertexCompileLog);
    }
    else
    {
        printf("NO ERRORS IN VERTEX SHADER\n\n");
    }
    
    glCompileShader(fragmentShader);
    
    //check output
    GLint fragmentCompileLogLength;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &fragmentCompileLogLength);
    
    if(fragmentCompileLogLength > 0)
    {
        GLchar* fragmentCompileLog = (GLchar*)malloc(fragmentCompileLogLength);
        glGetShaderInfoLog(fragmentShader, fragmentCompileLogLength, &fragmentCompileLogLength, fragmentCompileLog);
        printf("Fragment Shader Compile Log: \n%s", fragmentCompileLog);
        free(fragmentCompileLog);
    }
    else
    {
        printf("NO ERRORS IN FRAGMENT SHADER\n\n");
    }

    
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    
    glBindAttribLocation(program, 0, "position");
    glBindAttribLocation(program, 1, "normal");
    glBindAttribLocation(program, 2, "texture");

    
    glGenTextures(1, &textureID);
    
    glLinkProgram(program);
    
    
    //check output
    GLsizei programLinkLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &programLinkLogLength);
    
    
    if(programLinkLogLength > 0)
    {
        GLchar* programLinkLog = (GLchar *)malloc(programLinkLogLength);
        glGetProgramInfoLog(program, 0xFFFF, &programLinkLogLength, programLinkLog);
        printf("Program Link Log: \n%s", programLinkLog);
        free(programLinkLog);
    }
    else
    {
        printf("NO ERRORS FOR PROGRAM LINKER\n\n");
    }
    
    glUseProgram(program);
    glutMainLoop();
    
    
    
    return 0;
}

/**
 * Idle Function
 */
void IdleFunction(void)
{
}

/**
 * Display Function.
 */
void ApplicationDisplay(void)
{
    // setup variables for vertex and fragment shaders
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // get projection matrix
    projectionMatrix = scene.perspective;
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, (GLfloat*)&projectionMatrix);
    
    // get light position
    ATVector4D lpos = scene.getLights()[0].getPosition();
    glUniform4f(glGetUniformLocation(program, "lightPosition"), lpos.getX(), lpos.getY(), lpos.getZ(), lpos.getW());
    
    // get light color
    ATColor lcol = scene.getLights()[0].getColor();
    glUniform4f(glGetUniformLocation(program, "lightColor"), lcol.getredF(), lcol.getgreenF(), lcol.getblueF(), lcol.getalphaF());
    
    // get ambient color
    ATColor ambientColor = scene.ambientColor;
    glUniform4f(glGetUniformLocation(program, "ambientColor"), ambientColor.getredF(), ambientColor.getgreenF(), ambientColor.getblueF(), ambientColor.getalphaF());
    
    
    // draw every object in the scene
    for(unsigned int objIdx=0; objIdx < scene.getObjects().size(); objIdx++)
    {
        ATTriangleGroup &thisObject = scene.getObjects()[objIdx];
        
        // get modelview matrix
        modelViewMatrix = scene.getObjects()[objIdx].modelView;
        glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, (GLfloat*)&modelViewMatrix);
        
        // set textured
        glUniform1i(glGetUniformLocation(program, "textured"), thisObject.textured);

        if(thisObject.textured)
        {
            
            // debug - turn on green in case texture is not loaded
            glUniform4f(glGetUniformLocation(program, "color"),0.0f, 1.0f, 0.0f, 0.0f);
            


            int width, height, maxNumber;
            ATColor *texels;
        
            readInPPMtexture((*thisObject.textureMap), &width, &height, &maxNumber, &texels);

            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
            
            delete [] texels;
        }
        else
        {
            // get object color
            ATColor col = scene.getObjects()[objIdx].materialColor;
            glUniform4f(glGetUniformLocation(program, "color"), col.getredF(), col.getgreenF(), col.getblueF(), col.getalphaF());
        }
        
        // get specular color and shininess
        glUniform1f(glGetUniformLocation(program, "shininess"), scene.getObjects()[objIdx].shininess);
        ATColor specColor = scene.getObjects()[objIdx].specularColor;
        glUniform4f(glGetUniformLocation(program, "specularColor"), specColor.getredF(), specColor.getgreenF(), specColor.getblueF(), specColor.getalphaF());
        long trianglesize = thisObject.triangles.size();
        float geometry[12*trianglesize];
        int idx = 0;
        
        float normals[9*trianglesize];
        int nidx = 0;
        
        float textures[6*trianglesize];
        int tidx = 0;
        
        for(unsigned int i=0; i< trianglesize; i++)
        {
            geometry[idx++]  = thisObject.triangles[i].vertA.getX();
            geometry[idx++]  = thisObject.triangles[i].vertA.getY();
            geometry[idx++]  = thisObject.triangles[i].vertA.getZ();
            geometry[idx++]  = thisObject.triangles[i].vertA.getW();
            
            //normals
            normals[nidx++] = thisObject.triangles[i].normA.getX();
            normals[nidx++] = thisObject.triangles[i].normA.getY();
            normals[nidx++] = thisObject.triangles[i].normA.getZ();
            
            geometry[idx++]  = thisObject.triangles[i].vertB.getX();
            geometry[idx++]  = thisObject.triangles[i].vertB.getY();
            geometry[idx++]  = thisObject.triangles[i].vertB.getZ();
            geometry[idx++]  = thisObject.triangles[i].vertB.getW();
            
            // normals
            normals[nidx++] = thisObject.triangles[i].normB.getX();
            normals[nidx++] = thisObject.triangles[i].normB.getY();
            normals[nidx++] = thisObject.triangles[i].normB.getZ();
            
            geometry[idx++]  = thisObject.triangles[i].vertC.getX();
            geometry[idx++]  = thisObject.triangles[i].vertC.getY();
            geometry[idx++]  = thisObject.triangles[i].vertC.getZ();
            geometry[idx++]  = thisObject.triangles[i].vertC.getW();
            
            // normals
            normals[nidx++] = thisObject.triangles[i].normC.getX();
            normals[nidx++] = thisObject.triangles[i].normC.getY();
            normals[nidx++] = thisObject.triangles[i].normC.getZ();
            
            if (thisObject.textured) 
            {
                //textures
                textures[tidx++] = thisObject.triangles[i].textA.getX();
                textures[tidx++] = thisObject.triangles[i].textA.getY();
                
                //textures
                textures[tidx++] = thisObject.triangles[i].textB.getX();
                textures[tidx++] = thisObject.triangles[i].textB.getY();
                
                //textures
                textures[tidx++] = thisObject.triangles[i].textC.getX();
                textures[tidx++] = thisObject.triangles[i].textC.getY();   
            }
        }
        
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, geometry);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, normals);
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, textures);
        glEnableVertexAttribArray(2);
        
        // activate the texture
        if(thisObject.textured)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(glGetUniformLocation(program, "textureMap"), 0);
        }
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(3*trianglesize));
        
        
    }
    glutSwapBuffers();
    
}
