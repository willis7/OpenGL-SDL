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
// This tutorial creates a bezier curve and allows a sphere to move along it
// with the LEFT and RIGHT arrow keys.  You will also note that we display
// 2 of the curve control points to get a better visualization of what is going on.
// 
// Here is the equation we are using:
//
// B(t) = P1 * ( 1 - t )^3 + P2 * 3 * t * ( 1 - t )^2 + P3 * 3 * t^2 * ( 1 - t ) + P4 * t^3 
//
// This is pretty straight forward, even though it looks complicated. If you got 
// up to Trig, you will notice that this is a polynomial.  That is what a curve is.
// "t" is the time from 0 to 1.  You could also think of it as the distance along the
// curve, because that is really what it is.  P1 - P4 are the 4 control points.
// They each have an (x, y, z) associated with them.  P1 is the starting point. P2 is
// the first control point, P3 is the second control point, and P4 is the end point of the
// curve.  You will hear that this is a 4 control point equation. I usually think of it
// as 2 control points because the 2 end points are just the start and end of the curve.
// 
// Bezier curves are awesome.  You can use them to make realistic camera movements.
// Tunnel effects most frequently use bezier curves to shape the tunnel.  Once you
// understand one single bezier curve, you can then branch off and learn how to create
// curved surfaces.  This will also prepare you to learn about NURBS and SPLINES.
// 
// I would like to thank Paul Bourke at http://astronomy.swin.edu.au/pbourke/curves/bezier/
// 
// His web site helped TREMENDOUSLY with understanding Bezier Curves, as well as his source code.  
// I suggest checking it out because it has a HUGE range of other graphics programming topics.
// 
// Once again, go to http://astronomy.swin.edu.au/pbourke/curves/bezier/ to get a more clear
// understanding of the math behind Bezier Curves as well as excellent visual examples.
//
// Good Luck!
//
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
