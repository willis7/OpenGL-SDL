//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        Ben Humphrey    digiben@gametutorials.com      //
//                                                                       //
//        $Program:        Color                                         //
//                                                                       //
//        $Description:    Adds color to our OpenGL primatives           //
//                                                                       //
//        $Date:            6/18/01                                      //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file

///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeGL(SCREEN_WIDTH, SCREEN_HEIGHT);           // Init OpenGL with the global rect

    // *Hint* We will put all our game init stuff here
    // Some things include loading models, textures and network initialization
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
/////    This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
    int i=0;    

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                    // Reset The matrix
    
        //       Position      View       Up Vector
    gluLookAt(0, 0, 6,     0, 0, 0,     0, 1, 0);        // This determines where the camera's position and view is

    // *COLOR*
    // You will notice below that we call a new function called glColor3ub()
    // This stands for (OpenGL Color with 3 parameters using unsigned bytes)
    // That means that, just like in GDI, we pass in the R G and B values from 0 to 255.
    // As you can see, we color the first point RED, then the next GREEN, then BLUE.
    // Coloring each vertex like this makes a cool blend effect.  This is called interpolation.
    // If we want to color from 0.0 to 1.0 we can use glColor3f() (IE.  glColor3f(1.0f, 0.0f, 0.0f); = RED)
    // Try commenting out the GREEN and BLUE lines.  What do you think will happen?
    // You got it!  It will turn the triangle RED.  That is how you use color.  Pretty simple huh?

    // Below we say that we want to draw triangles
    glBegin (GL_TRIANGLES);                              // This is our BEGIN to draw

        glColor3ub(255, 0, 0);                           // Make the top vertex RED
        glVertex3f(0, 1, 0);                             // Here is the top point of the triangle

        glColor3ub(0, 255, 0);                           // Make the left vertex GREEN
        glVertex3f(-1, 0, 0);                            // Here is the left point of the triangle

        glColor3ub(0, 0, 255);                           // Make the right vertex BLUE
        glVertex3f(1, 0, 0);                             // Here is the right point of the triangle
    glEnd();                                             // This is the END of drawing

    SDL_GL_SwapBuffers();                                // Swap the backbuffers to the foreground
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// The functions that are in this source file are the ones that we will
// be changing on a regular basis.  The functions left in init.cpp are ones
// we will rarely touch, and when we do, it is maybe a line or so.  From now
// on, the tutorials that follow will use this project format.  It will allow
// us to simply focus on the main.cpp with less code, rather than sifting through
// tons of superfluous initialization code.
//
// So, what do you think?  Color is pretty simple huh?  The 2 functions that you
// will want to use is:  glColor3ub() and glColor3f().
//
// You usually want to use glColor3ub() because it works just like GDI so it might
// be a little more comfortable with you.  On the other hand, if you want more
// precision you will want to use glColor3f().  This makes it so you can use high
// precision floating point numbers like 0.33333f.  This comes in handy for heightmap
// terrain.  Also, you might want to use this because it could be more intuitive to
// think of the color from 0 to 1 because it's like 0 to 100 percent.  The only problem
// with using this is that floating point calculations are always more computationally
// expensive and you want to avoid them when ever you can.
//
// For example, we could actually use glVertex3i() instead of glVertex3f() since we aren't
// using floating point values for our triangle points, but it's rare that we use just
// whole numbers so I recommend just sticking with glVertex3f() unless you ONLY use
// integer numbers, which is rare, but faster.  If you are loading a model or dealing with
// cameras, you are always dealing with floating point values.  
//
// In the next tutorials we will actually show you how to move around in this world.
// That is when it starts to get REALLY cooOoOoOoOoOool.  
//
// For now, just start messing around with plotting points and changing colors.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
