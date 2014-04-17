//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        DigiBen        digiben@gametutorials.com       //
//                                                                       //
//        $Program:        Lighting                                      //
//                                                                       //
//        $Description:    This draws rotating pyramids with lighting.   //
//                                                                       //
//        $Date:            7/5/01                                       //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// Let's assign our light position right above (0, 0, 0), directly above the middle triangle.
// We also have our boolean that holds if we want light enabled of disabled

float g_LightPosition[4] = {0, 1, 0, 1};                // This is the position of our light
float g_bLight = true;                                    // This holds if we want light ON of OFF

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////// CREATE PYRAMID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This creates a pyramid with the center being (X, Y, Z).
/////   The width and height depend on the WIDTH and HEIGHT passed in
/////
///////////////////////////////// CREATE PYRAMID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreatePyramid(float x, float y, float z, int width, int height)
{
    
    glBegin(GL_TRIANGLES);        
        
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

        // Below we add something new to our pyramid code, NORMALS.  In order for OpenGL
        // to know how much it needs to light a certain polygon, it depends on the normal
        // of that polygon.  A normal is the direction of the polygon.  You will notice
        // that we give the BACK polgon a normal of (0, 1, -1).  This means that the triangle
        // is facing slightly up and down the negative Z axis (into the screen).  Remember,
        // These are directions, not actuall coordinates.  There is more discussed about
        // Normals at the bottom of this file in the QUICK NOTES section.  There will be
        // a seperate tutorial just devoted to this subject, since it is very important for lighting.

        // The glNormal3f() functions let's us specify the normal for the next vertices passed in.
        // To compute the normal of a poly is a bit complicated at first so I will not go into it.
        // To avoid having to discuss to much in this tutorial, we will just do an approximation.

        // These vertices create the Back Side
        glNormal3f(0, 1, -1);     // This polygon is facing backwards and up
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(0, 255, 255); glVertex3f(x - width, y - height, z - width);    // Bottom left point
        glColor3ub(255, 0, 255); glVertex3f(x + width, y - height, z - width);  // Bottom right point

        // These vertices create the Front Side
        glNormal3f(0, 1, 1);     // This polygon is facing towards us and slightly up.
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(0, 255, 255); glVertex3f(x + width, y - height, z + width);  // Bottom right point
        glColor3ub(255, 0, 255); glVertex3f(x - width, y - height, z + width);    // Bottom left point

        // These vertices create the Front Left Side
        glNormal3f(-1, 1, 0);     // This polygon is facing slightly up and to the left
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(255, 0, 255); glVertex3f(x - width, y - height, z + width);    // Front bottom point
        glColor3ub(0, 255, 255); glVertex3f(x - width, y - height, z - width);    // Bottom back point

        // These vertices create the Front right Side
        glNormal3f(1, 1, 0);    // This polygon is facing slightly up and to the right
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(255, 0, 255); glVertex3f(x + width, y - height, z - width);    // Bottom back point
        glColor3ub(0, 255, 255); glVertex3f(x + width, y - height, z + width);    // Front bottom point
            
    glEnd();

    // Now render the bottom of our pyramid

    glBegin(GL_QUADS);

        // These vertices create the bottom of the pyramid
        glNormal3f(0, -1, 0);    // This polygon is facing straight down
        glColor3ub(0, 0, 255); glVertex3f(x - width, y - height, z + width);    // Front left point
        glColor3ub(0, 0, 255); glVertex3f(x + width, y - height, z + width);    // Front right point
        glColor3ub(0, 0, 255); glVertex3f(x + width, y - height, z - width);    // Back right point
        glColor3ub(0, 0, 255); glVertex3f(x - width, y - height, z - width);    // Back left point
    glEnd();

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

}

///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    // Enable key repeat 
    if ( SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL) )
    {
        cerr << "Failed enabling key repeat : " << SDL_GetError() << endl;
        Quit(1);
    }

    InitializeOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);    // Init OpenGL
}

///////////////////////////////// KEY PRESSED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function handles the input more effectively than in the WinProc()
/////
///////////////////////////////// KEY PRESSED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void KeyPressed(SDL_keysym * keysym)
{
    switch(keysym -> sym)
    {
        case SDLK_F1 :                      // If the key is F1 
            ToggleFullScreen();             // Toggle between FullScreen and windowed mode
            break;

        case SDLK_ESCAPE :                  // if the key is ESCAPE
            Quit(0);

        case SDLK_KP_PLUS :                          // Check if we hit the PLUS key
        case SDLK_EQUALS  :
            g_LightPosition[1] += 0.1f;              // Increase the light's Y position
                                                     // Make sure that we don't go higher than 5
            if( g_LightPosition[1] > 5 ) 
                g_LightPosition[1] = 5;

            
            break;
        
        case SDLK_KP_MINUS :                         // Check if we hit the MINUS key
        case SDLK_MINUS    :
            g_LightPosition[1] -= 0.1f;               // Decrease the light's Y position
                                                      // Make sure that we don't go lower than -5
            if( g_LightPosition[1] < -5 ) g_LightPosition[1] = -5 ;
            
            break;

        case SDLK_l :                                 // Check if we hit the key L
            g_bLight = !g_bLight ;                   // Turn the light ON of OFF 

            if( g_bLight )                             // if we want the light on , turn it on
                glEnable(GL_LIGHTING);
            else                                     // Else, turn OFF the light 
                glDisable(GL_LIGHTING);

        default:
            break;
    }
}
///////////////////////////////// MAIN GAME LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function Handles the main game loop
/////
///////////////////////////////// MAIN GAME LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

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
                    KeyPressed( & event. key.keysym );                  // callback for handling keystrokes, arg is key pressed
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

