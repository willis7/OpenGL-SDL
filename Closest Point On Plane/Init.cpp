//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen         DigiBen@GameTutorials.com        //
//                                                                       //
//      $Program:       ClosestPointOnLine                               //
//                                                                       //
//      $Description:   Finds closest point on a line to another point   //
//                                                                       //
//      $Date:          2/23/02                                          //
//                                                                       //
//***********************************************************************//

#include "main.h"                                       // includes our function protoypes and globals                        

extern CVector3 vTriangle[3];
extern CVector3 g_vPosition;
extern float    g_rotateY;

int VideoFlags = 0;                                     // Video Flags for the Create Window function
SDL_Surface * MainWindow = NULL;                        // drawing surface on the SDL window

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
    CreateMyWindow("www.GameTutorials.com - Closest Point on Plane", SCREEN_WIDTH, SCREEN_HEIGHT, VideoFlags);

    // Initializes our OpenGL drawing surface
    Init();

    // Run our message loop
    MainLoop();

    // quit main returning success
    return 0;
}


//////////////////////////////////      HANDLE KEY PRESS EVENT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////     This function handles the keypress events generated when the user presses a key
//////
//////////////////////////////////      HANDLE KEY PRESS EVENT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void HandleKeyPressEvent(SDL_keysym * keysym)
{
    switch(keysym -> sym)                               // which key have we got
    {
        case SDLK_F5 :                                  // if it is F1
            ToggleFullScreen();                         // toggle between fullscreen and windowed mode
            break;

        case SDLK_ESCAPE:                               // if it is ESCAPE
            Quit(0);                                    // quit after cleaning up

        case SDLK_UP :                                  // if we hit the UP key
            g_vPosition.y += 0.01f;                     // move the sphere up
            break;
            
        case SDLK_DOWN :                                // if we hit the DOWN key
            g_vPosition.y -= 0.01f;                     // move the sphere down

        case SDLK_LEFT :                                // If we hit the LEFT arrow Key
            g_vPosition.x -= 0.01f;                     // move the sphere left along its plane
            break;

        case SDLK_RIGHT :                               // If we hit the RIGHT arrow key
            g_vPosition.x += 0.01f;                     // move the sphere right along its plane
            break;

        case SDLK_F3 :                                  // if we hit the F3 key
            g_vPosition.z -= 0.01f;                     // move the sphere in front of the poly
            break;

        case SDLK_F4 :                                  // if we hit the F4 key
            g_vPosition.z += 0.01f;                     // move the sphere in front of the poly
            break;

        case SDLK_F1 :                                  // check if we hit F1
            g_rotateY -= 2;                             // Rotate the Camera Left
            break;

        case SDLK_F2 :                                  // check if we hit F2
            g_rotateY += 2;                             // Rotate the camera right
            
        default:                                        // any other key
            break;                                      // nothing to do
    }
}


////////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
////////
////////      This will shutdown SDL and quit the program
////////
////////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Quit(int ret_val)
{
    SDL_Quit();                                         // shuts down SDL stuff

    exit(ret_val);                                      // quit the program
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Nothing new was changed in this file in regards to this tutorial.
// 
// 
// 
// DigiBen
// www.GameTutorials.com
// 
// 
