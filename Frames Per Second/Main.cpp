//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        Ben Humphrey    digiben@gametutorials.com      //
//                                                                       //
//        $Program:       Frames Per Second                              //
//                                                                       //
//        $Description:   Demonstrates calculating the frames per second //
//                                                                       //
//        $Date:          1/03/02                                        //
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

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// CALCULATE FRAMES PER SECOND \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function calculates the frames we render each second and displays it to the window title
/////
///////////////////////////////// CALCULATE FRAMES PER SECOND \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CalculateFrameRate()
{
    // Below we create a bunch of static variables because we want to keep the information
    // in these variables after the function quits.  We could make these global but that would
    // be somewhat messy and superfluous.  Note, that normally you don't want to display this to
    // the window title bar.  This is because it's slow and doesn't work in full screen.
    // Try using the 3D/2D font's.  You can check out the tutorials at www.gametutorials.com.

    static float framesPerSecond    = 0.0f;                            // This will store our fps
    static float lastTime            = 0.0f;                           // This will hold the time from the last frame
    static char strFrameRate[50] = {0};                                // We will store the string here for the window title

    // Here we get the current tick count and multiply it by 0.001 to convert it from milliseconds to seconds.
    // GetTickCount() returns milliseconds (1000 ms = 1 second) so we want something more intuitive to work with.
    float currentTime = SDL_GetTicks() * 0.001f;                

    // Increase the frame counter
    ++framesPerSecond;

    // Now we want to subtract the current time by the last time that was stored.  If it is greater than 1
    // that means a second has passed and we need to display the new frame rate.  Of course, the first time
    // will always be greater than 1 because lastTime = 0.  The first second will NOT be true, but the remaining
    // ones will.  The 1.0 represents 1 second.  Let's say we got 12031 (12.031) from GetTickCount for the currentTime,
    // and the lastTime had 11230 (11.230).  Well, 12.031 - 11.230 = 0.801, which is NOT a full second.  So we try again
    // the next frame.  Once the currentTime - lastTime comes out to be greater than a second (> 1), we calculate the
    // frames for this last second.
    if( currentTime - lastTime > 1.0f )
    {
        // Here we set the lastTime to the currentTime.  This will be used as the starting point for the next second.
        // This is because GetTickCount() counts up, so we need to create a delta that subtract the current time from.
        lastTime = currentTime;
        
        // Copy the frames per second into a string to display in the window title bar
        sprintf(strFrameRate, "Current Frames Per Second: %d", int(framesPerSecond));

        // Set the window title bar to our string
        SDL_WM_SetCaption(strFrameRate,"GameTutorials");            // The First argument is the window title

        // Reset the frames per second
        framesPerSecond = 0;
    }
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
    
        //       Position      View       Up Vector
    gluLookAt(0, 0, 6,     0, 0, 0,     0, 1, 0);          // This determines where the camera's position and view is

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Here we create a static variable that will count forever.  This is used for the rotation degree
    static float rotY = 0;

    // Pass in our current rotation value to glRotatef() for rotation around the Y axis (0, 1, 0)
    glRotatef(rotY, 0, 1, 0);

    // Increase the rotation by 2 degrees
    rotY += 2;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

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

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Call our new function to count and calculate the frames for each second.
    // This needs to be called every time the screen renders so we get a correct frame rate.
    // Because of its static variables inside, we don't need any globals or outside variables.
    CalculateFrameRate();

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Pretty easy huh?  This tutorial just shows a simple way to calculate the frames per second.
// The best way to do this would be to use time().  time()
// is the most accurate way to check the system clock, but isn't absolutely necessary.
//
// Keep in mind that you don't ideally want to show the frames per second in the title bar.
// This slows down the application a bit, so it is best to use a font that is rendered in the view port.
//
// Another thing to recognize is that OpenGL will only render 60 frames per second if your monitor's mhz
// is set to 60 mhz, etc...  If you want to have higher frames rates you will need to increase your
// monitor frequency.  This is because OpenGL waits for the synchronization of the monitor before rendering
// each frame.  
//
// Once again, a tutorial that every graphics programmer must have.
//
// Enjoy!
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
