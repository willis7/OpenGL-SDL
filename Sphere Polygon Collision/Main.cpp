//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen         DigiBen@GameTutorials.com        //
//                                                                       //
//      $Program:       SpherePolygonCollision                           //
//                                                                       //
//      $Description:   This demonstrates sphere and polygon collision   //
//                                                                       //
//      $Date:          1/23/02                                          //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
//
// This tutorial is built from the "PolygonCollision" tutorial.  Instead of a line
// colliding with a polygon, we now can do better collision that checks to see if
// a sphere collides with a polygon.  The code is modular that it can do triangles
// and quads, or any other convex polygon.  This technique is great for camera  
// and world collision.  Since sphere to sphere collision is too simple, as well as
// sphere to plane collision, we will explain both of them in this tutorial.
// What this application does is allow us to move a wire frame sphere around and have
// it run into a triangle.  The sphere will turn green if it is colliding with
// the triangle, while it will be purple when there is no collision.  
// Here are the commands to move the sphere and camera position:
//
// LEFT ARROW - Moves the sphere left along it's current plane
// RIGHT ARROW - Moves the sphere right along it's current plane
// UP ARROW - Moves the sphere up along it's current plane
// DOWN ARROW - Moves the sphere down along it's current plane
// F3 - Moves the sphere towards the front of the triangle's plane
// F4 - Moves the sphere towards the back of the triangle's plane
// 
// F1 - Rotates the camera left
// F2 - Rotates the camera right
// ESC - Quits the program
//
// 
// Sphere and polygon collision is a lot more complicated that you might first assume.
// If you want to jump immediately to the theory, the concepts are explain in the 
// * QUICK NOTES * section near the bottom of this file or 3DMath.cpp.  The next 
// tutorial will show us how to use this code for our camera class, which will 
// allow us to collide and slide into the walls of a world.
//
//

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *       

// This is the array of 3 vertices that will hold our triangle coordinates      
CVector3 g_vTriangle[3];

// This is the center of our sphere.  We are able to move it with the arrow keys.
CVector3 g_vPosition;

