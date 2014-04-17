//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:      DigiBen        digiben@gametutorials.com         //
//                                                                       //
//        $Program:     Fog                                              //
//                                                                       //
//        $Description: This draws rotating pyramids with adjustable fog.//
//                                                                       //
//        $Date:        7/4/01                                           //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

float g_FogDensity = 0.2f;                                // This holds how thick the fog is
bool  g_bFog = true;                                    // This holds if we want fog ON of OFF

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// CREATE PYRAMID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This creates a pyramid with the center being (X, Y, Z).
/////   The width and height depend on the WIDTH and HEIGHT passed in
/////
///////////////////////////////// CREATE PYRAMID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreatePyramid(float x, float y, float z, int width, int height)
{
    // Here we create 4 TRIANGLES and 1 QUAD to form a pyramid
    // With the passed in radius, we determine the width and height of the pyramid
    // You will notice, I add some color at each vertice to make it more interesting
    // You can delete them if you like and add texture coordinates
    // Notice the order I pass them in.  This is very important.  It is also
    // important that you pick a direction and stick with it.  I pass in my vertices
    // clockwise, assuming I am looking the backs of them.  So, the front triangle
    // has the Top point first, the the right point, then the left point.  Where the
    // back triangle would have the top point first, then the LEFT point, then right point.
    // You can think of it as always starting with the front triangle, then rotating that
    // triangle around to the side or the back.  That is how you will know you pass them
    // in the correct order and direction every time.

    glBegin(GL_TRIANGLES);        
        
        // These vertices create the Back Side
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(0, 255, 255); glVertex3f(x - width, y - height, z - width);    // Bottom left point
        glColor3ub(255, 0, 255); glVertex3f(x + width, y - height, z - width);  // Bottom right point

        // These vertices create the Front Side
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(0, 255, 255); glVertex3f(x + width, y - height, z + width);  // Bottom right point
        glColor3ub(255, 0, 255); glVertex3f(x - width, y - height, z + width);    // Bottom left point

        // These vertices create the Front Left Side
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(255, 0, 255); glVertex3f(x - width, y - height, z + width);    // Front bottom point
        glColor3ub(0, 255, 255); glVertex3f(x - width, y - height, z - width);    // Bottom back point

        // These vertices create the Front right Side
        glColor3ub(255, 0, 0);   glVertex3f(x, y + height, z);                    // Top point
        glColor3ub(255, 0, 255); glVertex3f(x + width, y - height, z - width);    // Bottom back point
        glColor3ub(0, 255, 255); glVertex3f(x + width, y - height, z + width);    // Front bottom point
            
    glEnd();

    // Now, when we want to render a different shape, 
    // we just start a whole new glBegin() and glEnd();

    glBegin(GL_QUADS);

        // These vertices create the bottom of the pyramid
        glColor3ub(0, 0, 255); glVertex3f(x - width, y - height, z + width);    // Front left point
        glColor3ub(0, 0, 255); glVertex3f(x + width, y - height, z + width);    // Front right point
        glColor3ub(0, 0, 255); glVertex3f(x + width, y - height, z - width);    // Back right point
        glColor3ub(0, 0, 255); glVertex3f(x - width, y - height, z - width);    // Back left point
    glEnd();
}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *



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
             g_FogDensity += 0.015f;                 // Increase the fog density
             glFogf(GL_FOG_DENSITY, g_FogDensity);   // How Dense Will The Fog Be
             if(g_FogDensity > 1) g_FogDensity = 1;  // Make sure we don't go above 1
             break;
        
        case SDLK_KP_MINUS :                         // Check if we hit the MINUS key
        case SDLK_MINUS    :
             g_FogDensity -= 0.015f;                 // Decrease the fog density
             glFogf(GL_FOG_DENSITY , g_FogDensity);  // How Dense Will the Fog be
             if(g_FogDensity < 0 ) g_FogDensity = 0; // Make sure we don't go below 0
             break;

        case SDLK_f :                                 // Check if we hit the key F
             g_bFog = !g_bFog;                       // This turns our bool to opposite value 

             if( g_bFog )                             // if g_bFog is true 
                 glEnable(GL_FOG);                   // Turn on fog
             else
                 glDisable(GL_FOG);                  // Else turn off fog 
             break;
             
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
    
        //       Position      View       Up Vector
    gluLookAt(0, 0, 6,     0, 0, 0,     0, 1, 0);        // This determines where the camera's position and view is

    // In here, we create 4 pyramids with out pyramid function.
    // This code will actually rotate all the pyramids AROUND the middle pyramid.
    // You might ask why?  Well, it's because they are all using the same matrix.
    // Check out the Matrix, Rotation and Translation tutorial(s) to understand this further.
    // But basically, all the rotations are done at the origin (0, 0, 0) initially.
    // So, we are rotating the middle one perfectly around the origin, where the other
    // Pyramids are rotating around the origin at a longer distance so they spin around it.
    // The only one that just rotates in it's place is the pyramid that is perfectly
    // centered around the origin.  To make them all rotate in there place we would need
    // to use the glPushMatrix() glPopMatrix() glTranslatef() and glRotatef() functions.
    // There is a tutorial that just goes over these functions.

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
    // This increase the rotation of the cube along each axis.

    rotY += 0.6f;                                        // Increment the Y rotation to increase the angle

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    SDL_GL_SwapBuffers();                                // Swap the backbuffers to the foreground
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// In this file, we create some pyramid and rotate them around the Y axis like teacups :)
// We created a neat function called CreatePyramid() allows us to easily render a modular pyramid.
// We also allow the user to use the PLUS and MINUS key to increase/decrease the fog thickness.
// If you hit 'F' it turns fog on and off.
//
// We put all the fog initialization inside of InitializeOpenGL(), and here are the steps:
//
// 1) We need to set the fog equation (How the fog is generated)
//    with this function:
//
//            glFogi(GL_FOG_MODE, GL_EXP2);                // This is an exponentional method (nice)
//
// 2) Next, we need to pass in a array of size [4] that holds our color:
//
//            glFogfv(GL_FOG_COLOR, fogColor);            // Sets Fog Color
//
// 3) After that, we need to set how thick the fog is:
//
//            glFogf(GL_FOG_DENSITY, g_FogDensity);        // How dense the fog will be
//
// 4) Next, we want to set how perfect the fog is.  Unless have a particular preference, do this:
//
//            glHint(GL_FOG_HINT, GL_DONT_CARE);            // Let openGL choose the quality of the fog
//
// 5) We need to give openGL a distance into the screen when the fog starts and stops:
//
//            glFogf(GL_FOG_START, 0);                    // Fog Start Depth
//            glFogf(GL_FOG_END, 10.0f);                    // Fog End Depth
//    
//    6) Lastly, we just need to turn the ON switch for the fog:
//
//            glEnable(GL_FOG);                            // Turns on fog (glDisable(GL_FOG) turns it off)
// 
// That's it!  Fog is pretty straightforward.  Lighting is just as basic.  We will discuss 
// lighting next.
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
