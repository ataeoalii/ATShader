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
    
    cout << scene.getObjects()[0].triangles.size() << endl;
    cout << "triangle size" << endl;
    
    // get projection matrix
    projectionMatrix = scene.perspective; //ATMatrix4::ConcatMatrices(scene.perspective, scene.viewport);
    
//    projectionMatrix = ATMatrix4(1.0f, 0.0f, 0.0f, 0.0f,
//                                 0.0f, 1.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 1.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f, 1.0f);
    
    // get modelview matrix
    modelViewMatrix = scene.getObjects()[0].modelView;
    
//    modelViewMatrix = ATMatrix4(1.0f, 0.0f, 0.0f, 0.0f,
//                                 1.0f, 0.0f, 0.0f, 0.0f,
//                                 1.0f, 0.0f, 0.0f, 0.0f,
//                                 1.0f, 0.0f, 0.0f, 0.0f);
    
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
    
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    
    
    
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

    // setup variables for vertex and fragment shaders
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    
    glBindAttribLocation(program, 0, "position");
    //glUniform1f(glGetUniformLocation(program, "phase"), phase);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, (GLfloat*)&modelViewMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, (GLfloat*)&projectionMatrix);
    ATColor col = scene.getObjects()[0].materialColor;
    glUniform4f(glGetUniformLocation(program, "color"), col.getredF(), col.getgreenF(), col.getblueF(), col.getalphaF());
    ATVector4D lpos = scene.getLights()[0].getPosition();
    glUniform4f(glGetUniformLocation(program, "eyeLightPosition"), lpos.getX(), lpos.getY(), lpos.getZ(), lpos.getW());
    
    vector<ATTriangle> triangles = scene.getObjects()[0].triangles;
    
    ATMatrix4 atm = ATMatrix4::ConcatMatrices(projectionMatrix, modelViewMatrix);
    
    float geometry[12*triangles.size()];
    int idx = 0;
    for(unsigned int i=0; i<2; i++)
    {
        geometry[idx++]  = triangles[i].vertA.getX();
        geometry[idx++]  = triangles[i].vertA.getY();
        geometry[idx++]  = triangles[i].vertA.getZ();
        geometry[idx++]  = 1.0f;
        
        geometry[idx++]  = triangles[i].vertB.getX();
        geometry[idx++]  = triangles[i].vertB.getY();
        geometry[idx++]  = triangles[i].vertB.getZ();
        geometry[idx++]  = 1.0f;
        
        geometry[idx++]  = triangles[i].vertC.getX();
        geometry[idx++]  = triangles[i].vertC.getY();
        geometry[idx++]  = triangles[i].vertC.getZ();
        geometry[idx++]  = 1.0f;
        
    }
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, geometry);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(3*triangles.size()));
    
    
    glutSwapBuffers();
    
}
