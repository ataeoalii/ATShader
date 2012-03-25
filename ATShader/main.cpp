//
//  main.cpp
//  ATShader
//
//  Created by Andrew Taeoalii on 3/9/12.
//  Copyright 2012 Apple Inc. All rights reserved.
//




#include <iostream>
#include <string.h>
#include <GLUT/glut.h>
#include <fstream>
#include "ATMatrix4.h"
#include "ATVector4D.h"
#include "ATScene.h"

using namespace std;

void IdleFunction();
void ApplicationDisplay();



GLuint program;
ATMatrix4 modelViewMatrix;
ATMatrix4 projectionMatrix;
ATScene scene;



int main (int argc, const char * argv[])
{
    
    
    char* filename = (char*)"/Users/DreW/Documents/ATShader/ATShader/scene.u3d";
    
    // read in scene file
    scene.sceneReaderPhong(filename);
    
    ifstream vertexShaderFile("/Users/DreW/Documents/ATShader/ATShader/ATVertexShader.sdr");
	printf("Reading in file ATVertexShader.sdr\n");
    char* cwd = getcwd(NULL, 0);
    printf("Current Dir%s\n", cwd);
    string vertStream((istreambuf_iterator<char>(vertexShaderFile)), istreambuf_iterator<char>());
    
    const GLchar* vertexShaderCode = (const GLchar*)vertStream.c_str();
    
    vertexShaderFile.close();
    
    ifstream fragmentShaderFile("/Users/DreW/Documents/ATShader/ATShader/ATFragmentShader.sdr");
	printf("Reading in file ATFragmentShader.sdr\n");
    
    string fragStream((istreambuf_iterator<char>(fragmentShaderFile)), istreambuf_iterator<char>());
    const GLchar* fragmentShaderCode = (const GLchar*)fragStream.c_str();
    
    glutInit(&argc, (char**)argv);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(scene.width, scene.height);
    glutCreateWindow("ATShader");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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
    
    glLinkProgram(program);

    glEnable(GL_DEPTH_TEST);
    
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
    glutPostRedisplay();
}

/**
 * Display Function.
 */
void ApplicationDisplay(void)
{
    static float rotater = 0.0f;
    rotater+=0.01f;
    // setup variables for vertex and fragment shaders
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    
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
        vector<ATTriangle> triangles = scene.getObjects()[objIdx].triangles;
        
        // get modelview matrix
        modelViewMatrix = scene.getObjects()[objIdx].modelView;
        glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, (GLfloat*)&modelViewMatrix);
        
        // get object color
        ATColor col = scene.getObjects()[objIdx].materialColor;
        glUniform4f(glGetUniformLocation(program, "color"), col.getredF(), col.getgreenF(), col.getblueF(), col.getalphaF());
        
        float geometry[12*triangles.size()];
        int idx = 0;
        
        float normals[9*triangles.size()];
        int nidx = 0;
        
        for(unsigned int i=0; i< triangles.size(); i++)
        {
            geometry[idx++]  = triangles[i].vertA.getX();
            geometry[idx++]  = triangles[i].vertA.getY();
            geometry[idx++]  = triangles[i].vertA.getZ();
            geometry[idx++]  = triangles[i].vertA.getW();
        
            //normals
            normals[nidx++] = triangles[i].normA.getX();
            normals[nidx++] = triangles[i].normA.getY();
            normals[nidx++] = triangles[i].normA.getZ();
            
            geometry[idx++]  = triangles[i].vertB.getX();
            geometry[idx++]  = triangles[i].vertB.getY();
            geometry[idx++]  = triangles[i].vertB.getZ();
            geometry[idx++]  = triangles[i].vertB.getW();
        
            // normals
            normals[nidx++] = triangles[i].normB.getX();
            normals[nidx++] = triangles[i].normB.getY();
            normals[nidx++] = triangles[i].normB.getZ();
            
            geometry[idx++]  = triangles[i].vertC.getX();
            geometry[idx++]  = triangles[i].vertC.getY();
            geometry[idx++]  = triangles[i].vertC.getZ();
            geometry[idx++]  = triangles[i].vertC.getW();
        
            // normals
            normals[nidx++] = triangles[i].normC.getX();
            normals[nidx++] = triangles[i].normC.getY();
            normals[nidx++] = triangles[i].normC.getZ();
        }
    
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, geometry);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, normals);
        glEnableVertexAttribArray(1);
        
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(3*triangles.size()));
        
    
    }
    glutSwapBuffers();
    
}
