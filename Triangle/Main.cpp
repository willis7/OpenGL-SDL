//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:         Ben Humphrey    digiben@gametutorilas.com     //
//                                                                       //
//        $Program:        Triangle                                      //
//                                                                       //
//        $Description:    Init OpenGL and Draw a triangle to the screen //
//                                                                       //
//        $Date:           3/3/01                                        //
//                                                                       //
//***********************************************************************//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/gl.h>                                         // Header File For The OpenGL32 Library
#include <GL/glu.h>                                        // Header File For The GLu32 Library

#define SCREEN_WIDTH  800                                  // We want our screen width 800 pixels
#define SCREEN_HEIGHT 600                                  // We want our screen height 600 pixels
#define SCREEN_DEPTH  16                                   // We want 16 bits per pixel

#include "SDL.h"                                           // The SDL Header File

using namespace std;

int VideoFlags = 0;                                        // Video Flags for the Create Window function
SDL_Surface * MainWindow = NULL;                           // drawing surface on the SDL window


// This is our main rendering function prototype.  It's up here for now so MainLoop() can call it.
void RenderScene();

// This Quits SDL
void Quit(int);

/////////////////////////////////// TOGGLE FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
///////
///////   This function TOGGLES between FULLSCREEN and WINDOWED mode
///////
/////////////////////////////////// TOGGLE FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void ToggleFullScreen(void)
{
    if(SDL_WM_ToggleFullScreen(MainWindow) == 0)           // try to toggle fullscreen mode for window 'MainWindow'
    {
        cerr << "Failed to Toggle Fullscreen mode : " << SDL_GetError() << endl;   // report error in case toggle fails
        Quit(0);
    }
}
///////////////////////////////   CREATE MY WINDOW   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
////////
////////  This function CREATES our WINDOW for drawing the GL stuff
////////
///////////////////////////////   CREATE MY WINDOW   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateMyWindow(const char * strWindowName, int width, int height, int VideoFlags)
{
    MainWindow = SDL_SetVideoMode(width, height, SCREEN_DEPTH, VideoFlags);     // SCREEN_DEPTH is macro for bits per pixel

    if( MainWindow == NULL )                                                    // if window creation failed
    {
        cerr << "Failed to Create Window : " << SDL_GetError() << endl;         // report error
        Quit(0);
    }

    SDL_WM_SetCaption(strWindowName, strWindowName);       // set the window caption (first argument) and icon caption (2nd arg)
}

/////////////////////////////   SETUP PIXEL FORMAT   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
///////
///////  Sets the pixel format for openGL and video flags for SDL
///////
/////////////////////////////   SETUP PIXEL FORMAT   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void SetupPixelFormat(void)
{
    //////// SURFACE IS THE DRAWABLE PORTION OF AN SDL WINDOW \\\\\\\\*

       /////////////  we set the common flags here
    VideoFlags    = SDL_OPENGL;                            // it's an openGL window
    VideoFlags   |= SDL_HWPALETTE;                         // exclusive access to hardware colour palette
    VideoFlags   |= SDL_RESIZABLE;                         // the window must be resizeable

    const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();  // query SDL for information about our video hardware

    if(VideoInfo == NULL)                                  // if this query fails
    {
        cerr << "Failed getting Video Info : " << SDL_GetError() << endl;          // report error
        Quit(0);
    }

    ///////////// we set the system dependant flags here
    if(VideoInfo -> hw_available)                          // is it a hardware surface
        VideoFlags |= SDL_HWSURFACE;
    else
        VideoFlags |= SDL_SWSURFACE;
    
    // Blitting is fast copying / moving /swapping of contiguous sections of memory
    // for more about blitting check out : http://www.csc.liv.ac.uk/~fish/HTML/blitzman/bm_blitter.html
    if(VideoInfo -> blit_hw)                               // is hardware blitting available
        VideoFlags |= SDL_HWACCEL;

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );         // tell SDL that the GL drawing is going to be double buffered
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   SCREEN_DEPTH);         // size of depth buffer
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0);          // we aren't going to use the stencil buffer
    SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 0);        // this and the next three lines set the bits allocated per pixel -
    SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 0);      // - for the accumulation buffer to 0
    SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 0);
}

