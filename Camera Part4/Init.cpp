//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        DigiBen        DigiBen@GameTutorials.com       //
//                                                                       //
//        $Program:        Camera4                                       //
//                                                                       //
//        $Description:    Rotates the camera in 3rd person mode         //
//                                                                       //
//        $Date:            11/1/01                                      //
//                                                                       //
//***********************************************************************//

#include "main.h"                                       // includes our function protoypes and globals                        

# define kSpeed       0.02f                             // This is how fast our Camera moves
int VideoFlags = 0;                                     // Video Flags for the Create Window function
SDL_Surface * MainWindow = NULL;                        // drawing surface on the SDL window

bool upPressed = false , downPressed = false , leftPressed = false , rightPressed = false ;

/////////////////////////////////// TOGGLE FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
///////
///////   This function TOGGLES between FULLSCREEN and WINDOWED mode
///////
/////////////////////////////////// TOGGLE FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void ToggleFullScreen(void)
{
    if(SDL_WM_ToggleFullScreen(MainWindow) == 0)        // try to toggle fullscreen mode for window 'main_window' 
    {
        cerr << "Failed to Toggle Fullscreen mode : " << SDL_GetError() << endl;   // report error in case toggle fails
        Quit(0);
    }
}


///////////////////////////////   CREATE MY WINDOW   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
////////
////////  This function CREATES our WINDOW for drawing the GL stuff
////////
///////////////////////////////   CREATE MY WINDOW   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateMyWindow(const char * strWindowName, int width, int height, int VideoFlags)
{
    MainWindow = SDL_SetVideoMode(width, height, SCREEN_DEPTH, VideoFlags);     // SCREEN_DEPTH is macro for bits per pixel

    if( MainWindow == NULL )                            // if window creation failed
    {
        cerr << "Failed to Create Window : " << SDL_GetError() << endl;         // report error 
        Quit(0);
    }

    SDL_WM_SetCaption(strWindowName, strWindowName);    // set the window caption (first argument) and icon caption (2nd arg)
}


/////////////////////////////   SETUP PIXEL FORMAT   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
///////
///////  Sets the pixel format for openGL and video flags for SDL
///////
/////////////////////////////   SETUP PIXEL FORMAT   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void SetupPixelFormat(void)
{
    //////// SURFACE IS THE DRAWABLE PORTION OF AN SDL WINDOW \\\\\\\\*
    
    /////////////  we set the common flags here
    VideoFlags    = SDL_OPENGL;                         // it's an openGL window
    VideoFlags   |= SDL_HWPALETTE;                      // exclusive access to hardware colour palette
    VideoFlags   |= SDL_RESIZABLE;                      // the window must be resizeable

    const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();     // query SDL for information about our video hardware

    if(VideoInfo == NULL)                                     // if this query fails
    {
        cerr << "Failed getting Video Info : " << SDL_GetError() << endl;          // report error
        Quit(0);
    }

    ///////////// we set the system dependant flags here
    if(VideoInfo -> hw_available)                      // is it a hardware surface
        VideoFlags |= SDL_HWSURFACE;
    else
        VideoFlags |= SDL_SWSURFACE;

    // Blitting is fast copying / moving /swapping of contiguous sections of memory
    // for more about blitting check out : http://www.csc.liv.ac.uk/~fish/HTML/blitzman/bm_blitter.html
    if(VideoInfo -> blit_hw)                            // is hardware blitting available
        VideoFlags |= SDL_HWACCEL;

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );      // tell SDL that the GL drawing is going to be double buffered
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   SCREEN_DEPTH);         // size of depth buffer 
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0);       // we aren't going to use the stencil buffer
    SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 0);     // this and the next three lines set the bits allocated per pixel -
    SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 0);   // - for the accumulation buffer to 0
    SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 0);
}


//////////////////////////// RESIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function resizes the viewport for OpenGL.
/////
//////////////////////////// RESIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void SizeOpenGLScreen(int width, int height)            // Initialize The GL Window
{
    if (height==0)                                      // Prevent A Divide By Zero error
    {
        height=1;                                       // Make the Height Equal One
    }

    glViewport(0,0,width,height);                       // Make our viewport the whole window
                                                        // We could make the view smaller inside
                                                        // Our window if we wanted too.
                                                        // The glViewport takes (x, y, width, height)
                                                        // This basically means, what our our drawing boundries

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                   // Reset The Projection Matrix

                                                        // Calculate The Aspect Ratio Of The Window
                                                        // The parameters are:
                                                        // (view angle, aspect ration of the width to the height, 
                                                        //  The closest distance to the camera before it clips, 
                  // FOV        // Ratio                //  The farthest distance before it stops drawing)
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,150.0f);

    glMatrixMode(GL_MODELVIEW);                         // Select The Modelview Matrix
    glLoadIdentity();                                   // Reset The Modelview Matrix
}


