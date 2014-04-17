//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen         DigiBen@GameTutorials.com        //
//                                                                       //
//      $Program:       Normals                                          //
//                                                                       //
//      $Description:   This shows how to find the normal of a polygon.  //
//                                                                       //
//      $Date:          7/8/01                                           //
//                                                                       //
//***********************************************************************//



#include "main.h"                                        // This includes our header file

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


// This part below is important.  We create an array of 3 CVector3's.
// We could have called it CPoint3 or CVertex3, but let's call it it CVector3.
// A CVector3 is a class that has 3 floats, x, y, and z.  We will need a class
// to hold 3D points from now on.  We chose to use a class instead of a structure
// because later we will add to this class and make it a robust vector class, but
// for now we just want 3 floats.  We will store our triangle points in this array below.
// We put a 'v' in front of 'Triangle' to show that it is of type CVector3.
// Notice that we initialize the triangle with the left point first, then right, then the top.
// This is important.  We need to stick to this order.  This is important.  I would recommend
// doing it this way, which is counter-clockwise (considering we are looking at (0, 0, 0) down
// the negative Z axis.  This will make sure that our normals are consistent.  When backface
// culling, the default order is counter-clockwise.  This was just picked as a standard.  
// You can change this in OpenGL if you want though.  Check our glFrontFace() and glCullFace()
// in MSDN for more information on back face culling and how to use it.
// I labeled the points below with an '*' to give you more of a visual understanding of the poly.

//                                                 *
//                              *                                   *

                          // Left Point     // Right Point   // Top point
