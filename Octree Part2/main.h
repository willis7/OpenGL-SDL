#ifndef _MAIN_H
#define _MAIN_H


#include <GL/gl.h>                      // Header File For The OpenGL32 Library
#include <GL/glu.h>                     // Header File For The GLu32 Library
#include <iostream>                     // Include our c++ standard header
#include <fstream>                      // Include this to use ifstream
#include <string>                       // Include this to use the string class
#include <vector>                       // Include this to use STL vectors

#include "SDL.h"

using namespace std;                    // Start off with the standard namespace

#define SCREEN_WIDTH  1024              // We want our screen width 640 pixels
#define SCREEN_HEIGHT 768               // We want our screen height 480 pixels
#define SCREEN_DEPTH 16                 // We want 16 bits per pixel

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This is our basic 3D point/vector class
class CVector3
{
public:
    
    // A default constructor
    CVector3() {}

    // This is our constructor that allows us to initialize our data upon creating an instance
    CVector3(float X, float Y, float Z) 
    { 
        x = X; y = Y; z = Z;
    }

    // Here we overload the + operator so we can add vectors together 
    CVector3 operator+(CVector3 vVector)
    {
        // Return the added vectors result.
        return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
    }

    // Here we overload the - operator so we can subtract vectors 
    CVector3 operator-(CVector3 vVector)
    {
        // Return the subtracted vectors result
        return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
    }
    
    // Here we overload the * operator so we can multiply by scalars
    CVector3 operator*(float num)
    {
        // Return the scaled vector
        return CVector3(x * num, y * num, z * num);
    }

    // Here we overload the / operator so we can divide by a scalar
    CVector3 operator/(float num)
    {
        // Return the scale vector
        return CVector3(x / num, y / num, z / num);
    }

    float x, y, z;                      
};


// Because COctree.h needs a reference to CVector3 we include it after the class definition
#include "Octree.h"

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


// Extern our global variables so other source files can use them

extern float g_FrameInterval;
extern int VideoFlags;                                     // defines how the drawing surface should be
extern SDL_Surface *MainWindow;                            // our main window for drawing

// This controls our main program loop
void MainLoop();

// This changes the screen to full screen mode
void ToggleFullScreen();

void HandleKeyPressEvent(SDL_keysym *);

void RecreateOctree();

// This is our own function that makes creating a window modular and easy
void CreateMyWindow(char *strWindowName, int width, int height, int Videoflags);

// This allows us to configure our window for OpenGL and backbuffered
void SetupPixelFormat(void);

// This inits our screen translations and projections
void SizeOpenGLScreen(int width, int height);

// This sets up OpenGL
void InitializeOpenGL(int width, int height);

// This initializes the whole program
void Init();

// This draws everything to the screen
void RenderScene();

// This Shuts down SDL and quits the program
void Quit(int ret_val);
#endif 


/////////////////////////////////////////////////////////////////////////////////
////
//// * QUICK NOTES *
////
//// We added the Frustum.h to this file's includes.  Nothing else was added.
////
////
//// Ben Humphrey (DigiBen)
//// Game Programmer
//// DigiBen@GameTutorials.com
//// Co-Web Host of www.GameTutorials.com
////
//// © 2001 GameTutorials
