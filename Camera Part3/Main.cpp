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

#include "main.h"                                        // This includes our header file
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
//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////
            
            // Here we check for mouse and key movements every frame
            g_Camera.SetViewByMouse();                    // Move the camera by the mouse
            CheckForMovement();                           // Check if we pressed a key
            
            RenderScene();                                // Update the screen    
            
//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////
    } // while( ! done)
}

///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                    // Reset The matrix

    // Give openGL our camera position, then camera view, then camera up vector
    gluLookAt(g_Camera.m_vPosition.x, g_Camera.m_vPosition.y, g_Camera.m_vPosition.z,    
              g_Camera.m_vView.x,      g_Camera.m_vView.y,     g_Camera.m_vView.z,    
              g_Camera.m_vUpVector.x, g_Camera.m_vUpVector.y, g_Camera.m_vUpVector.z);

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

    // Below, instead of drawing triangle we draw 3D rectangles, or QUADS as they are called.
    // So, now instead of passing GL_TRIANGLES we say GL_QUADS.
    // Triangles take 3 points, but Quads take 4, so we need to add an extra 3D point for each primative.

    // Below there is about 20 quads draw upwards to form a tower type shape.
    // I slowly make each quads smaller, and it makes an interesting effect.

    // Below we say that we want to draw QUADS
    glBegin (GL_QUADS);                               // This is our BEGIN to draw
        
        for(float i = 0; i < 100; i += 5)
        {
            glColor3ub(255, 255,   0);                // Make this corner YELLOW
            glVertex3f(-10 + i/5,   i,  10 - i/5);    // Here is the bottom left corner of the QUAD

            glColor3ub(255,   0,   0);                // Make this corner RED
            glVertex3f(-10 + i/5,   i, -10 + i/5);    // Here is the top left corner of the QUAD
        
            glColor3ub(0,   255, 255);                // Make the left vertex CYAN
            glVertex3f(10 - i/5,    i, -10 + i/5);    // Here is the top right corner of the QUAD
        
            glColor3ub(0,     0, 255);                // Make the right vertex BLUE
            glVertex3f(10 - i/5,    i,  10 - i/5);    // Here is the bottom right corner of the QUAD
        }
        
//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

    glEnd();                                            // This is the END of drawing

    SDL_GL_SwapBuffers();                               // Swap the backbuffers to the foreground
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

