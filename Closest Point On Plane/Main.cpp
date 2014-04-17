//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen         DigiBen@GameTutorials.com        //
//                                                                       //
//      $Program:       ClosestPointOnLine                               //
//                                                                       //
//      $Description:   Finds closest point on a line to another point   //
//                                                                       //
//      $Date:          2/23/02                                          //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
//
// This tutorial demonstrates how to find the closest point on a line segment, to a
// given point out in space.  You would use this algorithm for collision detection.
// In the Sphere Polygon Collision tutorial you can see how this is implemented.
// When running this program, you will see a green sphere that represents the point,
// along with a line segment and another blue sphere to represent the closest point
// on the line to the given point.  You are able to move the sphere and camera around
// with the following keys:
//
// LEFT ARROW - Moves the sphere left along it's current plane
// RIGHT ARROW - Moves the sphere right along it's current plane
// UP ARROW - Moves the sphere up along it's current plane
// DOWN ARROW - Moves the sphere down along it's current plane
// F3 - Moves the sphere towards the front of the line's plane
// F4 - Moves the sphere towards the back of the liness plane
// 
// F1 - Rotates the camera left
// F2 - Rotates the camera right
// ESC - Quits the program
//
// It is assumed that by reading this tutorial you understand the last few tutorials on
// normals and the dot product.  This was written in preparation for the Sphere Polygon
// Collision tutorial.  It is also assumed that you have looked at the quadric tutorial.
// This is because we use quadric spheres to simplify the tutorial and cut down code.
//
//

// This is the array of 2 vertices that will hold our line segment coordinates      
CVector3 g_vLine[2];

// This is the center of our sphere.  We are able to move it with the arrow keys.
CVector3 g_vPosition;

