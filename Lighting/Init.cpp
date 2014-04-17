//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        DigiBen        digiben@gametutorials.com       //
//                                                                       //
//        $Program:        Lighting                                      //
//                                                                       //
//        $Description:    This Draws rotating pyramids with lighting.   //
//                                                                       //
//        $Date:            7/5/01                                       //
//                                                                       //
//***********************************************************************//

#include "main.h"                                       // includes our function protoypes and globals                        

int VideoFlags = 0;                                     // Video Flags for the Create Window function
SDL_Surface * MainWindow = NULL;                        // drawing surface on the SDL window


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
/////
/////         This function handles all the initialization for openGL
/////
/////////////////////////////////   INITIALIZE OPENGL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void InitializeOpenGL(int width, int height)
{
    glEnable( GL_DEPTH_TEST );                           // Enables Depth Test i.e enables use of depth buffer for drawing

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // It's a good idea to have the color of our background the same color as our darkness.
    // Since we are using white as our light color, we will set our background color to black.

    glClearColor(0, 0, 0, 1);                            // Set our background color to black (0, 0, 0)

    // Below we create 2 arrays of color.  The first one, ambience[], is the default color
    // when there is no light shinning on the polygon.  If we didn't have an ambience, the 
    // polygon's would be extremely dark so you couldn't see them.  Since we are using pure white
    // as our light color, we will set the ambience half of that, so it's not TOO dark when the light
    // isn't shinning on the surface of a polygon.  The second array, "diffuse", is the color
    // of the light that is shinning.  We chose pure white.  The first 3 are the R G B values.
    // (0, 0, 0) is pure black, where (1, 1, 1) is pure white. The last index is the alpha value.
    // Don't worry about that now.  For our ambience color we choose a dark grey light source.
    // For our diffuse color, we chose a half and half between full black light and full white.

    float ambience[4] = {0.3f, 0.3f, 0.3f, 1.0};        // The color of the light in the world
    float diffuse[4] = {0.5f, 0.5f, 0.5f, 1.0};         // The color of the positioned light

    // To set the ambience of our world, we need to give OpenGL our array of values.
    // OpenGL allows us to have multiple lights.  The number of lights is dependant on GL_MAX_LIGHTS.
    // We will use a OpenGL define for the first usable light, GL_LIGHT0.  After we specify a
    // light to use, we pass in the flag that tells OpenGL we are setting the ambience, followed
    // by our ambience array.  To set the diffuse color, we do the same thing but use GL_DIFFUSE
    // and our diffuse array.

    glLightfv( GL_LIGHT0, GL_AMBIENT,  ambience );       // Set our ambience values (Default color without direct light)
    glLightfv( GL_LIGHT0, GL_DIFFUSE,  diffuse );        // Set our diffuse color (The light color)

    // Next, we need to set the POSITION of our light.  We can have many lights at many positions.
    // In this tutorial, we will just do one light, but feel free to add more.  Use GL_LIGHT1 next.
    // We have our light position global so we can change it with the '+' and '-' keys.
    // Our default posiiton stored in g_LightPosition is (0, 1, 0, 1).  This should place our
    // light position directly above the middle pyramid.  The last value in the g_LightPosition array
    // tells OpenGL if we want our light position to be a DIRECTION or a POSITION.  If the
    // value is a 1, that means it is just a position, otherwise it's a direction from our camera.
    // If we have it a direction , it lights up everything in that direction (or vector) from
    // our eye, or in other words our camera.  If we say it's a position, it takes the values
    // as an X Y and Z position in the world.  Then it lights up everything around it.  That is
    // what we want, so we have a 1 to say that.  Once again, we pass in the light we are setting,
    // then GL_POSITION to let OpenGL we are setting the light position, the pass in our array
    // holding the position values.

    glLightfv( GL_LIGHT0, GL_POSITION, g_LightPosition );    // This Sets our light position

    // After we initialize a particular light, we need to turn it on.
    // We use the glEnable() function to do so, then pass in the light to turn on.

    glEnable(  GL_LIGHT0   );                                // Turn this light on

    // That is enough though, we still need to allow lighting to take place.
    // Just because the light switch is on, doesn't mean we have power :)
    // This works just like fog, once you init everything, you need to allow it to be displayed.

    glEnable(  GL_LIGHTING );                                // This turns on lighting

    // This next line allows us to use colors on our polygons during lighting.
    // If we did not have this line it would turn our pyramids white. 

    glEnable(GL_COLOR_MATERIAL);                             // Allow color to show during lighting

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    SizeOpenGLScreen(width, height);                         // Setup the screen translations and viewport


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
    CreateMyWindow("www.GameTutorials.com - Lighting", SCREEN_WIDTH, SCREEN_HEIGHT, VideoFlags);

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
// Remember, if we are using the color functions glColor*(), the are ignored
// when we enable lighting unless we enable GL_COLOR_MATERIALS.  
// We will want to use texture mapping to simulate better lighting.
//
// In this file we initialized lighting.  This was done in InitializeOpenGL().
// Here are the list of steps that need to take place:
//
// 1) First we need to set the ambience of our world: (The non lit color)
//  
//            glLightfv( GL_LIGHT0, GL_AMBIENT,  ambienceArray );
//
// 2) Next we need to set the diffuse of our light:  (The light color)
//
//            glLightfv( GL_LIGHT0, GL_DIFFUSE,  diffuseArray);
//
// 3) Next, we need to set the light's position:
//
//            glLightfv( GL_LIGHT0, GL_POSITION, positionArray);
//
// 4) Next, once we initialized the light, we need to turn it on:
//
//            glEnable(  GL_LIGHT0   );                   // Choose the light we want to turn on
//
// 5) Finally, we need to turn lighting on so our individual lights have power:
//
//            glEnable(  GL_LIGHTING)                     // Turn lighting on (glDisable(GL_LIGHTING) turns it off)
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
