// Done by TheTutor -- 11/2/01

/*    This tutorial shows ONE way to do "particles" in OpenGL -- Like everything in
    programming, are a lot of ways you can make a "particle" -- This is one simple
    effective way

    This is how we're going to make our particles:

    1)  Each particle will be comprised of two textures -- Both are "gray scale" bitmaps
        that have a "cool particle look" (these can EASILY be made with PhotoShop)
    2)  Each texture will be drawn to an OpenGL stock object "the quad" (ie a flat square)
    3)  Then to both quads we will add an "alpha blended color" so they look cooooool :)
    4)  Finally we will spin one of the quads around the z-axis so the particle has a
        "shimmering look"

    That will do it.  So without any other fanfare, it's particle time...
*/

// Make sure we load all the libraries we need

#define WIN_WID 800
#define WIN_HGT 600
#define SCREEN_DEPTH 16

#include "particle.h"
#include "SDL.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

// Globals ***

GL_OBJ gl_obj; // Our OpenGL object
SDL_Surface * MainWindow;                                                // Our drawable portion of the window
int VideoFlags = 0;                                                      // Our video flags

// *** End of Globals

// Changes the screen resolution
bool ChangeDisplayMode(int pixel_width, int pixel_height, bool switch_back = false);

// This locks the frame rate to "frame_rate" passed in -- Here we overload the
// input parameter so by default the frame rate will be 60 frames per second
bool LockFrameRate(int frame_rate = 60);

bool MasterInit();             // This initializes EVERYTING

void SizeOpenGLScreen(int width, int height)            // Initialize The GL Window
{
    if (height==0)                                      // Prevent A Divide By Zero error
    {
        height=1;                                        // Make the Height Equal One
    }

    glViewport(0,0,width,height);                        // Make our viewport the whole window
                                                        // We could make the view smaller inside
                                                        // Our window if we wanted too.
                                                        // The glViewport takes (x, y, width, height)
                                                        // This basically means, what our our drawing boundries

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix

                                                        // Calculate The Aspect Ratio Of The Window
                                                        // The parameters are:
                                                        // (view angle, aspect ration of the width to the height, 
                                                        //  The closest distance to the camera before it clips, 
                  // FOV        // Ratio                //  The farthest distance before it stops drawing)
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,150.0f);

    glMatrixMode(GL_MODELVIEW);                          // Select The Modelview Matrix
    glLoadIdentity();                                    // Reset The Modelview Matrix
}
void ToggleFullScreen(void)
{
    if(SDL_WM_ToggleFullScreen(MainWindow) == 0)        // try to toggle fullscreen mode for window 'main_window' 
    {
        cerr << "Failed to Toggle Fullscreen mode : " << SDL_GetError() << endl;   // report error in case toggle fails
        Quit(0);
    }
}
void KeyPressed(SDL_keysym * keysym)
{
    switch(keysym -> sym)
    {
        case SDLK_F1 :                      // If the key is F1 
            ToggleFullScreen();             // Toggle between FullScreen and windowed mode
            break;

        case SDLK_ESCAPE :                  // if the key is ESCAPE
            Quit(0);

        default:
             break;
    }
}
void CreateMyWindow(const char * strWindowName, int width, int height, int VideoFlags)
{
    MainWindow = SDL_SetVideoMode(width, height, SCREEN_DEPTH, VideoFlags);     // SCREEN_DEPTH is macro for bits per pixel

    if( MainWindow == NULL )                            // if window creation failed
    {
        cerr << "Failed to Create Window : " << SDL_GetError() << endl;         // report error 
        Quit(0);
    }

    SDL_WM_SetCaption(strWindowName, "GameTutorials");  // set the window caption (first argument) and icon caption (2nd arg)
}
int main(int argc , char **argv)
{
    SDL_Event event;
    bool done = false;

    cout << " Hit the F1 key to Toggle between Fullscreen and windowed mode" << endl;
    cout << " Hit ESC to quit" << endl;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )                      // try to initialize SDL video module
    {
        cerr << "Failed initializing SDL Video : " << SDL_GetError() << endl;      // report error if it fails
        return 1;                                             // we use this instead of Quit because SDL isn't yet initialized
    }
    // Create a window
    
    gl_obj.setPixelFormat(SCREEN_DEPTH);
    CreateMyWindow("www.GameTutorials.com - Particle", WIN_WID, WIN_HGT, VideoFlags);


    // If anything we try to initialize fails, we better quit
    if(!MasterInit())
    {
        FreeTextures(); // Free textures (if they we're allocated)
            return EXIT_FAILURE;
    }
    
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
        if( LockFrameRate())
        {
            gl_obj.startDraw();
            RunApp();
            gl_obj.endDraw();
        }
    } // while( ! done)


    FreeTextures(); // Free the textures

}

// Locks the frame rate at "frame_rate"
// Returns true when it's okay to draw, false otherwise
bool LockFrameRate(int frame_rate)
{
    static float lastTime = 0.0f;
    
    // Get current time in seconds (milliseconds * .001 = seconds)
    float currentTime = SDL_GetTicks() * 0.001f; 

    // Get the elapsed time by subtracting the current time from the last time
    // If the desired frame rate amount of seconds has passed -- return true (ie Blit())
    if((currentTime - lastTime) > (1.0f / frame_rate))
    {
        // Reset the last time
        lastTime = currentTime;    
            return true;
    }

    return false;
}

// Here we will attempt to initialize all our "objects"
bool MasterInit()
{
    // Initialize "OpenGL" using 16 bits per pixel for the color and depth bits
    if(gl_obj.Init(SCREEN_DEPTH) == false)
        return false;

    // Try and initialize the particles and textures
    if(Init() == false)
        return false;

    // Set up the view port
    gl_obj.setViewPort(WIN_WID,WIN_HGT,45);


    // Now set all necessary OpenGL flags
    glEnable(GL_TEXTURE_2D); // So we can texture map
    glEnable(GL_BLEND); // So we can "alpha blend"
    glShadeModel(GL_SMOOTH); // We'll have "smooth shading"
    
    // Set our alpha blending -- Take a look at MSDN for the big technical definition
    // of what's happening.  For the watered down highly non-technical version,
    // we're just taking all the source pixels (our textures) and blending them with
    // what ever color(s) our underneath them.  Also the "blend" is going towards the color
    // SOLID white (255,255,255) -- So in our case our two textures per 
    // particle will get blended since they overlap AND if two or more particles overlap
    // their colors will get blended together as well.
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    return true; // The app has been initialized

}

/*

    This tutorial doesn't do anything revolutionary -- It's intent was to simply show you
    a way to program "particles" -- What you do with your particles is totally up to you :)

*/

/*----------------------------*\
|  TheTutor                    |
|  thetutor@gametutorials.com  |
|  © 2001 GameTutorials        |
\*----------------------------*/
