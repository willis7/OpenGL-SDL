//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        DigiBen        DigiBen@GameTutorials.com       //
//                                                                       //
//        $Program:        Camera5    (strafing)                         //
//                                                                       //
//        $Description:    Demonstrates camera strafing right and left   //
//                                                                       //
//        $Date:            1/1/02                                       //
//                                                                       //
//***********************************************************************//

#include "main.h"                                       // This includes our header file
#include "Camera.h"

CCamera g_Camera;                                       // Define our camera object

///////////////////////////////////////////////////////////////////////////////////////
//
// The code in this tutorial was taken from the Camera4 tutorial.  This is our last camera 
// tutorial.  We might create a bezier curve camera tutorial though in the future.  This 
// tutorial shows how to strafe the camera right or left.  
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
/////    This creates a pyramid with the center being (X, Y, Z).
/////   The width and height depend on the WIDTH and HEIGHT passed in
/////
///////////////////////////////// CREATE PYRAMID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreatePyramid(float x, float y, float z, int width, int height)
{
    // Below we create a pyramid (hence CreatePyramid() :).  This entails
    // 4 triangles for the sides and one QUAD for the bottom.  We could have done
    // 2 triangles instead of a QUAD but it's less code and seemed appropriate.
    // We also assign some different colors to each vertex to add better visibility.
    // The pyramid will be centered around (x, y, z).  This code is also used in the
    // Lighting and Fog tutorial on our site at www.GameTutorials.com.

    // Start rendering the 4 triangles for the sides
    glBegin(GL_TRIANGLES);        
        
        // These vertices create the Back Side
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                  // Top point
        glColor3ub(0, 255, 255); glVertex3f(x - width, y - height, z - width);  // Bottom left point
        glColor3ub(255, 0, 255); glVertex3f(x + width, y - height, z - width);  // Bottom right point

        // These vertices create the Front Side
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                  // Top point
        glColor3ub(0, 255, 255); glVertex3f(x + width, y - height, z + width);  // Bottom right point
        glColor3ub(255, 0, 255); glVertex3f(x - width, y - height, z + width);  // Bottom left point

        // These vertices create the Front Left Side
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                  // Top point
        glColor3ub(255, 0, 255); glVertex3f(x - width, y - height, z + width);  // Front bottom point
        glColor3ub(0, 255, 255); glVertex3f(x - width, y - height, z - width);  // Bottom back point

        // These vertices create the Front right Side
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                  // Top point
        glColor3ub(255, 0, 255); glVertex3f(x + width, y - height, z - width);  // Bottom back point
        glColor3ub(0, 255, 255); glVertex3f(x + width, y - height, z + width);  // Front bottom point
            
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
/////    This function create a simple grid to get a better view of the animation
/////
///////////////////////////////// DRAW 3D GRID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Draw3DSGrid()
{
    // This function was added to give a better feeling of moving around.
    // A black background just doesn't give it to ya :)  We just draw 100
    // green lines vertical and horizontal along the X and Z axis.

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


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// DRAW SPIRAL TOWERS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This draws a spiral of pyramids to maneuver around
/////
///////////////////////////////// DRAW SPIRAL TOWERS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DrawSpiralTowers()
{
    const float PI = 3.14159f;                            // Create a constant for PI
    const float kIncrease = PI / 16.0f;                   // This is the angle we increase by in radians
    const float kMaxRotation = PI * 6;                    // This is 1080 degrees of rotation in radians (3 circles)
    float radius = 40;                                    // We start with a radius of 40 and decrease towards the center

    // Keep looping until we go past the max degrees of rotation (which is 3 full rotations)
    for(float degree = 0; degree < kMaxRotation; degree += kIncrease)
    {
        // Here we use polar coordinates for the rotations, but we swap the y with the z
        float x = float(radius * cos(degree));            // Get the x position for the current rotation and radius
        float z = float(radius * sin(degree));            // Get the z position for the current rotation and radius

        // Create a pyramid for every degree in our spiral with a width of 1 and height of 3 
        CreatePyramid(x, 3, z, 1, 3);
    
        // Decrease the radius by the constant amount so the pyramids spirals towards the center
        radius -= 40 / (kMaxRotation / kIncrease);
    }    
}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                    // Reset The matrix


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Instead of calling gluLookAt(), we just call our Look() function from
    // the camera object.  This makes it more object oriented, along with
    // cleaning up RenderScene().  If we were building an engine, you would
    // not want to ever explicitely call a specific graphics API.  This allows
    // you to not have to change client code, but only the code underneath
    // when a new API is desired for rendering, or a code addition/bug fix.
    g_Camera.Look();

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


    // Draw a grid so we can get a good idea of movement around the world        
    Draw3DSGrid();


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Draw the pyramids that spiral in to the center
    DrawSpiralTowers();

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


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
    bool done = false;                                   // is our job done ? not yet !
    SDL_Event event;

    while(! done)                                        // as long as our job's not done
    {
        while( SDL_PollEvent(& event) )                  // look for events (like keystrokes, resizing etc.)
        {
            switch ( event.type )                        // what kind of event have we got ?
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
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
    
            // We now just need to call Update() from the camera object to
            // deal with the input and updating.

            g_Camera.Update();                            // Update the camera information
            RenderScene();                                // Render the scene every frame

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
    } // while( ! done)
}

/////////////////////////////////////////////////////////////////////////////////
//
//
// * QUICK NOTES * 
//
// This tutorial was taken from the Camera4 tutorial.  This is our last camera tutorial.
// We might create a bezier curve camera tutorial though in the future.  This tutorial
// shows how to strafe the camera right or left.  This might seem easy when you think to
// attempt it, but if you don't know your linear algebra it can be tricky.  Strafing the
// camera is moving the camera 90 degrees left or right from the current view.  In other
// words, it's as if you were side stepping while you look forward.  This is used in most
// first person shooters games, and comes in handy when peering around corners or running
// past a hallway while firing off some rounds.  It's also a great way to move diagonal
// while doing a shootout at close ranges to avoid being hit.
//
// Since we understand what strafing is and what it does, let me actually explain how it works.
// We know that we want to walk in the direction that is 90 degrees from the view vector (with
// the view vector being m_vView - m_vPosition).  So how do we then get the vector that is 90
// degrees from our view vector?  If you know what the cross product is, you can easily see how
// this would be done.  The cross product is a mathematical formula that takes 2 vectors and
// returns the vector 90 degrees from those 2 vectors.  This is how you find the normal to a plane.
// Well, we have a view vector, but what would the other vector be?  Does the up vector come to mind?
// That's it!  We want to take the cross product between the up vector and the view vector.  This
// will return the vector (or direction) that we want to strafe in.  In games like Descent, the
// up vector will change because you can go upside down and spin in crazy directions.  The cross
// product will ensure that we will always strafe correctly no matter what orientation the camera is in.
// Once we have the strafe vector, we need to add it to the position and view points.
// Here are the controls for this tutorial:
//
// w, s, UP_ARROW, DOWN_ARROW - Move the camera forward and backward
// a, d, RIGHT_ARROW, LEFT_ARROW - Strafe the camera left and right
// Mouse movement - Moves the view for first person mode
// ESCAPE - Quits the program
//
// You may notice that we don't use all the methods in the camera class.  I decided to leave
// them in there because since this is the last camera tutorial it would be nice to just use
// this in your projects without having to cut and paste code.  That's also why I finally
// added a camera.h so you don't have the camera class in main.h.
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
