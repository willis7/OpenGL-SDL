//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     DigiBen@GameTutorials.com            //
//                                                                       //
//      $Program:       Camera4                                          //
//                                                                       //
//      $Description:   Rotates the camera in 3rd person mode            //
//                                                                       //
//      $Date:          11/1/01                                          //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file

#define kSpeed  0.03f                                   // This is how fast our camera moves

CCamera g_Camera;                                       // Define our camera object

// This tutorial was taken from the Camera2 tutorial.  So far we had only first
// person camera tutorials, but now we move into the realm of 3rd person views.
// Some games that use this view are "Metal Gear Solid", "Tomb Raider", 
// "My Little Pony's Revenge" (okay.. I made that up).  This can be a tricky
// mode for a game because the feel and movement can be uncomfortable if you
// don't get it just right.  Many people complain about this view.  Another bad
// thing about this view is that it makes you render your character all the time
// which can slow down the frame rate.  In first person shooters you only have
// to render the people you come in contact with, which saves a bit of frame rate.
// A cool thing about this mode is that it let's you see what your character looks
// like and how they react to objects and characters/monsters in the world.  This
// tutorial adds very little code to the second camera tutorial to accomplish this
// result.  All we really did was add the function RotateAroundPoint() to our CCamera class.
// This rotates the camera around a certain point.  What point wout that be?  Well, you
// usually want it to be your m_vView point (the place you are looking).  You can then
// set the position of your character at that point as well, which we do.  You might not
// always want to rotate around the view point, so we allow you to pass in your own
// center point to RotateAroundPoint().  This tutorial just creates a simple object
// in a simple world that moves around with you in third person.  The object would be
// replaced with your character, but for the sake of simplicity we create something easy.
// Use the arrow keys to move around.  You will notice with you hit the left or right arrow
// keys the camera will rotate around the object.  Just imagine that is your character.
// This tutorial doesn't do the full full third person view like in Tomb Raider, but
// if you understand these basics, you can finish the rest.


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

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// CREATE PYRAMID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This creates a pyramid with the center being (X, Y, Z).
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
/////   This function create a simple grid to get a better view of the animation
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



///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                   // Reset The matrix

    // Give openGL our camera position, then camera view, then camera up vector
    gluLookAt(g_Camera.m_vPosition.x, g_Camera.m_vPosition.y,  g_Camera.m_vPosition.z,  
              g_Camera.m_vView.x,     g_Camera.m_vView.y,      g_Camera.m_vView.z,  
              g_Camera.m_vUpVector.x, g_Camera.m_vUpVector.y,  g_Camera.m_vUpVector.z);


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

    // Draw a grid so we can get a good idea of movement around the world       
    Draw3DSGrid();

    // Below we create 4 pyramids which will just be objects in the world
    // to orient us.  Nothing special, but I figured I should add them since
    // it was only 4 lines of extra code and they act as pillars.

    // Create a pyramid at (-6, 3, 6) with a width and height of 1 and 6
    CreatePyramid(-6, 3, 6, 1, 6);

    // Create a pyramid at (-6, 3, 6) with a width and height of 1 and 6
    CreatePyramid(6, 3, 6, 1, 6);

    // Create a pyramid at (-6, 3, 6) with a width and height of 1 and 6
    CreatePyramid(6, 3, -6, 1, 6);

    // Create a pyramid at (-6, 3, 6) with a width and height of 1 and 6
    CreatePyramid(-6, 3, -6, 1, 6);

    // Translate the object represented as our character to our view position.
    // This way we will always be looking at the object and it will follow us
    // where ever we go.
    glTranslatef(g_Camera.m_vView.x, 0, g_Camera.m_vView.z);

    // Now we create our object.  It's simply 2 pyramids with one turned upside down.
    // This will always be in the center of our screen because we keep setting it's
    // position to be where we are looking.

    // Create a pyramid at (0, 2, 0) with a width and height of 1
    CreatePyramid(0, 2, 0, 1, 1);

    // Before we create the other pyramid that makes up our object, 
    // we rotate it 180 degrees around the X axis which turns it upside down.
    glRotatef(180, 1, 0, 0);

    // Create a pyramid at (0, -1, 0) with a width and height of 1
    CreatePyramid(0, -1, 0, 1, 1);

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


    SDL_GL_SwapBuffers();                                   // Swap the backbuffers to the foreground
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
        // Here we check for mouse and key movements every frame
        keyPressed();                               // Check if we pressed a key
        RenderScene();                              // Update the screen    
    } // while( ! done)
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// In this tutorial we learned a simple way to make the camera rotate with the mouse.
// There is 2 more things that we are missing with cameras and that is straffing, and
// camera paths.  See if you can make the camera strafe left or right.  Since you should
// now know about the CROSS product, you can calculate your strafing vector to be applied
// to the current position and current view.  
// 
// The main functions in this file changed are MainLoop(), CheckForMovement(), and RenderScene()..
// Not to much coded aded to this tutorial, but still a neat one that is a must for any first
// person view.
// 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//

