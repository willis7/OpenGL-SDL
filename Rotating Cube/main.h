# ifndef _MAIN_H
# define _MAIN_H

# include <GL/gl.h>                                     // header for standard OpenGL calls (gl_ calls )
# include <GL/glu.h>                                    // header for OpenGl Utility library (glu_ calls)

# include <stdlib.h>                                    // for exit()
# include <math.h>                                      // standard math library

# include <iostream>                                    // input/output streams (cout, cerr etc.)

# include "SDL.h"                                       // the header gets included from the path supplied by compiler flags
                                                        // see Makefile for this (or try running sdl-config --libs or --cflags

# define SCREEN_WIDTH    800                            // default window width
# define SCREEN_HEIGHT   600                            // default window height
# define SCREEN_DEPTH    16                             // default color depth


using namespace std;

extern int  VideoFlags;                                 // the pixel format flags

extern SDL_Surface * MainWindow;                        // our drawable portion of the window

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
void HandleKeyPressEvent(SDL_keysym * keysym);

// This shuts down SDL and quits program
void Quit(int ret_val);

# endif // _MAIN_H




///////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES *
//
// Nothing new added to this file in this tutorial.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
//
//
