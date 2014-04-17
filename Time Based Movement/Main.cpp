//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     DigiBen@GameTutorials.com            //
//                                                                       //
//      $Program:       TimeBasedMovement                                //
//                                                                       //
//      $Description:   Demonstrates camera movement in regards to time  //
//                                                                       //
//      $Date:          4/30/02                                          //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file
#include "Camera.h"

CCamera g_Camera;                                       // Define our camera object

///////////////////////////////////////////////////////////////////////////////////////
//
// The code in this tutorial was taken from the Camera5 tutorial.  The point of this
// tutorial is to show how to make time based movement with the camera.  This means
// that the camera speed will be the same, no matter how fast the person's machine
// or video card is.  Granted, that the frames might render faster, but the player
// will not get from point A to B faster than the next guy.  Without doing time
// based movement, and doing just frame based movement, a person with a Geforce2
// will go incredibly faster than a person running a TNT2 in the level.  Some might
// try and lock their frame rate to compensate for this, but this doesn't always work.
// So spots might have more polygons that are being rendered and might make the player
// go slower on those parts, yet faster on low poly parts of the level.  
// 
// This feature is a MUST in any engine.  In the tutorial, we mixed the FPS
// and Camera5 tutorial together.  That way you don't have to sift through some new
// code to pick out the few lines of that need to be added to make this feature happen.
// I will only mark these before mentioned lines as NEW so you can pick them out easier.
// Basically, all that's going on is that we are saving the time elapsed between each
// frame, then multiplying that interval by our desired speed.
//
// Notice that we did changed the colors of the pyramids, just to give it a new look.
//
//


///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeGL(SCREEN_WIDTH, SCREEN_HEIGHT);    // Init OpenGL with the global rect
    SDL_WM_GrabInput(SDL_GRAB_ON);

    if ( SDL_ShowCursor(SDL_DISABLE) != SDL_DISABLE )
              cerr << SDL_GetError() << endl;
    
    // Enable Key Repeat
    if( SDL_EnableKeyRepeat(100,SDL_DEFAULT_REPEAT_INTERVAL) )
    {
        cerr << "Failed enabling key repeat" << endl;
        Quit(1);
    }

    // *Hint* We will put all our game init stuff here
    // Some things include loading models, textures and network initialization
                         //Position      View          Up Vector
    g_Camera.PositionCamera(0, 1.5, 6,   0, 1.5, 0,   0, 1, 0 );    
}

