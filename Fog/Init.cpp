//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     digiben@gametutorials.com            //
//                                                                       //
//      $Program:       Fog                                              // 
//                                                                       //
//      $Description:   This Draws rotating pyramids with adjustable fog.// 
//                                                                       //
//      $Date:          7/4/01                                           //
//                                                                       //
//***********************************************************************//


#include "main.h"                                       // includes our function protoypes and globals                        

int VideoFlags = 0;                                     // Video Flags for the Create Window function
SDL_Surface * MainWindow = NULL;                        // drawing surface on the SDL window


///////////////////////////////////      SWAP      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////  This swaps 2 elements we pass to it
/////
///////////////////////////////////      SWAP      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

static void swap(unsigned char & a, unsigned char & b)
{
    unsigned char temp;

    temp = a;
    a    = b;
    b    = temp;

    return;
}

/////////////////////////////////// TOGGLE FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function TOGGLES between FULLSCREEN and WINDOWED mode
/////
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
/////
/////  This function CREATES our WINDOW for drawing the GL stuff
/////
///////////////////////////////   CREATE MY WINDOW   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

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


/////////////////////////////   SETUP PIXEL FORMAT   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////  Sets the pixel format for openGL and video flags for SDL
/////
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
    if (height==0)                                        // Prevent A Divide By Zero error
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

    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
    glLoadIdentity();                                    // Reset The Modelview Matrix
}


/////////////////////////////////   INITIALIZE OPENGL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////         This function handles all the initialization for openGL
/////
/////////////////////////////////   INITIALIZE OPENGL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void InitializeOpenGL(int width, int height)
{
    glEnable( GL_DEPTH_TEST );                             // Enables Depth Test i.e enables use of depth buffer for drawing
    glEnable( GL_TEXTURE_2D );                             // Enable Texture Mapping 


    /////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // This function clears the backbuffer and turns it to a desired color.
    // Since we are using blue fog, we want the background color to be blue.
    // The first 3 values are R G B from 0.0 to 1.0 (0 to 255 ratio) with the last an alpha value.

    glClearColor(0, 0, 1, 1);                            // Set the background color to blue

    // Below is the initialization of our fog.  First off, we need to choose
    // a fog color.  Since there is a potentional for an RGBA color, we need to
    // give it 4 values.  The first 3 values are the Red Green and Blue color value.
    // The last value is the alpha.  The default is (0, 0, 0, 0) so we want to change that.
    // We put our colors in an array because glFogfv takes them that way.
    // We chose blue for our fog color.

    float fogColor[4] = {0.0f,0.0f,1.0f,1.0f};            // Let's make the Fog Color BLUE

    // Now that we have our 4 colors, let's set the fog mode.  I chose GL_EXP2, but the default
    // is GL_EXP.  There is also a GL_LINEAR.  Try them all to get a feel for the type of fog
    // you are looking for.  They are computed each with different equations.  I think the
    // exponential ones look the best. (f = e^(- density * z)) (Z is the eye or camera position)

    glFogi(GL_FOG_MODE, GL_EXP2);                        // Fog Mode

    // We pass in our array to set the fog color
    glFogfv(GL_FOG_COLOR, fogColor);                    // Set Fog Color

    // We also have a option to change the fog thickness, or "density".
    // The density is default 1.0, but we will start with 0.2 (g_FogDensity).
    glFogf(GL_FOG_DENSITY, g_FogDensity);                // How Dense Will The Fog Be

    // The "FOG_HINT" is how accurate the fog is calculate.  We don't really care,
    // so we say so :)  GL_DONT_CARE let's openGL choose either per vertex or per pixel fog.
    // Otherwise, we have the option to choose GL_NICEST or GL_FASTEST if desired.
    glHint(GL_FOG_HINT, GL_DONT_CARE);                    // The Fog's calculation accuracy

    // This is the important part.  This sets the start of the fog from our screen where 
    // the fog density will effect our world, coming from our camera.  Everything outside 
    // of our START and END will appear the solid color of our fog.  Think of it as our 
    // fall off distance.  I usually set this distance to be the same as our fall off
    // distance (Where OpenGL stops drawing far polys) set in gluPerspective().
    // The default is 0 so we don't need to set it, but you should know how
    glFogf(GL_FOG_START, 0);                            // Fog Start Depth

    // This is the END of the start distance.  The default is 1.0, but we want it
    // to go 10 units into the screen.  Remember this is a distance, not an actuall coordinate.
    // Basically, if we have anything past 10 units from the camera it will be covered in fog.
    glFogf(GL_FOG_END, 10.0f);                            // Fog End Depth
    
    // Finally, once we initialize all of our fog data, we need to tell OpenGL to turn on fog.
    // To turn fog off, you can call glDisable(GL_FOG);

    glEnable(GL_FOG);                                    // This enables our OpenGL Fog

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    SizeOpenGLScreen(width, height);                       // resize the OpenGL Viewport to the given height and width
}


////////////////////////////////      MAIN      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////     create the window and calling the initialization functions
//////
////////////////////////////////      MAIN      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int main(void)
{
    // print user instructions
    cout << " Hit the F1 key to Toggle between Fullscreen and windowed mode" << endl;
    cout << " Hit ESC to quit" << endl;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )                      // try to initialize SDL video module
    {
        cerr << "Failed initializing SDL Video : " << SDL_GetError() << endl;      // report error if it fails
        return 1;                                             // we use this instead of Quit because SDL isn't yet initialized
    }

    // Set up the format for the pixels of the OpenGL drawing surface
    SetupPixelFormat();
    // Create our window, we pass caption for the window, the width, height and video flags required
    CreateMyWindow("www.GameTutorials.com - Fog", SCREEN_WIDTH, SCREEN_HEIGHT, VideoFlags);

    // Initializes our OpenGL drawing surface
    
    Init();

    // Run our message loop
    MainLoop();

    // quit main returning success
    return 0;
}


////////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////      This will shutdown SDL and quit the program
//////
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
// We initialized our fog in this file in InitializeOpenGL().  It's VERY simple and intuitive.
// Here are the steps to take when generating OpenGL fog:
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
// 
// DigiBen
// www.GameTutorials.com
//
//
