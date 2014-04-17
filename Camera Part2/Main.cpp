//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        DigiBen        digiben@gametutorials.com       //
//                                                                       //
//        $Program:        Camera2                                       //
//                                                                       //
//        $Description:    Rotate our existing camera around in the world//
//                                                                       //
//        $Date:            6/18/01                                      //
//                                                                       //
//***********************************************************************//

#include "main.h"                                       // This includes our header file
//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

#define kSpeed  0.03f                                   // This is how fast our camera moves

// These are externed in main.h.  Look in main.h for more explanation on externing.
// Basically, it allows Init.cpp to use them too.  We do this for all global variables.

CCamera g_Camera;                                       // Define our camera object

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

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

    // *Hint* We will put all our game init stuff here
    // Some things include loading models, textures and network initialization
                         //Position      View          Up Vector
    g_Camera.PositionCamera(0, 0.5, 6,   0, 0.5, 0,   0, 1, 0 );    
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
                    HandleKeyReleaseEvent(& event.key.keysym) ;         // callback for handling keystrokes, arg is key released
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
        CheckForMovement();                                 // Check for Movement
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                      // Reset The matrix

    // Give openGL our camera position, then camera view, then camera up vector
    gluLookAt(g_Camera.m_vPosition.x, g_Camera.m_vPosition.y, g_Camera.m_vPosition.z,    
              g_Camera.m_vView.x    , g_Camera.m_vView.y    , g_Camera.m_vView.z,    
              g_Camera.m_vUpVector.x, g_Camera.m_vUpVector.y, g_Camera.m_vUpVector.z);

    glBegin(GL_TRIANGLES);
//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////
        
        // Draw 10 triangles of the same type down the -Z axis (in front of the camera)
        for(float i = -10; i < 0; i++)
        {
            glColor3ub(255, 0, 0);                      // Make the top vertex RED
            glVertex3f(0, 1, i);                        // Here is the top point of the triangle
    
            glColor3ub(0, 255, 0);                      // Make the left vertex GREEN
            glVertex3f(-1, 0, i);                       // Here is the left point of the triangle
    
            glColor3ub(0, 0, 255);                      // Make the right vertex BLUE
            glVertex3f(1, 0, i);                        // Here is the right point of the triangle
        }

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////
    glEnd();

    SDL_GL_SwapBuffers();                               // Swap the backbuffers to the foreground
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// In this tutorial we allowed the camera to rotate left and right.  Also, we fixed
// the choppy movement.
//
// when we get the SDL_KEYDOWN event we find out which key is down and untill it is 
// released we mark the key as pressed . For example
// 
// This is in SDL_KEYDOWN (HandleKeyPressEvents)
// case SDLK_UP :
//        upPressed = true;
//        break;
//
// This is in SDL_KEYUP (HandleKeyReleaseEvents)
// case SDLK_UP :
//        upPressed = false;
//        break;
//
// here upPressed is just a boolean variable . so untill we get the release event we make
// upPressed true;
// and in the CheckForMovement Function we check whether upPressed is true or false 
// if it is true we change the camera position otherwise we leave it like that ..
// Similarly for other keys (down , left , right -> downPressed , leftPressed , rightPressed)
// 
// Eventually, we will want to use spherical coordinates for the view.  This means
// that we will be translating the mouse coordinates to direct where we are looking,
// just like in most first person shooter type games.
// 
// By now you should be able to make some walls and levels to walk around.  Give it a
// try.  I wanted to make a small maze, but I didn't want to complicate the tutorial.
// The creativity should come from you, not me :)  I just deliver the know how.
// 
// I did however, add more triangles by creating a for loop in the glBegin() scope.
// It just draws 10 triangles instead of one.  Walk around and check them out!
// 
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
