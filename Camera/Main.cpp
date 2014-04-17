//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     digiben@gametutorials.com            //
//                                                                       //
//      $Program:       Camera                                           //
//                                                                       //
//      $Description:   Create and move a camera around in our world     //
//                                                                       //
//      $Date:          6/18/01                                          //
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                      // Reset The matrix

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

    // Now instead of passing in hard coded values, we give gluLookAt()
    // the values that our global camera has.  Now we can check if we
    // hit the LEFT RIGHT UP and DOWN arrow keys, then update the camera.
    // It will automatically update right here.  Cool huh?

    // Give openGL our camera position, then camera view, then camera up vector
    gluLookAt(g_Camera.m_vPosition.x, g_Camera.m_vPosition.y, g_Camera.m_vPosition.z,    
              g_Camera.m_vView.x    , g_Camera.m_vView.y    , g_Camera.m_vView.z,    
              g_Camera.m_vUpVector.x, g_Camera.m_vUpVector.y, g_Camera.m_vUpVector.z);

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

    // Below we say that we want to draw triangles
    glBegin (GL_TRIANGLES);                                // This is our BEGIN to draw

        glColor3ub(255, 0, 0);                             // Make the top vertex RED
        glVertex3f(0, 1, 0);                               // Here is the top point of the triangle

        glColor3ub(0, 255, 0);                             // Make the left vertex GREEN
        glVertex3f(-1, 0, 0);                              // Here is the left point of the triangle

        glColor3ub(0, 0, 255);                             // Make the right vertex BLUE
        glVertex3f(1, 0, 0);                               // Here is the right point of the triangle
    glEnd();                                               // This is the END of drawing

    SDL_GL_SwapBuffers();                                  // Swap the backbuffers to the foreground
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
// So now we can actually move around somewhat in our world!  Excited yet?
// Okay, so moving forward and backward isn't to exciting, but it's a start.
// I decided NOT to do any camera rotations in this tutorial because I wanted
// you to suck up all of this camera stuff before we got more complicated.  If
// you understand what we did here, you should be prepared for the next tutorial
// that deals with camera rotations.  In the next tutorial we will do the code that
// allows us to rotate our view left and right, and then walk in that direction.
// It's not hard, but it's a bit too much to digest with the other stuff.  I want
// to make sure you understand everything that we did here.
// 
// So, what DID we do in this tutorial?  Well, if you notice the ** NEW ** tags,
// you will see what has been added to the previous tutorial.  This will be common
// with the rest of the tutorials since we have so much code now.  We didn't touch
// init.cpp at all, and we probably won't for a long time.  We create a CCamera class
// that holds our position, view, and up vector, along with a bunch of helper functions.
// If you still don't understand the up vector, don't worry about it.  You won't ever
// have to change it in my tutorials.  Really, just think of it as pointing up.  That's it.
//
// In this tutorial we also started to create our vector class, CVector3.  We will add
// to this class in the next tutorial.  Basically, this just holds an X Y and Z, or in
// other words, a 3D point.
// 
// SDLK_UP and SDLK_DOWN are the key codes for the UP and DOWN arrow on the keyboard.
// You can change this to what ever you want, but I would advise sticking to this
// for now, since I will be using it throughout the rest of my tutorials.
//
// Now that we have a class that holds our position and view, we can use that to
// pass into the gluLookAt() function.  No more hard coding numbers.  If we want
// to move, we have to use variables that hold the changes to our camera.
//
// We added another source file called Camera.cpp which will hold all of our camera code.
// Later, we will add an Input.cpp which will hold our input code, but that's later.
//
// Once again, I just wanted to make this tutorial as simple as possible.  Go on
// to the next tutorial if you are ready to rotate! :)
//
// * INPUT NOTE *
// 
// You will notice that the input is kinda jumpy and choppy.  That is because we
// are using the SDL_KEYDOWN method of checking for Input.  Later, in the next 
// tutorials we will do another method that gives us liquid fast results.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
