# ifndef _MAIN_H
# define _MAIN_H

# include <GL/gl.h>                                     // header for standard OpenGL calls (gl_ calls )
# include <GL/glu.h>                                    // header for OpenGl Utility library (glu_ calls)

# include <stdlib.h>                                    // for exit()
# include <string.h>                                    // for memset
# include <math.h>                                      // standard math library

# include <iostream>                                    // input/output streams (cout, cerr etc.)

# include "SDL.h"                                       // the header gets included from the path supplied by compiler flags
                                                        // see Makefile for this (or try running sdl-config --libs or --cflags

#define SCREEN_WIDTH 800                                // We want our screen width 800 pixels
#define SCREEN_HEIGHT 600                                // We want our screen height 600 pixels
#define SCREEN_DEPTH 16                                    // We want 16 bits per pixel


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

#define MAX_TEXTURES 100                                // The Maximum number of textures to load 

using namespace std;

typedef unsigned char byte;

// Here is our simple vector class
class CVector3 {                                        // Remember, we use a C in front to show it's a class

public:                                                 // We want this data public so we don't have to use data access functions.  We could use a struct but I chose not to.
    float x, y, z;                                      // We just want a float for a X Y and Z.
};

// This is our camera class
class CCamera {

public:

    CVector3 m_vPosition;                                // The camera's position
    CVector3 m_vView;                                    // The camera's View
    CVector3 m_vUpVector;                                // The camera's UpVector

    // Our camera constructor can take values, or just default to standard ones
    CCamera(float positionX = 0.0f, float positionY = 0.0f, float positionZ = 0.0f,
            float viewX = 0.0f,     float viewY = 0.0f,     float viewZ = -1.0f,
            float upVectorX = 0.0f, float upVectorY = 1.0f, float upVectorZ = 0.0f);
    
    // This changes the position, view, and up vector of the camera.
    // This is primarily used for initialization
    void PositionCamera(float positionX = 0.0f, float positionY = 0.0f, float positionZ = 0.0f,
                        float viewX = 0.0f,     float viewY = 0.0f,     float viewZ = -1.0f,
                        float upVectorX = 0.0f, float upVectorY = 1.0f, float upVectorZ = 0.0f);

    // This rotates the camera's view around the position depending on the values passed in.
    // If we want to rotate around the X axis a certain amount, we would just pass in a value
    // in the first parameter, likewise for the rest.  Pass in 0 if we do not want to rotate around that axis
    void RotateView(float X, float Y, float Z);

    
    // If we don't want use the keyboard to look around, this will use the mouse.
    void MoveCameraByMouse();

    // This will move the camera forward or backward depending on the speed
    void MoveCamera(float speed);
};

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


extern int  VideoFlags;                                 // the pixel format flags

extern SDL_Surface * MainWindow;                        // our drawable portion of the window

typedef unsigned int UINT;
extern UINT g_Textures[MAX_TEXTURES];                   // This is our texture data array

// This loads a texture into openGL from a file(IE , "bitmap.bmp")
void CreateTexture(UINT textureArray[],char *strFileName,int textureID);

// This initializes the whole program
void Init(void);

// This controls our main program loop
void MainLoop(void);

// This toggles between Fullscreen and windowed mode
void ToggleFullScreen(void);

// This is our function that makes creating a window modular and easy
void CreateMyWindow(const char * strWindowName, int width, int heigh, int VideoFlags);

// This allows us to configure our window for openGL and backbuffered
void SetupPixelFormat(void);

// This initializes our screen translations and projections
void InitializeOpenGL(int width, int height);

// This resizes the OpenGL viewport
void SizeOpenGLScreen(int width, int height);

// This draws everything to the screen
void RenderScene(void);

// This handles the keypress events generated when the user presses a key
void KeyPressed(SDL_keysym * keysym);

// This shuts down SDL and quits program
void Quit(int ret_val);

# endif // _MAIN_H


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// In this tutorial we added the CVector and CCamera class to this file.
// Since we are using textures, we also added the MAX_TEXTURES define.
// 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
// 
