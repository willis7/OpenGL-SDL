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

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// *Vector Class*
// This will be the start of our vector class.  In the beginning, it will be simple,
// but as we add to it, it will include numorous functions and a couple more data types.
// For now, we will use it for the camera class down below.  Right now, just holding
// an X, Y and Z is perfect for what we need.  If we weren't going to add to this we
// would just make it a structure, not a class.

class CVector3 {                                        // Remember, we use a C in front to show it's a class

public:                                                 // We want this data public so we don't have to use data access functions.  We could use a struct but I chose not to.
    float x, y, z;                                      // We just want a float for a X Y and Z.
};

// *Camera Class*
// This will be used to hold our camera position, our view, and up vector.
// Right now, we have 3 CVector3's that will hold this information.
// On top of that, we have some helper functions.  We are going to make
// all of our data public so we can access our position and view explicitly.
// I decided this for now so we don't clutter this class right now.  Later we will :).
// Like the CVector3 class, I could have just used a struct instead of a class since
// it's default is public, but I chose not to because eventually it will have member functions.
// Also, it's a good idea to make something a class when you have functions in it just
// to keep from the confusion of C and C++.  I try to only use 'struct' for NON function objects.

class CCamera {

public:

    CVector3 m_vPosition;                                    // The camera's position
    CVector3 m_vView;                                        // The camera's View
    CVector3 m_vUpVector;                                    // The camera's UpVector

    // We put a m in front of the variables because it shows that they are "member" variables.
    // I also put a "v" in front of CVector3 instances so we know it's a vector with x, y, z.
    // This is a good habit to get into.  It's easier to keep track of what's class data
    // verses some local variables create in the helper functions.

    // Our camera constructor
    CCamera();    
    
    // This changes the position, view, and up vector of the camera.
    // This is primarily used for initialization
    void PositionCamera(float positionX, float positionY, float positionZ,
                         float viewX,     float viewY,     float viewZ,
                        float upVectorX, float upVectorY, float upVectorZ);

    // This will move the camera forward or backwards depending on the speed
    void MoveCamera(float speed);
};


// Extern our global variables so other source files can use them
extern CCamera    g_Camera;                                    // This will be our global camera data

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


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
//
// * QUICK NOTES * 
//
// In this tutorial, we added the CVector and CCamera class to this header file.
// You might be noticing that this header file is getting a bit cluttered.  Soon
// we will make another header file the split the rest of this stuff up.
//
// We also added a global instance of the class CCamera, called gCamera.  Now any
// source file can access the camera's current view and position.
// 
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
