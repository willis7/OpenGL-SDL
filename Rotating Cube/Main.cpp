////***********************************************************************//
////                                                                       //
////      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
////                                                                       //
////      $Author:        DigiBen     digiben@gametutorials.com            //
////                                                                       //
////      $Program:       RotateCube                                       //
////                                                                       //
////      $Description:   This draws a rotating cube to the screen         //
////                                                                       //
////      $Date:          7/3/01                                           //
////                                                                       //
////***********************************************************************//

# include "main.h"

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// CREATE CUBE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This create a cube at X Y Z being the bottom left corner.
/////   The width and height depend on the radius passed in.
/////
///////////////////////////////// CREATE CUBE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateCube(float x, float y, float z, int radius)
{
    // Here we create 6 QUADS (Rectangles) to form a cube
    // With the passed in radius, we determine the width and height of the cube
    // You will notice, I add some color at each vertice to make it more interesting

    glBegin(GL_QUADS);        
        
        // These vertices create the Back Side
        glColor3ub(0, 0, 255);   glVertex3f(x, y, z);
        glColor3ub(255, 0, 255); glVertex3f(x, y + radius, z);
        glColor3ub(0, 255, 255); glVertex3f(x + radius, y + radius, z); 
        glColor3ub(0, 255, 255); glVertex3f(x + radius, y, z);

        // These vertices create the Front Side
        glColor3ub(0, 0, 255);   glVertex3f(x, y, z + radius);
        glColor3ub(255, 0, 255); glVertex3f(x, y + radius, z + radius);
        glColor3ub(0, 255, 255); glVertex3f(x + radius, y + radius, z + radius); 
        glColor3ub(0, 255, 255); glVertex3f(x + radius, y, z + radius);

        // These vertices create the Bottom Face
        glColor3ub(0, 0, 255);   glVertex3f(x, y, z);
        glColor3ub(255, 0, 255); glVertex3f(x, y, z + radius);
        glColor3ub(0, 255, 255); glVertex3f(x + radius, y, z + radius); 
        glColor3ub(0, 255, 255); glVertex3f(x + radius, y, z);
            
        // These vertices create the Top Face
        glColor3ub(0, 0, 255);   glVertex3f(x, y + radius, z);
        glColor3ub(255, 0, 255); glVertex3f(x, y + radius, z + radius);
        glColor3ub(0, 255, 255); glVertex3f(x + radius, y + radius, z + radius); 
        glColor3ub(0, 255, 255); glVertex3f(x + radius, y + radius, z);

        // These vertices create the Left Face
        glColor3ub(0, 0, 255);   glVertex3f(x, y, z);
        glColor3ub(255, 0, 255); glVertex3f(x, y, z + radius);
        glColor3ub(0, 255, 255); glVertex3f(x, y + radius, z + radius); 
        glColor3ub(0, 255, 255); glVertex3f(x, y + radius, z);

        // These vertices create the Right Face
        glColor3ub(0, 0, 255);   glVertex3f(x + radius, y, z);
        glColor3ub(255, 0, 255); glVertex3f(x + radius, y, z + radius);
        glColor3ub(0, 255, 255); glVertex3f(x + radius, y + radius, z + radius); 
        glColor3ub(0, 255, 255); glVertex3f(x + radius, y + radius, z);

    glEnd();
}
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


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
/////    This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float rotX = 0.0f, rotZ = 0.0, rotY = 0.0f;

void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                    // Reset The matrix
    
        //       Position      View       Up Vector
    gluLookAt(0, 0, 6,     0, 0, 0,     0, 1, 0);        // This determines where the camera's position and view is

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Below we rotate our cube by the glRotatef() function.
    // The first parameter is the degrees to rotate by (That is why we need to keep the data)
    // We create 3 global variables to store and keep the current rotation so it can be incremental
    // and smooth.  The next 3 parameters is the vector we are rotating by.
    // If we want to rotate around the X axis, we pass in (1, 0, 0)
    // if want to rotate around the Y axis, we pass in (0, 1, 0) and (0, 0, 1) for Z.
    // That is how it knows how to rotate it along which axis.  We are basically give it a rotation vector.
    // You might be thinking, well... where does it change the vertices?  It doesn't.
    // There is a maxtrix that is associated with our projection, rotation, etc...
    // We just times that current matrix by a rotation matrix, which rotates the cube
    // on the screen, it doesn't actually change the vertices.  It is faster to use matrixs
    // than it is to rotate every vertice by itself.  I personally like doing every vertice so I
    // can keep track of where every vertice is in the world, but it is a lot slower that way.

    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

    // This creates our cube starting the bottom left corner at (-1, -1, -1)
    // We give our cube a width and height of 2.  Nice little function eh? :)
    // See if you can figure out how to texture map the cube using the past tutorials.
    CreateCube(-1, -1, -1, 2);

    // Since we center the cube around the origin (0, 0, 0) , it rotates nicely.
    // if we didn't, it would rotate in a weird way, like try changing the z position to 0.

    // Below we just increment our rotation degree by a small amount each frame.
    // This increase the rotation of the cube along each axis.

    rotX += 0.3f;
    rotY += 0.3f;
    rotZ += 0.8f;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    SDL_GL_SwapBuffers();                                    // Swap the backbuffers to the foreground
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Creating a cube is simple isn't it?  Once you know how to draw a QUAD, you know
// how to draw a cube.  it's just 6 quads positioned correctly.
// We created a nifty Function called CreateCube() which can make cubes to our hearts delight :)
// 
// ROTATING:  We learned how to use the glRotatef() function.  This is as simple as it looks.
//            Just pass in the degree you want to rotate, and then place a 1 in the axis
//            you want to rotate around.  Try messing around with glTranslatef(x, y, z);
//
// Hopefully this tutorial helps out enough for you to get your own cubes and rotations up
// and running.  Let us know what you do with this or if you have any feedback about the tutorial.
// 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