/////////////////////////////////   INITIALIZE OPENGL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////         This function handles all the initialization for openGL
//////
/////////////////////////////////   INITIALIZE OPENGL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void InitializeGL(int width, int height)
{
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    SizeOpenGLScreen(width, height);                    // resize the OpenGL Viewport to the given height and width
}

//////////////////////////////      MAIN      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////     create the window and calling the initialization functions
//////
//////////////////////////////      MAIN      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int main(void)
{
    // print user instructions
    cout << " Hit the F1 key to Toggle between Fullscreen and windowed mode" << endl;
    cout << " Hit ESC to quit" << endl;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )                // try to initialize SDL video module
    {
        cerr << "Failed initializing SDL Video : " << SDL_GetError() << endl;      // report error if it fails
        return 1;                                       // we use this instead of Quit because SDL isn't yet initialized
    }

    // Set up the format for the pixels of the OpenGL drawing surface
    SetupPixelFormat();
    // Create our window, we pass caption for the window, the width, height and video flags required
    CreateMyWindow("www.GameTutorials.com - 3rd Person Camera", SCREEN_WIDTH, SCREEN_HEIGHT, VideoFlags);

    // Initializes our OpenGL drawing surface
    Init();

    // Run our message loop
    MainLoop();

    // quit main returning success
    return 0;
}

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function handles the input faster
/////
//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void keyPressed()
{
    // This might look like it's not too much better than what we had before,
    // but believe me, it responds a whole lot better.
    // but when we want to
    // check for constant input like MOVING, we need a faster method.  You noticed
    // that we moved really choppy in the previous tutorial.
    

    if(upPressed) {                // If we hit the UP arrow key
        g_Camera.MoveCamera(kSpeed);            // Move our camera forward by a positive speed
    }

    if(downPressed) {            // If we hit the DOWN arrow key
        g_Camera.MoveCamera(-kSpeed);            // Move our camera backward by a negative speed
    }
    
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // We made 2 changes from the Camera2 tutorial.  Instead of gCamera.RotateView()
    // we use our new RotateAroundPoint() function.  We pass in the camera's view
    // point.  This will be the point that we rotate our camera position around.
    
    if(leftPressed) {            // If we hit the LEFT arrow key

        // We want to rotate around the Y axis so we pass in a positive Y speed
        g_Camera.RotateAroundPoint(g_Camera.m_vView, kSpeed, 0, 1, 0);    
    }

    if(rightPressed) {            // If we hit the RIGHT arrow key

        // Use a negative Y speed to rotate around the Y axis
        g_Camera.RotateAroundPoint(g_Camera.m_vView, -kSpeed, 0, 1, 0);    
    }

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

}

//////////////////////////////////      HANDLE KEY PRESS EVENT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////     This function handles the keypress events generated when the user presses a key
//////
//////////////////////////////////      HANDLE KEY PRESS EVENT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void HandleKeyPressEvent(SDL_keysym * keysym)
{
    switch(keysym -> sym)                                  // which key have we got
    {
        case SDLK_F1 :                                     // if it is F1
            ToggleFullScreen();                            // toggle between fullscreen and windowed mode
            break;

        case SDLK_ESCAPE:                                  // if it is ESCAPE
            Quit(0);                                       // quit after cleaning up

        case SDLK_UP :                                     // If we hit the UP arrow Key
            upPressed = true;                              // make upPressed true
            break;
            
       case SDLK_DOWN :                                    // If we hit the Down arrow Key
            downPressed = true;                            // make downPressed true
            break;

       case SDLK_RIGHT :                                   // If we hit the right arrow Key
            rightPressed = true;                           // make rightPressed true
            break;

       case SDLK_LEFT :                                    // If we hit the left arrow key
            leftPressed = true;                            // make the leftPressed true
            break;

        default:                                           // any other key
            break;                                         // nothing to do
    }
}

//////////////////////////////////      HANDLE KEY RELEASE EVENT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////     This function handles the keyrelease events generated when the user release a key
//////
//////////////////////////////////      HANDLE KEY RELEASE EVENT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void HandleKeyReleaseEvent(SDL_keysym * keysym)
{
    switch(keysym -> sym)                                  // which key have we got
    {
        case SDLK_UP :                                     // If we release the UP arrow Key
            upPressed = false;                             // make it false 
            break;
            
        case SDLK_DOWN :                                   // If we release the Down arrow Key
            downPressed = false;                           // make downPressed false 
            break;

        case SDLK_RIGHT :                                  // if we release the right arrow key
            rightPressed = false;                          // make the rightPressed false
            break;

        case SDLK_LEFT :                                   // if we release the left arrow key 
            leftPressed = false;                           // make leftPressed false
            break;

        default:                                           // any other key
            break;                                         // nothing to do
    }
}

////////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
////////
////////      This will shutdown SDL and quit the program
////////
////////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Quit(int ret_val)
{
    SDL_Quit();                                            // shuts down SDL stuff

    exit(ret_val);                                         // quit the program
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Nothing new was added to this file since the last tutorial (Camera2).
// 
// 
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