float rotY = 0.0f;

void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                    // Reset The matrix
    
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // We update our light position BEFORE we call gluLookAt()
    // so it will update the lighting correctly.  Put it below to see what I mean.        
    // If we weren't moving the light around, we wouldn't need to put this here.
    glLightfv( GL_LIGHT0, GL_POSITION, g_LightPosition );    

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

        //       Position      View       Up Vector
    gluLookAt(0, 0, 6,     0, 0, 0,     0, 1, 0);        // This determines where the camera's position and view is

    glRotatef(rotY, 0.0f, 1.0f, 0.0f);                    // Rotate the pyramids along the Y axis

    // This creates a 3D pyramid in the center at (0, 0, 0)
    CreatePyramid(0, 0, 0, 1, 1);

    // This creates a 3D pyramid back and to the right of the middle pyramid
    CreatePyramid(3, 0, -3, 1, 1);

    // This creates a 3D pyramid back and to the left of the middle pyramid
    CreatePyramid(-3, 0, -3, 1, 1);

    // This creates a 3D pyramid in right front of the camera
    CreatePyramid(0, 0, 5, 1, 1);

    // Below we just increment our rotation degree by a small amount each frame.
    // This increases the rotation of the cube along each axis.

    rotY += 0.6f;                                        // Increment the Y rotation to increase the angle

    SDL_GL_SwapBuffers();                                // Swap the backbuffers to the foreground
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// In this file, we create some pyramid and rotate them around the Y axis like teacups :)
// We created a neat function called CreatePyramid() allows us to easily render a modular pyramid.
// We also allow the user to use the PLUS and MINUS key to increase/decrease the lights Y position.
// If you hit 'L' it will allow the user to turn on and off lighting
//
// Remember, if we are using the color functions glColor*(), the are ignored
// when we enable lighting unless we enable GL_COLOR_MATERIALS.  
// We will want to use texture mapping to simulate better lighting.
//
// NORMALS - I will try to quickly explain what a normal is and why we need them for lighting.
//             This does not substitute a good tutorial, so please look on the site for one.
//           Every polygon has a NORMAL.  A normal is just the direction that the polygon is
//           facing.  If we have a poly facing straight up, the normal could be (0, 1, 0).
//           If we have a polygon facing straight down, the normal could be (0, -1, 0).  It's
//           just a direction, that's it.  To calculate the normal you use the cross product formula.
//           The cross product gives you a vector that is perpendicular to 2 other vectors.
//             So, if you have a triangle, then you have 2 vectors that you can get the normal from.
//           The reason why we use normals for lighting is because, OpenGL looks at the normal
//           to find out how much it needs to light a certain polygon.  If we have a light pointing
//           in the opposite direction of the light, then that polygon gets practically NO light.
//           See how that works?  Depending on how the direction of the polygon is facing, it gets
//           a certain degree of light applied to it.
//
// I did not compute the exact normal for each of the sides of the pyramid because I didn't
// want to complicate the tutorial.  In another tutorial I will discuss how to do this, along
// with doing VERTEX normals rather than just doing face normals.  We are just doing face normals.
// A face normal is the normal for the whole polygon, but we can get better looking results when
// we do a normal for EACH vertex.  it produces smoother lighting.
//
// We put all the light initialization inside of InitializeOpenGL() (init.cpp).
// Here is a brief list of steps to doing initializing lighting.
//
// 1) First we need to set the ambience of our world: (The non lit color)
//  
//            glLightfv( GL_LIGHT0, GL_AMBIENT,  ambienceArray );
//
// 2) Next we need to set the diffuse of our light:  (The light color)
//
//            glLightfv( GL_LIGHT0, GL_DIFFUSE,  diffuseArray);
//
// 3) Next, we need to set the light's position:
//
//            glLightfv( GL_LIGHT0, GL_POSITION, positionArray);
//
// 4) Next, once we initialized the light, we need to turn it on:
//
//            glEnable(  GL_LIGHT0   );                   // Choose the light we want to turn on
//
// 5) Finally, we need to turn lighting on so our individual lights have power:
//
//            glEnable(  GL_LIGHTING)                        // Turn lighting on (glDisable(GL_LIGHTING) turns it off)
//
// Let us know what you think and if this was helpfull.  We are always excited to get feedback.
// 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