///////////////////////////////// CREATE PYRAMID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This creates a pyramid with the center being (X, Y, Z).
/////   The width and height depend on the WIDTH and HEIGHT passed in
/////
///////////////////////////////// CREATE PYRAMID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreatePyramid(float x, float y, float z, int width, int height)
{
    // Start rendering the 4 triangles for the sides
    glBegin(GL_TRIANGLES);      
        
        // These vertices create the Back Side
        glColor3ub(255, 255, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(255, 255, 255); glVertex3f(x - width, y - height, z - width);    // Bottom left point
        glColor3ub(255, 0, 0); glVertex3f(x + width, y - height, z - width);  // Bottom right point

        // These vertices create the Front Side
        glColor3ub(255, 255, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(255, 255, 255); glVertex3f(x + width, y - height, z + width);  // Bottom right point
        glColor3ub(255, 0, 0); glVertex3f(x - width, y - height, z + width);    // Bottom left point

        // These vertices create the Front Left Side
        glColor3ub(255, 255, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(255, 0, 0); glVertex3f(x - width, y - height, z + width);    // Front bottom point
        glColor3ub(255, 255, 255); glVertex3f(x - width, y - height, z - width);    // Bottom back point

        // These vertices create the Front right Side
        glColor3ub(255, 255, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(255, 0, 0); glVertex3f(x + width, y - height, z - width);    // Bottom back point
        glColor3ub(255, 255, 255); glVertex3f(x + width, y - height, z + width);    // Front bottom point
            
    glEnd();

    // Now render the bottom of our pyramid

    glBegin(GL_QUADS);

        // These vertices create the bottom of the pyramid
        glColor3ub(0, 0, 255); glVertex3f(x - width, y - height, z + width);    // Front left point
        glColor3ub(0, 0, 255); glVertex3f(x + width, y - height, z + width);    // Front right point
        glColor3ub(0, 0, 255); glVertex3f(x + width, y - height, z - width);    // Back right point
        glColor3ub(0, 0, 255); glVertex3f(x - width, y - height, z - width);    // Back left point
    glEnd();
}


///////////////////////////////// DRAW 3D GRID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function create a simple grid to get a better view of the animation
/////
///////////////////////////////// DRAW 3D GRID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Draw3DSGrid()
{
    // Turn the lines GREEN
    glColor3ub(0, 255, 0);

    // Draw a 1x1 grid along the X and Z axis'
    for(float i = -50; i <= 50; i += 1)
    {
        // Start drawing some lines
        glBegin(GL_LINES);

            // Do the horizontal lines (along the X)
            glVertex3f(-50, 0, i);
            glVertex3f(50, 0, i);

            // Do the vertical lines (along the Z)
            glVertex3f(i, 0, -50);
            glVertex3f(i, 0, 50);

        // Stop drawing lines
        glEnd();
    }
}


///////////////////////////////// DRAW SPIRAL TOWERS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This draws a spiral of pyramids to maneuver around
/////
///////////////////////////////// DRAW SPIRAL TOWERS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DrawSpiralTowers()
{
    const float PI = 3.14159f;                          // Create a constant for PI
    const float kIncrease = PI / 16.0f;                 // This is the angle we increase by in radians
    const float kMaxRotation = PI * 6;                  // This is 1080 degrees of rotation in radians (3 circles)
    float radius = 40;                                  // We start with a radius of 40 and decrease towards the center

    // Keep looping until we go past the max degrees of rotation (which is 3 full rotations)
    for(float degree = 0; degree < kMaxRotation; degree += kIncrease)
    {
        // Here we use polar coordinates for the rotations, but we swap the y with the z
        float x = float(radius * cos(degree));          // Get the x position for the current rotation and radius
        float z = float(radius * sin(degree));          // Get the z position for the current rotation and radius

        // Create a pyramid for every degree in our spiral with a width of 1 and height of 3 
        CreatePyramid(x, 3, z, 1, 3);
    
        // Decrease the radius by the constant amount so the pyramids spirals towards the center
        radius -= 40 / (kMaxRotation / kIncrease);
    }   
}


///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                   // Reset The matrix

    // Tell OpenGL where to point the camera
    g_Camera.Look();

    // Draw a grid so we can get a good idea of movement around the world       
    Draw3DSGrid();

    // Draw the pyramids that spiral in to the center
    DrawSpiralTowers();

    // Swap the backbuffers to the foreground
    SDL_GL_SwapBuffers();   
}

////////////////////////////////   MAIN GAME LOOP   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
////////
////////   This function handles the main game loop
////////
////////////////////////////////   MAIN GAME LOOP   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

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
                case SDL_QUIT :                                         // if user wishes to quit
                    done = true;                                        // this implies our job is done
                    break;

                case SDL_KEYDOWN :                                      // if the user has pressed a key
                    HandleKeyPressEvent( & event. key.keysym );         // callback for handling keystrokes, arg is key pressed
                    break;
                    
                case SDL_KEYUP :
                    HandleKeyReleaseEvent(& event.key.keysym) ;           // callback for handling keystrokes, arg is key released
                    break;

                case SDL_VIDEORESIZE :                                  // if there is a resize event
                    // request SDL to resize the window to the size and depth etc. that we specify
                    MainWindow = SDL_SetVideoMode( event.resize.w, event.resize.h, SCREEN_DEPTH, VideoFlags );
                    SizeOpenGLScreen(event.resize.w, event.resize.h);   // now resize the OpenGL viewport
    
                    if(MainWindow == NULL)                              // if window resize has failed
                    {
                        cerr << " Failed resizing SDL window : " << SDL_GetError() << endl;   // report error
                        Quit(0);
                    }
                    break;

                default:                                    // any other event
                    break;                                  // nothing to do
            } // switch
        } // while( SDL_ ...
        g_Camera.Update();                          // Update the camera information
        RenderScene();                              // Render the scene every frame
    } // while( ! done)
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Simple Huh!!!  This is probably one of the simplest tutorials in 3D we have.
// Though it might seem complicated to wrap your head around at first, it's
// only a few extra lines of code to add.  Now you guys have no excuse for frame
// based movement in your demos/games :)
// 
// The basic premise is, calculate the time (in seconds) between each frame,
// then multiply that elapsed time by your speed that you desire.  Try it! 
//
// This tutorial includes a new header file, <mmsystem.h>. This is so we can use
// the timeGetTime() function instead of GetTickCount().  Apparently, GetTickCount()
// doesn't seem to update as fast as timeGetTime().  What you really should use
// is QueryPerformanceCounter() and QueryPerformanceFrequency().  These seem to
// be the fastest method to get time from your CPU.
//
// Enjoy!
// 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