//////////////////////////// RESIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function resizes the viewport for OpenGL.
/////
//////////////////////////// RESIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void SizeOpenGLScreen(int width, int height)               // Initialize The GL Window
{
    if (height==0)                                         // Prevent A Divide By Zero error
    {
        height=1;                                          // Make the Height Equal One
    }

    glViewport(0,0,width,height);                          // Make our viewport the whole window
                                                           // We could make the view smaller inside
                                                           // Our window if we wanted too.
                                                           // The glViewport takes (x, y, width, height)
                                                           // This basically means, what our drawing boundries

    glMatrixMode(GL_PROJECTION);                           // Select The Projection Matrix
    glLoadIdentity();                                      // Reset The Projection Matrix

                                                           // Calculate The Aspect Ratio Of The Window
                                                           // The parameters are:
                                                           // (view angle, aspect ration of the width to the height, 
                                                           //  The closest distance to the camera before it clips, 
                  // FOV        // Ratio                   //  The farthest distance before it stops drawing)
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 1 ,150.0f);

    // * Note * - The farthest distance should be at least 1 if you don't want some
    // funny artifacts when dealing with lighting and distance polygons.  This is a special
    // thing that not many people know about.  If it's less than 1 it creates little flashes
    // on far away polygons when lighting is enabled.

    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
    glLoadIdentity();                                      // Reset The Modelview Matrix
}

////////////////////////////////   INITIALIZE GL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////         This function handles all the initialization for openGL
/////
////////////////////////////////   INITIALIZE GL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void InitializeOpenGL(int width, int height)
{
    SizeOpenGLScreen(width, height);                       // resize the OpenGL Viewport to the given height and width
}

///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeOpenGL(SCREEN_WIDTH,SCREEN_HEIGHT);          // Initialize openGL

    // *Hint* We will put all our game init stuff here
    // Some things include loading models, textures and network initialization
}

//////////////////////////////////      HANDLE KEY PRESS EVENT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////     This function handles the keypress events generated when the user presses a key
//////
//////////////////////////////////      HANDLE KEY PRESS EVENT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void HandleKeyPressEvent(SDL_keysym * keysym)
{
    switch(keysym -> sym)                                  // which key have we got
    {
        case SDLK_F1 :                                     // if it is F1
            ToggleFullScreen();                            // toggle between fullscreen and windowed mode
            break;

        case SDLK_ESCAPE:                                  // if it is ESCAPE
            Quit(0);                                       // quit after cleaning up
            
        default:                                           // any other key
            break;                                         // nothing to do
    }
}
//////////////////////////////   MAIN GAME LOOP   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////   This function handles the main game loop
//////
//////////////////////////////   MAIN GAME LOOP   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void MainLoop(void)
{
    bool done = false;                                     // is our job done ? not yet !
    SDL_Event event;

    while(! done)                                          // as long as our job's not done
    {
        while( SDL_PollEvent(& event) )                    // look for events (like keystrokes, resizing etc.)
        {
            switch ( event.type )                          // what kind of event have we got ?
            {
                case SDL_QUIT :                            // if user wishes to quit
                    done = true;                           // this implies our job is done
                    break;

                case SDL_KEYDOWN :                         // if the user has pressed a key
                    HandleKeyPressEvent( & event. key.keysym );         // callback for handling keystrokes, arg is key pressed
                    break;

                case SDL_VIDEORESIZE :                     // if there is a resize event
                    // request SDL to resize the window to the size and depth etc. that we specify
                    MainWindow = SDL_SetVideoMode( event.resize.w, event.resize.h, SCREEN_DEPTH, VideoFlags );
                    SizeOpenGLScreen(event.resize.w, event.resize.h);   // now resize the OpenGL viewport
    
                    if(MainWindow == NULL)                 // if window resize has failed
                    {
                        cerr << " Failed resizing SDL window : " << SDL_GetError() << endl;   // report error
                        Quit(0);
                    }
                    break;

                default:                                   // any other event
                    break;                                 // nothing to do
            } // switch
        } // while( SDL_ ...

        RenderScene();                                     // draw our OpenGL scene
    } // while( ! done)
}
//////////////////////////////      MAIN      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////     create the window and calling the initialization functions
//////
//////////////////////////////      MAIN      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int main(void)
{
    // print user instructions
    cout << " Hit the F1 key to Toggle between Fullscreen and windowed mode" << endl;
    cout << " Hit ESC to quit" << endl;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )                   // try to initialize SDL video module
    {
        cerr << "Failed initializing SDL Video : " << SDL_GetError() << endl;      // report error if it fails
        return 1;                                          // we use this instead of Quit because SDL isn't yet initialized
    }

    // Set up the format for the pixels of the OpenGL drawing surface
    SetupPixelFormat();    
    // Create our window, we pass caption for the window, the width, height and video flags required
    CreateMyWindow("www.GameTutorials.com - First OpenGL Program", SCREEN_WIDTH, SCREEN_HEIGHT, VideoFlags);

    // Initializes our OpenGL drawing surface
    Init();

    // Run our message loop
    MainLoop();

    // quit main returning success
    return 0;
}