// This is the current rotation of our camera (F1 and F2 keys)
float g_rotateY = 0;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeGL(SCREEN_WIDTH, SCREEN_HEIGHT);    // Init OpenGL with the global rect
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
    
    // Initialize our line segment along the X axis
    g_vLine[0] = CVector3(-1,  0,   0);
    g_vLine[1] = CVector3( 1,  0,   0);

    // Initialize our position of the sphere, which will be used as the given point
    g_vPosition = CVector3(0, 0.5f, 0);

    // Here, we turn on a light and enable lighting.  We don't need to
    // set anything else for lighting because we will just take the defaults.
    // We also want color, so we turn that on too.  This gives the spheres a bit
    // of dimension.

    glEnable(GL_LIGHT0);                                // Turn on a light with defaults set
    glEnable(GL_LIGHTING);                              // Turn on lighting
    glEnable(GL_COLOR_MATERIAL);                        // Allow color

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


    // Enable Key Repeat
    if( SDL_EnableKeyRepeat(100,SDL_DEFAULT_REPEAT_INTERVAL) )
    {
        cerr << "Failed enabling key repeat" << endl;
        Quit(1);
    }
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

        RenderScene();                                      // draw our OpenGL scene
    } // while( ! done)
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


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Set our camera to the left a bit for a nice side view of the left hand side

        //      Position          View       Up Vector
    gluLookAt(0, 1, -2,  0, 0, 0,   0, 1, 0);           // This determines where the camera's position and view is

    // Rotate the camera around our world for a 360 degree view of what is going on.
    // Yes I know... we are actually rotating the whole world, not the camera :)
    glRotatef(g_rotateY, 0, 1, 0);

    // We need a radius for our spheres, so let's create it here with 0.05f
    float radius  = 0.05f;
    float radius2 = 0.01f;
        
    // This sets our line width to %250, since 1 is %100 for normal line width
    glLineWidth(2.5f);

    // Turn of lighting so our line isn't effected by it
    glDisable(GL_LIGHTING);
    
    // Draw the line segment

    glBegin (GL_LINES);                                 // This is our BEGIN to draw

        glColor3ub(255, 0, 0);                          // Make the Left vertex RED
        glVertex3f(g_vLine[0].x, g_vLine[0].y, g_vLine[0].z);

        glColor3ub(0, 255, 0);                          // Make the Right vertex GREEN
        glVertex3f(g_vLine[1].x, g_vLine[1].y, g_vLine[1].z);
    glEnd();                                            // This is the END of drawing

    glEnable(GL_LIGHTING);                              // Turn lighting back on

    // Instead of calculating the sphere ourselves, we are going to use quadrics.
    // Check out the tutorial on quadrics if this is confusing for you.

    // Allocate a quadric object to use as a sphere
    GLUquadricObj *pObj = gluNewQuadric();              // Get a Quadric off the stack

    // Below we create a new matrix so that when we translate the first sphere,
    // it doesn't effect the other sphere that we need to translate.

    glPushMatrix();                                     // Push on a new matrix

        // Move the sphere to it's center position
        glTranslatef(g_vPosition.x, g_vPosition.y, g_vPosition.z);
        
        // Draw the quadric as a sphere with the radius of .05 and a 15 by 15 detail.
        // To increase the detail of the sphere, just increase the 2 last parameters.
        gluSphere(pObj, radius, 15, 15);
    
    glPopMatrix();                                      // Pop off the current matrix

    // Now we need to find the closest point on the line.  We pass in the end points
    // of the line segment, and the point we want to use, which is the sphere's center position.
    // This returns the closest point to use for the center of the purple sphere.
    CVector3 vClosestPoint = ClosestPointOnLine(g_vLine[0], g_vLine[1], g_vPosition);

    // Move the closest point sphere to it's center position
    glTranslatef(vClosestPoint.x, vClosestPoint.y, vClosestPoint.z);

    // Turn the line segment sphere purple
    glColor3f(0.8f, 0, 1);

    // Draw the quadric as a sphere with the radius of .01 and a 15 by 15 detail.
    gluSphere(pObj, radius2, 15, 15);

    gluDeleteQuadric(pObj);                             // Free the Quadric

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


    SDL_GL_SwapBuffers();                                   // Swap the backbuffers to the foreground
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This tutorial is intended to be a preparation to the Sphere Polygon Collision tutorial.
// This is because we need to use this technique when doing our sphere polygon collision.
// 
// Our rendering function works like this:  First, we draw the line, then we draw the
// sphere representing the point, then we calculate the closest point on the line,
// then finally draw the closest point as a small sphere.  You can move the sphere
// around by using the appropriate keys.  You will see the sphere on the line
// move along, continually updated the closest point, depending on the position of the sphere.
//
// Here is a snippet from 3DMath.h that briefly explains how the math works:
// 
// The dot product is the key function that makes all this magic work.  Basically,
// if we get a vector between an end point of the line segment and the point, plus
// a normalized directional vector of that end point to the other end point, we can
// use the dot product to project the first vector onto the directional vector.  This
// gives us a distance from the new project vector to the end point.  If that distance
// is 0, the 2 vectors were perpendicular and the projected vector is directory on top
// of the end point, which would make the distance 0.  If the distance is negative, the
// point is on the outside of the line segment and closest to that end point.  If the
// distance is positive, we know that the point is on the other side of the end point.
// If we find the distance of the line segment, we can then check to see if the point
// is closest to a point on the line, or it is past the other end point, which makes that
// other end point the closest point to our given point.  To check the closest point
// is inside the line and not an end point, we just make sure that the original dot product
// result is less than the distance of the line segment. This distance can be calculated 
// using the normal distance formula you use in algebra.  
// 
// So to sum it all up... if the distance "t" is less than 0, our first end point is the
// closest point on the line, where if the distance "t" is greater than the distance of
// the line, distance "d", then it's the other end point.  If both of those fail, it must
// be on the line between the 2 end points.  To calculate where, we multiply our directional
// vector times the result of the dot product.  Then add that result to our original end point.
// That will give us the closest point on the line, between the 2 end points.
// 
// Now, move on to the next tutorial that talks about Sphere Polygon Collision.  This
// tutorial was created to help you better understand and prepare you for that collision tutorial. 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
