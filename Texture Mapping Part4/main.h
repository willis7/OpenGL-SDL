#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>                                      // Header File For The OpenGL32 Library
#include <GL/glu.h>                                     // Header File For The GLu32 Library

#include "SDL.h"

#define SCREEN_WIDTH 800                                // We want our screen width 800 pixels
#define SCREEN_HEIGHT 600                               // We want our screen height 600 pixels
#define SCREEN_DEPTH 16                                 // We want 16 bits per pixel

#define MAX_TEXTURES 1                                  // This says how many texture we will be using

using namespace std;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// These defines are used to tell us about the type of TARGA file it is
#define TGA_RGB      2      // This tells us it's a normal RGB (really BGR) file
#define TGA_A        3      // This tells us it's a ALPHA file
#define TGA_RLE     10      // This tells us that the targa is Run-Length Encoded (RLE)

// This is our image structure for our targa data
struct tImageTGA
{
    int channels;           // The channels in the image (3 = RGB : 4 = RGBA)
    int sizeX;              // The width of the image in pixels
    int sizeY;              // The height of the image in pixels
    unsigned char *data;    // The image pixel data
};

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *



extern int VideoFlags;                                  // The Pixel Format flags
extern SDL_Surface * MainWindow;                        // Our Drawable portion of the window

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This loads a targa (TGA) file and returns its data
tImageTGA *LoadTGA(const char *filename);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


// This Controls our MainLoop
void MainLoop(void);

// This toggles between the Full Screen mode and the windowed mode
void ToggleFullScreen(void);

// This allows us to configure our window for OpenGL and backbuffered
void SetupPixelFormat(void);

// This is our own function that makes creating a window modular and easy
void CreateMyWindow(const char *strWindowName, int width, int height, int VideoFlags);

// This loads a texture into openGL from a file (IE, "bitmap.bmp")
void CreateTexture(unsigned int textureArray[], char *strFileName, int textureID);

// This inits our screen translations and projections
void SizeOpenGLScreen(int width, int height);

// This sets up OpenGL
void InitializeOpenGL(int width, int height);

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
// We added MAX_TEXTURES to this header file.  This define is the size of the g_Texture[]
// array, which we also extern here so init.cpp can free it's data.
// 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
