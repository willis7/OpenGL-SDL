#ifndef _MAIN_H
#define _MAIN_H

//# ifndef GL_GLEXT_LEGACY
//# endif

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>                                      // Header File For The OpenGL32 Library
#include <GL/glu.h>                                     // Header File For The GLu32 Library
#include <GL/glext.h>

# include <string>

#include "SDL.h"


#define SCREEN_WIDTH 800                                // We want our screen width 800 pixels
#define SCREEN_HEIGHT 600                               // We want our screen height 600 pixels
#define SCREEN_DEPTH 16                                 // We want 16 bits per pixel


#define MAX_TEXTURES 1000                               // The maximum amount of textures to load

extern unsigned int g_Texture[MAX_TEXTURES];            // This stores the texture IDs

# ifndef GL_CLAMP_TO_EDGE 
// We need to define this for glTexParameteri()
#define GL_CLAMP_TO_EDGE    0x812F                      // This is for our skybox textures

# endif


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// To allow volumetric fog, we need to add some EXT defines and a typedef

// These are the defines for the flags that are passed into glFogi()
#define GL_FOG_COORDINATE_SOURCE_EXT            0x8450
#define GL_FOG_COORDINATE_EXT                   0x8451


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

#include "Terrain.h"                                    // Include our header file for our Terrain.cpp
                            

// This is our basic 3D point/vector class
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

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


extern int VideoFlags;                                    // The Pixel Format flags
extern SDL_Surface * MainWindow;                        // Our Drawable portion of the window

// This Controls our MainLoop
void MainLoop(void);

// This toggles between the Full Screen mode and the windowed mode
void ToggleFullScreen(void);

void CreateTexture(unsigned int textureArray[],char *strFileName,int textureID);

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

bool isExtensionSupported(string ext);

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
//// We added some variables and defines to allow volumetric fog.
////
////
//// Ben Humphrey (DigiBen)
//// Game Programmer
//// DigiBen@GameTutorials.com
//// Co-Web Host of www.GameTutorials.com
////
////
