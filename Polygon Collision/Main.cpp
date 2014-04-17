//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen         DigiBen@GameTutorials.com        //
//                                                                       //
//      $Program:       PolygonCollision                                 //
//                                                                       //
//      $Description:   This demonstrates line and polygon collision.    //
//                                                                       //
//      $Date:          7/11/01                                          //
//                                                                       //
//***********************************************************************//


#include "main.h"                                        // This includes our header file

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

//  This tutorial is built from the "PlaneCollision" tutorial.  Instead of just
//  colliding with a plane, we now can do better collision that checks the whole polygon.
//  You wouldn't do much yet with just plane collision, but now you can actually make
//  somewhat of a decent maze or what ever you want.  There is a lot of math explained
//  in this tutorial, but hopefully it is explained enough so you understand what is going
//  on.  In the next tutorials it will be further explained, as well as online papers will
//  be written with pictures to give a better illustration.  We didn't change much in this file.

//                                                 *
//                              *                                   *

                          // Left Point     // Right point  // Top Point     
CVector3 vTriangle[3] = { {-1,  0,     0},  {1,    0,   0}, {0,   1,    0} };

// This is our line that we will be checking against the polygon's plane for collision.
// We position the line going directly through the polygon at first.
CVector3 vLine[2]     = { {0,   0.5f, -0.5f},  {0,   0.5f, 0.5f} };

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
    
    // Let's set our camera to the left a bit for a better view

        //      Position          View       Up Vector
    gluLookAt(-2.5f, 0.5, 0.5f,  0, 0.5f, 0,   0, 1, 0);    // This determines where the camera's position and view is

    
    // Below we give OpenGL the 3 vertices of our triangle.  Once again, we put them
    // into an array of CVector3 structures so we could dynamically move it around screen.

    glBegin (GL_TRIANGLES);                             // This is our BEGIN to draw

        glColor3ub(255, 0, 0);                          // Make the Left vertex RED
        glVertex3f(vTriangle[0].x, vTriangle[0].y, vTriangle[0].z);

        glColor3ub(0, 255, 0);                          // Make the Right vertex GREEN
        glVertex3f(vTriangle[1].x, vTriangle[1].y, vTriangle[1].z);

        glColor3ub(0, 0, 255);                          // Make the Top vertex BLUE
        glVertex3f(vTriangle[2].x, vTriangle[2].y, vTriangle[2].z);
    glEnd();                                            // This is the END of drawing

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Now, instead of just testing against the plane, we take it a step further
    // and test if we actually hit the polygon.  This is a more usable collision.
    // We give our function the polygon, the line to test with, and the number of vertices of our polygon
    
    bool bCollided = IntersectedPolygon(vTriangle, vLine, 3);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


    // Below we draw the line that the polygon will be colliding with.(Green is collision)

    glBegin (GL_LINES);                                 // This is our BEGIN to draw

        if(bCollided)                                   // If we collided, change the color of the line to illustrate this.
            glColor3ub(0, 255, 0);                      // Make the line GREEN if we collided with the triangle's plane
        else
            glColor3ub(255, 255, 0);                    // Make the line YELLOW if we didn't collide

        glVertex3f(vLine[0].x, vLine[0].y, vLine[0].z); // Let's draw the normal centered on the triangle
                                                        // Draw the normal of the polygon from the center of the polygon to better see it
        glVertex3f(vLine[1].x, vLine[1].y, vLine[1].z); 
    glEnd();                                            // This is the END of drawing

    // That's it, now use the LEFT and RIGHT arrow keys to move it around to see it in action.

    SDL_GL_SwapBuffers();                                   // Swap the backbuffers to the foreground
}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// In this file we just replaced the call to IntersectedPlane() with IntersectedPolygon()
// and allowed the user to use the PAGE UP and PAGE DOWN keys to further move around the triangle.
// This gives you more ways to test the polygon intersection.  
// Now the triangle can move along the Y axis by using PAGE UP and PAGE DOWN.
// 
// Let's go over the material once more in a brief overview. (Explained more in 3DMath.h)
// 
// 1) Once you find out that the line and the plane intersect, you need to get the intersection
//    point.  In order to get the intersection point, we needed to learn about the dot product.
//    The basic premis of getting the intersection point is finding the distant from a point on
//    the line, either end is fine, then moving from that point along the lines vector.  But,
//    we can't just directly move across that distance, because that is the distance from the plane,
//    it doesn't mean that it's the actual point of intersection.  Take the case that the line is
//    almost parallel with the plane, but is slightly tilted so it intersects a ways down.  Well,
//    the distance from the plane would be very short, where the distance to the intersection point
//    is considerably longer.  To solve this problem, we divide the negated distance by the dot product
//    of the normal and the lines vector.  This then gives us the correct distant to the intersection point.
//
// 2) Once we find the intersection point, we need to test if that point is inside of the polygon.
//    just because we collide with the plane, doesn't mean that we collided with the polygon.
//    Planes are infinate, so the polygon could be hundred of coordinate units from us.  To test
//    to see if we are inside of the polygon, we create vectors from the intersection point to
//    each vertice of the polygon.  Then, as we do this, we calculate the angle between the vectors.
//    We create 2 vectors at a time, which then create a triangle.  We only care about the inner angle.
//    After we are finished adding up the angles between each vector of the polygon, if the angles
//    add up to 360 degrees, then the point is inside of the polygon.  We create a function called
//    AngleBetweenVectors() which gives us the angle between 2 vectors in radians.  So the angles
//    need to add up to at least 2 * PI.  To get the angle between 2 vectors we found out that
//    if we use this equation (V . W / || V || * || W || ) which is the dot product between
//    vector V and vector W, divided by the magnitude of vector V multiplied by the magnitude of vector W.
//    We then take the arc cosine of that result and it gives us the angle in radians.  If we
//    are working with unit vectors (vectors that are normalized with length of 1) we don't need
//    to do the || V || * || W || part of the equation, but that is the TRUE dot product equation.
//
// 3) After we coded those last 2 steps, we put them into a usable function called IntersectedPolygon().
//    It's simple to use, just pass in an array that makes up the polygon, pass in the line array, 
//    then the vertice count of the polygon.  
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
// 