CVector3 vTriangle[3] = { {-1,  0,   0},    {1,    0,   0},  {0,   1,   0} };

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeGL(SCREEN_WIDTH, SCREEN_HEIGHT);    // Init OpenGL with the global rect

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
    
    // We changed the position of the camera so we could better see the normal at an angle. 
    // This positions the camera off to the left a bit and in front of the triangle.
    // We move our view up a bit so fully see the triangle instead looking down

        //      Position          View       Up Vector
    gluLookAt(-2.5f, 0, -0.5f,  0, 0.5f, 0,   0, 1, 0); // This determines where the camera's position and view is

    
    // Below we draw the triangle like normal, except we pass in our vTriangle structure.
    // This is so we can move the triangle around and now just have it constant.
    // As we move the triangle around, the "normal" will follow it perfectly.

    glBegin (GL_TRIANGLES);                             // This is our BEGIN to draw

        glColor3ub(255, 0, 0);                          // Make the Left vertex RED
        glVertex3f(vTriangle[0].x, vTriangle[0].y, vTriangle[0].z);

        glColor3ub(0, 255, 0);                          // Make the Right vertex GREEN
        glVertex3f(vTriangle[1].x, vTriangle[1].y, vTriangle[1].z);

        glColor3ub(0, 0, 255);                          // Make the Top vertex BLUE
        glVertex3f(vTriangle[2].x, vTriangle[2].y, vTriangle[2].z);
    glEnd();                                            // This is the END of drawing

    // Now that we displayed the triangle, let's display a line coming from the
    // triangle to show you visually what the normal looks like.  Remember, the
    // normal does NOT have a position, just a direction, but just to help us visualize it,
    // we will draw the normal coming from the middle of the triangle.

    // Get the normal of our triangle by passing in our array or points.
    CVector3 vNormal = Normal(vTriangle);

    // Now, just for show, let's find the center of the triangle.
    // We know that the middle X is zero, so we don't need to calculate the X value.
    CVector3 vCenter = {0};

    // To get the Y value, we just divide the top point's Y value by 2, since we start at 0.
    // Do the same thing for the Z value, just divide the Z value of the top point by 2
    // because we start out at 0 on the Z axis, so only the top of the triangle will be moving.
    // If we weren't so conveniently positioned we would have to do something more modular.
    // To find the center of an object you just add up all the X Y and Z values singularly,
    // then divide each one by the number of vertices, and that gives you the center.
    // IE, after you get the total:  total.x /= totalVertices;  total.y /= totalVertices; total.z /= totalVertices;
    // I decided not to make another function for this for simplicity reasons, but you should :)

    vCenter.y = vTriangle[2].y / 2;                     // Get the Y value for the center of the triangle
    vCenter.z = vTriangle[2].z / 2;                     // Get the Z value for the center of the triangle.

    // Now let's draw the line coming from the center of our triangle.
    // The first point will be the center point of the triangle that we just calculated.
    // The next point will be the center point + the normal of the triangle.

    glBegin (GL_LINES);                                 // This is our BEGIN to draw

        glColor3ub(255, 255, 0);                        // Make the normal line YELLOW
        glVertex3f(vCenter.x, vCenter.y, vCenter.z);    // Let's draw the normal centered on the triangle
                                                        // Draw the normal of the polygon from the center of the polygon to better see it
        glVertex3f(vCenter.x + vNormal.x, vCenter.y + vNormal.y, vCenter.z + vNormal.z);    
    glEnd();                                            // This is the END of drawing

    // That's it, now use the LEFT and RIGHT arrow keys to move it around to further see it in action.

    SDL_GL_SwapBuffers();                                   // Swap the backbuffers to the foreground
}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// We didn't do much code in this file, but we added all our math code to 3DMath.cpp.
// This will come in handy because you can just add the .cpp and .h file to the rest
// of your projects.  You will use this and a lot more math almost every app you do.
// At least, any app that actually DOES something besides test OpenGL API's.  If you
// plan to do any lighting or collision you will definitely need this stuff.
//
// If you run this tutorial you can use the LEFT and RIGHT arrow keys to move around
// the top of the triangle so you can see how the normal will follow the triangle.
// If you still don't understand what a normal is used for, go to the next tutorial
// on collision, and it will make more sense.  Also, look over the lighting tutorial. 
//
// Here is what is repeated in 3DMath.cpp:
// 
// Basically, let me explain the steps again to finding the normal of a polygon.
// 
// 1)  First, if you have 3 points of a polygon, you can find a normal to that plane.
//     The first step you need to do is get 2 vectors from that polygon.  That means
//     that you need to pick to sides of the triangle (order is important) and subtract
//     the points from each other to get the vector of that side.  When it comes to triangles,
//     I usually get the first vector from Point3 - Point1. (V1 = P3 - P1)
//     Next, I choose Vector 2 from Point2 and Point1 (V2 = P2 - P1).  
//
// 2)  Now that we have 2 vectors:  V1 and V2, we can take the cross product of those 
//     2 vectors, with vector1 cross vector2.  Remember, the order is important.  
//     I always go counter-clockwise.  Pick and stick!  This is important because it decides the
//     direction of the normal.  It doesn't matter which direction, as long as you do the same
//     thing for all the rest of your polygons.  You don't want mixed up normal directions.
// 
// 3)  Now we should have the normal (Or direction) from the cross product of our polygon.
//     This normal has a strange length, so let's make the length (or distance from the origin) 1.
//     To do this, we take the magnitude (Deals with the length of the vector) of the normal.
//     With the magnitude, we divide the normal BY that magnitude.  That means the X, Y and Z values
//     get divided by the magnitude scalar.  A scalar is a single number like 1, 1.232, etc...
// 
// That's it!  Three basic steps to finding the normal.  It is nice not to have to worry about
// all that once you have a function that does it all for you.  We call ours "Normal()".  It
// won't matter it we have a polygon of 20 vertices, all we need is just 3 points (the first 3)
// from that polygon.  That is enough to define a plane.  Then we find the normal to that plane.
//
// 
// I hope this help!  Let us know at www.GameTutorials.com if this helped you at all.
//
// Good luck, and get ready for the collision tutorial next!
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
