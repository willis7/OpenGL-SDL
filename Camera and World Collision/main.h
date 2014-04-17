#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>                                      // Header File For The OpenGL32 Library
#include <GL/glu.h>                                     // Header File For The GLu32 Library

#include "SDL.h"

#include "3DMath.h"                                     // Include our new math header file
#include "Camera.h"


#define SCREEN_WIDTH 800                                // We want our screen width 800 pixels
#define SCREEN_HEIGHT 600                               // We want our screen height 600 pixels
#define SCREEN_DEPTH 16                                 // We want 16 bits per pixel

using namespace std;

extern int VideoFlags;                                  // The Pixel Format flags
extern SDL_Surface * MainWindow;                        // Our Drawable portion of the window

extern CCamera g_Camera;                                // This will be our global camera data

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
// A radius variable (m_radius) was added to our camera class, along with these 2
// functions:
// 
//
// // This intializes the camera's sphere radius
// void SetCameraRadius(float radius);
//
// // This takes a list of vertices and the vertex count to determine if the camera's
// // shere has collided with them.
// void CheckCameraCollision(CVector3 *pVertices, int numOfVerts);
//
//
// We can now check the camera against the world to see if we collided and need to
// be stopped.  Other collision techniques such as ellipse, bounding box and cylinder
// are good to have in your camera library.  It is important to be sure to remember to
// set the camera's radius upon initialization, otherwise it will not detect any collision.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
