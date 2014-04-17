#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>                                      // Header File For The OpenGL32 Library
#include <GL/glu.h>                                     // Header File For The GLu32 Library

#include "SDL.h"


#define SCREEN_WIDTH 800                                // We want our screen width 800 pixels
#define SCREEN_HEIGHT 600                               // We want our screen height 600 pixels
#define SCREEN_DEPTH 16                                 // We want 16 bits per pixel

using namespace std;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

#define PI 3.14159                                      // This is our define for PI

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *



// This is our basic vector class
struct CVector3
{
public:
    
    // A default constructor
    CVector3() {}

    // This is our constructor that allows us to initialize our data upon creating an instance
    CVector3(float X, float Y, float Z) 
    { 
        x = X; y = Y; z = Z;
    }

    // The x, y and z values of this vector
    float x, y, z;                      
};


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This is our basic quaternion class
class CQuaternion 
{

public:

    // This is our default constructor, which initializes everything to an identity
    // quaternion.  An identity quaternion has x, y, z as 0 and w as 1.
    CQuaternion() { x = y = z = 0.0f;   w = 1.0f;  }
    
    // Create a constructor that will allow us to initialize the quaternion when creating it
    CQuaternion(float X, float Y, float Z, float W) 
    { 
        x = X;      y = Y;      z = Z;      w = W;
    }

    // This converts and axis angle rotation to a quaternion
    void CreateFromAxisAngle(float X, float Y, float Z, float degree);

    // This takes in an array of 16 floats to fill in a 4x4 homogeneous matrix from a quaternion
    void CreateMatrix(float *pMatrix);

private:

    // This stores the 4D values for the quaternion
    float x, y, z, w;
};

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


extern int VideoFlags;                                    // The Pixel Format flags
extern SDL_Surface * MainWindow;                        // Our Drawable portion of the window

// This Controls our MainLoop
void MainLoop(void);

// This toggles between the Full Screen mode and the windowed mode
void ToggleFullScreen(void);

// This allows us to configure our window for OpenGL and backbuffered
void SetupPixelFormat(void);

// This is our own function that makes creating a window modular and easy
void CreateMyWindow(const char *strWindowName, int width, int height, int VideoFlags);

// This inits our screen translations and projections
void SizeOpenGLScreen(int width, int height);

// This sets up OpenGL
void InitializeGL(int width, int height);

// This initializes the whole program
void Init();

// This handles the keypress events generated when the user presses a key
void HandleKeyPressEvent(SDL_keysym * keysym);

// This draws everything to the screen
void RenderScene();

// This shuts down SDL and quits program
void Quit(int ret_val);

#endif  // _MAIN_H_

/////////////////////////////////////////////////////////////////////////////////
////
//// * QUICK NOTES *
////
//// We added our basic quaternion class and PI to this header file.
////
////
//// Ben Humphrey (DigiBen)
//// Game Programmer
//// DigiBen@GameTutorials.com
//// Co-Web Host of www.GameTutorials.com
////
////