// This is the current rotate of our camera (F1 and F2 keys)
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
    
    // Here is where we define our triangle.  Remember it is important which
    // direction you define the vertices.  This is important because it allows
    // us to know which side the normal will be on when we calculate the triangle's
    // normal.  Though clockwise is more natural to me to use, more people use
    // counter-clockwise.  Also, if you enable back face culling in OpenGL, the
    // default is counter-clockwise (GL_CCW).  We start with the bottom-left
    // vertices, then the bottom-right and finally, the top vertex.
    g_vTriangle[0] = CVector3(-1,  0,   0);
    g_vTriangle[1] = CVector3( 1,  0,   0);
    g_vTriangle[2] = CVector3( 0,  1,   0);

    // We need an initial position for the sphere's center, so we place it
    // right in the middle of the triangle, initially colliding with it.
    g_vPosition = CVector3(0, 0.5f, 0);

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
    int i=0;    

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                   // Reset The matrix


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Set our camera to the left a bit for a nice side view of the left hand side

        //      Position          View       Up Vector
    gluLookAt(-2.5f, 0.5, -0.1,  0, 0.5f, 0,   0, 1, 0); // This determines where the camera's position and view is

    // Rotate the camera around our world for a 360 degree view of what is going on.
    // Yes I know... we are actually rotating the whole world, not the camera :)
    glRotatef(g_rotateY, 0, 1, 0);

    // We need a radius for our sphere, so let's create it here with 0.1
    float radius = 0.1f;
    
    // Draw the polygon

    glBegin (GL_TRIANGLES);                             // This is our BEGIN to draw

        glColor3ub(255, 0, 0);                          // Make the Left vertex RED
        glVertex3f(g_vTriangle[0].x, g_vTriangle[0].y, g_vTriangle[0].z);

        glColor3ub(0, 255, 0);                          // Make the Right vertex GREEN
        glVertex3f(g_vTriangle[1].x, g_vTriangle[1].y, g_vTriangle[1].z);

        glColor3ub(0, 0, 255);                          // Make the Top vertex BLUE
        glVertex3f(g_vTriangle[2].x, g_vTriangle[2].y, g_vTriangle[2].z);
    glEnd();                                            // This is the END of drawing

    // Instead of calculating the sphere ourselves, we are going to use quadrics.
    // Check out the tutorial on quadrics if this is confusing for you.

    // Allocate a quadric object to use as a sphere
    GLUquadricObj *pObj = gluNewQuadric();              // Get a Quadric off the stack

    // To make it easier to see, we want the sphere to be in wire frame
    gluQuadricDrawStyle(pObj, GLU_LINE);                // Draw the sphere normally

    // Move the sphere to it's center position
    glTranslatef(g_vPosition.x, g_vPosition.y, g_vPosition.z);

    // Now we get to the wonderful function that does it all for us.  All we
    // need to do is pass in the array of vertices for the triangle, the center
    // of the sphere and it's radius.  This will return a true or false, depending
    // on if we collided or not.  True = The Sphere Collided
    bool bCollided = SpherePolygonCollision(g_vTriangle, g_vPosition, 3, radius);   
    
    // If we collided we want the sphere to be green, otherwise it should be purple
    if(bCollided)
        glColor3ub(0, 255, 0);                          // Make the sphere green
    else        
        glColor3ub(255, 0, 255);                        // Make the sphere purple
        
    // Draw the quadric as a sphere with the radius of .1 and a 15 by 15 detail.
    // To increase the detail of the sphere, just increase the 2 last parameters.
    gluSphere(pObj, radius, 15, 15);

    gluDeleteQuadric(pObj);                             // Free the Quadric

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


    SDL_GL_SwapBuffers();                                   // Swap the backbuffers to the foreground
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// As you can see, using our sphere-polygon collision function is really simple.
// The hard part is actually creating the code underneath it.  To test the collision
// detection, I set up some keys to move the camera and sphere around.  If the
// sphere is colliding with the triangle it will turn green, otherwise it will 
// be purple.
//
// You might be thinking, "So how does this help me games?".  Well, this code
// is perfect for checking if your character (3rd person view) or camera (1st person view)
// collides with the world.  Eventually, you are going to have to get down to checking
// collision with each polygon, this is a perfect technique.  It's faster than any
// other geometry-polygon collision test that I know of.  Some games, like MDK2
// used a cylinder for their character collision.  This is also another great way.
// An ellipse is another geometric shape that is fast.  Though bounding box checks
// can be slower, these are also still excellent choices, especially if you only
// need to check the front 3 faces of the box.  The next tutorial will show us
// how to create a function for our camera class that checks polygons and collides
// and slides along them.  We will have a little textured world to move around.
// I bet your mouth is drooling already.  
//
// Let's take a look at the explanations given in 3DMath.cpp of the theory/math behind
// sphere to polygon collision:
//
// Basically, here is the overview:  
//
// 1) First you want to check if the sphere collides with the triangle's plane.
//    Remember, that planes are infinite and you could be 500 units from the
//    polygon and it's still going to trigger this first test.  We want to 
//    write a function that classifies the sphere.  Either it's completely 
//    in front of the plane (the side the normal is on), intersecting the
//    plane or completely behind the plane.  Got it so far?  We created a 
//    function called ClassifySphere() that returns BEHIND, FRONT or INTERSECTS.
//    If ClassifySphere() returns INTERSECTS, then we move on to step 2, otherwise
//    we did not collide with the triangle.
// 
// 2) The second step is to get an intersection point right in front of the sphere.
//    This one of the tricky parts.  We know that once we have an intersection point
//    on the plane of the triangle, we just need to use the InsidePolygon() function
//    to see if that point is inside the dimensions of the triangle, just like we
//    did with the Ray to Polygon Collision tutorial.  So, how do we get the point
//    of intersection?  It's not as simple as it might sound.  Since a sphere is
//    infinite, there would be a million points that it collided at.  You can just
//    draw a ray in the direction the sphere was moving because it could have just
//    nicked the bottom of the triangle and your ray would find an intersection
//    point that is outside of the triangle.  Well, it turns out that we need to
//    first try and give it a shot.  We will try the first attempt a different way though.
//    We know that we can find the normal vector of the triangle, which in essence
//    tells us the direction that the triangle is facing.  From ClassifyPoly(),
//    it also returns the distance the center our sphere is from the plane.  That
//    means we have a distance our sphere center is from the plane, and the normal
//    tells us the direction the plane is in.  If we multiply the normal by the
//    distance from the plane we get an offset.  This offset can then be subtracted
//    from the center of the sphere.  Believe it or not, but we now have a position
//    on the plane in the direction of the plane.  Usually, this intersection points
//    works fine, but if we get around the edges of the polygon, this does not work.
//    What we just did is also called "projecting the center of the sphere onto the plane".
//    Another way to do this is to shoot out a ray from the center of the sphere in
//    the opposite direction of the normal, then we find the intersection of that line
//    and the plane.  My way just takes 3 multiplies and a subtraction.  You choose.
//
// 3) Once we have our psuedo intersection point, we just pass it into InsidePolygon(),
//    along with the triangle vertices and the vertex count.  This will then return
//    true if the intersection point was inside of the triangle, otherwise false.
//    Remember, just because this returns false doesn't mean we stop there!  If
//    we didn't collide yet, we need to skip to step 4.
//
// 4) If we get here, it's assumed that we tried our intersection point and it
//    wasn't in the polygon's perimeter.  No fear!  There is hope!  If we get to step
//    4, that's means that our center point is outside of the polygon's perimeter. Since
//    we are dealing with a sphere, we could still be colliding because of the sphere's radius.
//    This last check requires us to find the point on each of the polygon's edges that
//    is closest to the sphere's center.  We have a tutorial on finding this, so make sure
//    you have read it or are comfortable with the concept.  If we are dealing with a
//    triangle, we go through every side and get an edge vector, and calculate the closest
//    point on those lines to our sphere's center.  After getting each closest point, we
//    calculate the distance that point is from our sphere center.  If the distance is
//    less than the radius of the sphere, there was a collision.  This way is pretty fast.  
//    You don't need to calculate all three sides evey time, since the first closest point's 
//    distance could be less than the radius and you return "true".
// 
//
// Quite a bit of theory, but not too bad.  Once again, the next tutorial will 
// demonstrate how to hook this up to our camera class and collide and slide against 
// walls in a world. Check it out at www.GameTutorials.com - You won't find any place like it!
//
// I would like to thank Paul Nettle for the collision detection idea.  If you want a great
// tutorial on collision detection, check out http://www.fluidstudios.com/publications.html.
// I thuroughly enjoyed the tutorial on "Generic Collision Detection for Games Using Ellipsoids".
// This provides a great visual tutorial on this topic as well.  I highly recommend it!
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