///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                      // Reset The View
    
      //     Position      View     Up Vector
    gluLookAt(0, 0, 6,     0, 0, 0,     0, 1, 0);    // This determines where the camera's position and view is

    // The position has an X Y and Z.  Right now, we are standing at (0, 0, 6)
    // The view also has an X Y and Z.  We are looking at the center of the axis (0, 0, 0)
    // The up vector is 3D too, so it has an X Y and Z.  We say that up is (0, 1, 0)
    // Unless you are making a game like Descent(TM), the up vector can stay the same.

                              // Below we say that we want to draw triangles    
    glBegin (GL_TRIANGLES);                // This is our BEGIN to draw
            glVertex3f(0, 1, 0);        // Here is the top point of the triangle
      glVertex3f(-1, 0, 0);  glVertex3f(1, 0, 0);  // Here are the left and right points of the triangle
    glEnd();                      // This is the END of drawing

    // I arranged the functions like that in code so you could visualize better
    // where they will be on the screen.  Usually they would each be on their own line
    // The code above draws a triangle to those points and fills it in.
    // You can have as many points inside the BEGIN and END, but it must be in three's.
    // Try GL_LINES or GL_QUADS.  Lines are done in 2's and Quads done in 4's.

    SDL_GL_SwapBuffers();                                  // Swap the backbuffers to the foreground
}
//////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////      This will shutdown SDL and quit the program
//////
//////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Quit(int ret_val)
{
    SDL_Quit();                                            // shuts down SDL stuff

    exit(ret_val);                                         // quit the program
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// That's all there is to initializing openGL!
// Now that you can display a triangle to the screen,
// you can go onto many other cool things from here.
// A lot of this (most of it) might be confusing the first time.
// The only stuff you should REALLY understand is the RenderScene() function.
// This function is what draws everything.  The rest of the stuff is init junk
// that you will very rarely ever look at again.  You might add a line here
// and there, but mostly it stays the same.  In the next tutorials, we will
// eventually just stick it in another .cpp file so we can just focus on the
// important stuff that we will most likely be changing a lot.  This will take
// the stress of being overwhelmed from the other stuff.  Like I said,
// if you can understand that you have a camera, and it points in a direction,
// and that you plot points to make a triangle, you are good for now :)
//
// Here are the basic steps to initializing OpenGL.
//
// 1) If you want to go full screen, give the option too.  Granted, the Full
//    screen code doesn't have anything to do with OpenGL, but I would always
//    encourage giving a full screen option.  It goes faster when it's in full
//    screen mode which helps out on slower computers.
//
// 2) Create our window with the full screen information.  We need to check if
//    the user wants full screen first because our window properties depend on it.
//
// 3) Next, we need to setup the pixel format.  This allows us to use openGL in 
//    our window.  We can also specify double buffering, if we want to draw to the
//    window, or a bitmap, and if we want to allow GDI function calls.  This is
//    necessary if we want to do anything with OpenGL.  You will most likely never
//    need to change the code in SetupPixelFormat().  It's a one time coded thing :)
//
// 4) Lastly, we need to setup our screen translations.  If we didn't use OpenGL 
//    this part would be complicated and tricky.  Luckily, OpenGL keeps us from
//      all the math needed to do the translations.  We just tell OpenGL how large
//    our viewport is (usually the whole screen) and which perspective we want to look
//      at out world. After that, we are all set and we just need to DRAW SOMETHING!
//
// We plotted 3 points in this tutorial.  3 points make up a triangle.
// We used the GL_TRIANGLES flag to tell OpenGL to render triangles.
// 
// Like so (cleaner):
//
//    glBegin (GL_TRIANGLES);    
//        glVertex3f(0, 1, 0);    // Top point            
//        glVertex3f(-1, 0, 0);    // Bottom right point
//        glVertex3f(1, 0, 0);    // Bottom left point
//    glEnd();
//
//  If we wanted 2 triangles, we could say:
//
//    glBegin (GL_TRIANGLES);    
//        glVertex3f(0, 1, 0);    // Top point            
//        glVertex3f(-1, 0, 0);    // Bottom right point
//        glVertex3f(1, 0, 0);    // Bottom left point
//
//        glVertex3f(0, 1, 1);    // Top point            
//        glVertex3f(-1, 0, 1);    // Bottom right point
//        glVertex3f(1, 0, 1);    // Bottom left point
//    glEnd();
//
//  Here is a horrible attempt to draw the 3D axis's
//
//                Y
//                |
//                |
//                |________ X
//                 \
//                   \
//                    Z
//
//  This is drawn in a rotated manner so you can see the Z axis.
//  Otherwise it would be coming out of the screen and you couldn't see it.
//  Positive Z comes out of the screen towards us, where -z goes into the screen
//  Positive X is on the right, negative X is on the left
//  Positive Y goes straight up, where negative Y goes straight down
//
//  So here is what our triangle looks like with the axis
//                Y
//                |
//               /|\
//              /_|_\______ X
//                 \
//                   \
//                    Z
//
// As you can see, our triangle is half on the negative X side 
// and half on the positive X side.  It is also 0 on the Z axis,
// so really, it's really seen as 2D more than 3D because we
// aren't using the third dimension, which is Z.
//
// The next tutorial will be a lot smaller in comments because I will
// chop off all the stuff that I already explain, so then we can just focus
// on the RenderScene() function, rather than all the Init stuff. (Blah!)
//
// Once again, if it seems overwhelming.. it will get easier and MUCH more fun! 
//
// As for messing around with this tutorial, try changing camera positions
// to see how it effects your view.  Also, try plotting in other points and
// other triangles/lines/Quads (rectangles) to see what you can create so far.
//
// One last note about this code.  It might be a good idea, if you don't
// understand anything about 3D or matrices (matrix math) to go on the internet
// And do a little investigating and learning.  It couldn't hurt.  I will teach you
// all the math you will need to know, but the more versed you are in math, the better.
// If you are not good at math, or at least you don't know anything over algebra/geometry,
// you are going to get a little frustrated.  You will want to be pretty comfortable
// with cosine and sine, as well as plane and vector math.  You will want to know what
// a vector is, and what purpose it serves.  On my site I will most likely have a page
// dedicated to teaching 3D concepts, but if not when you read this, use the internet.
//
// Don't get scared if you are not a math person, I wasn't really when I first started
// doing 3D programming, but you learn REALLY fast, and it's fun when you use math
// to do something cool.  Yah, you heard me, math can be fun!
//
// 3D programming is by FAR the coolest thing to program.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
