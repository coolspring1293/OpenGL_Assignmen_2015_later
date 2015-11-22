//
//  main.cpp
//  OpenGL_Read_in_obj
//
//  Created by Kieran Will on 11/23/15.
//  Copyright © 2015 Kieran Will. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


// The GLM code for loading and displying OBJ mesh file
#include "glm.hpp"

// The trackball code for rotating the model
#include "trackball.h"


#include <string>
#include <cstring>

/// Init the OpenGL
void init(void);

/// Display the Object
void display(void);

/// Reshape the Window
void reshape(int w, int h);

/// Mouse Messenge
void mouse(int button, int state, int x, int y);

/// Motion Function
void motion(int x, int y);

/// Keyboard Messenge
void keyboard(unsigned char key, int x, int y);

/// Idle Function
void idle(void);

// The size of the GLUT window
int window_width  = 800;
int window_height = 600;

std::string File_Name_Tmp;
double Obj_Size_Display = 1.00;

// The OBJ model
GLMmodel* pModel = NULL;

// The current modelview matrix
double pModelViewMatrix[16];

// If mouse left button is pressed
bool bLeftBntDown = false;

// Old position of the mouse
int OldX = 0;
int OldY = 0;
bool isRead = false;

#include <fstream>
bool ReadbyFileName() {
    std::string t;
    do {
        std::cout << "Please input path:";
        std::cin >> t;
        std::ifstream fin(t.c_str());
        if (fin) {
            std::cout << "Input " << t << " sucessfully!\n";
            File_Name_Tmp = t.c_str();
            isRead = true;
        }
        else {
            std::cout << t << " does not exist in fact, please input again!!\n";
        }
        fin.close();
    } while (!isRead);
    return true;
}


int main(int argc, char* argv[]) {
    ReadbyFileName();
    
    // Initialize the GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Read an OBJ file");
    init();
    
    // Set the callback function
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    
    glutMainLoop();
    
    return 0;
}

/// Initialize the OpenGL
void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)window_width/(float)window_height, 0.01f, 200.0f);
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    glEnable(GL_CULL_FACE);
    
    // Setup other misc features.
    glEnable( GL_LIGHTING );
    glEnable( GL_NORMALIZE );
    glShadeModel( GL_SMOOTH );
    
    // Setup lighting model.
    GLfloat light_model_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_direction[] = {0.0f, 0.0f, 10.0f, 0.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_model_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable( GL_LIGHT0 );
    
    // Init the dlg of the open file
    //PopFileInitialize( NULL );
}

/// Display the Object
void display()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslated( 0.0, 0.0, -5.0 );
    glMultMatrixd( pModelViewMatrix );
    
    if ( pModel )
    {
        glmDraw( pModel, GLM_FLAT );
    }
    
    glutSwapBuffers();
}


/// Reshape the Window
void reshape(int w, int h) {
    // Update the window's width and height
    window_width  = w;
    window_height = h;
    
    // Reset the viewport
    glViewport(0, 0, window_width, window_height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)window_width/(float)window_height, 0.01f, 200.0f);
    
    glutPostRedisplay();
}

/// Keyboard Messenge

void getFileName(char *filename) {
    strcpy(filename, File_Name_Tmp.c_str());
}



void keyboard(unsigned char key, int x, int y)
{
    
    // The obj file will be loaded
    char FileName[128] = "/Users/liuw53/Desktop/newgirl/newgirl.obj"; //"/Users/liuw53/Desktop/tri.obj";
    char TitleName[128]= "/Users/liuw53/Desktop/newgirl/newgirl.mtl";
    
    // Center of the model
    float modelCenter[] = {0.0f, 0.0f, 0.0f};
    
    switch ( key ) {
        case 'o':
        case 'O':
            //getFileName(FileName);
            // If there is a obj model has been loaded, destroy it
            if ( pModel )
            {
                glmDelete( pModel );
                pModel = NULL;
            }
            
            // Load the new obj model
            pModel = glmReadOBJ( FileName );
            
            // Generate normal for the model
            glmFacetNormals( pModel );
            
            // Scale the model to fit the screen
            glmUnitize( pModel, modelCenter );
            
            // Init the modelview matrix as an identity matrix
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
            
            break;
            
        case '+':
            Obj_Size_Display *= 1.05;
            std::cout << "放大5%,当前倍数: " << Obj_Size_Display <<"\n";
            glMatrixMode( GL_MODELVIEW );
            glLoadIdentity();
            glLoadMatrixd( pModelViewMatrix );
            glScaled( 1.05, 1.05, 1.05 );
            glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
            break;
            
        case '-':
            Obj_Size_Display *= 0.95;
            std::cout << "缩小5%,当前倍数: " << Obj_Size_Display <<"\n";
            glMatrixMode( GL_MODELVIEW );
            glLoadIdentity();
            glLoadMatrixd( pModelViewMatrix );
            glScaled( 0.95, 0.95, 0.95 );
            glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
            break;
        case 'q':
            std::cout << "Quited!\n";
            exit(0);
        default:
            break;
    }
    
    glutPostRedisplay();
}

/// Mouse Messenge
void mouse(int button, int state, int x, int y)
{
    if ( pModel )
    {
        if ( state==GLUT_DOWN && button==GLUT_LEFT_BUTTON )
        {
            OldX = x;
            OldY = y;
            bLeftBntDown = true;
        }
        else if ( state==GLUT_DOWN && button==GLUT_LEFT_BUTTON )
        {
            bLeftBntDown = false;
        }
    }
}

/// Motion Function
void motion(int x, int y)
{
    if ( bLeftBntDown && pModel )
    {
        float fOldX =  2.0f*OldX/(float)window_width  - 1.0f;
        float fOldY = -2.0f*OldY/(float)window_height + 1.0f;
        float fNewX =  2.0f*x/(float)window_width  - 1.0f;
        float fNewY = -2.0f*y/(float)window_height + 1.0f;
        
        double pMatrix[16];
        trackball_opengl_matrix( pMatrix, fOldX, fOldY, fNewX, fNewY);
        
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glLoadMatrixd( pMatrix );
        glMultMatrixd( pModelViewMatrix );
        glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
        
        OldX = x;
        OldY = y;
        glutPostRedisplay();
    }
}

/// Idle function
void idle(void)
{
    glutPostRedisplay();
}