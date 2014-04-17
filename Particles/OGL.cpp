#include "OGL.h"
#include "SDL.h"
#include <iostream>


using namespace std;

extern int VideoFlags;

// This sets up "OpenGL" for us -- If an error occurs the method returns false
// otherwise the method returns true
bool GL_OBJ::Init( GLint bbp)
{
    // Set the pixel format
    //if(!setPixelFormat(bbp))
        //return false;

    return true; // OpenGL is up and running :)
}

// Sets the view port
bool GL_OBJ::setViewPort(GLint vp_wid, GLint vp_hgt, GLdouble fov)
{
    // Make sure we've been passed in a valid view port width and height
    if(vp_wid <= 0)
        return false;

    if(vp_hgt <= 0)
        return false;

    glViewport(0,0,vp_wid,vp_hgt);

    glMatrixMode(GL_PROJECTION); // Set projection matrix
    glLoadIdentity();

    // fov == Field of view in degrees
    // (GLdouble)vp_wid/(GLdouble)height == Aspect Ratio
    // 0.1f == Front clipping plane
    // 150.0f == Back clipping plane
    gluPerspective(fov,(GLdouble)vp_wid/(GLdouble)vp_hgt,0.1f,150.0f);

    glMatrixMode(GL_MODELVIEW); // Set the model view matrix
    glLoadIdentity();
        
    return true; // Success

}

// This is our "standard" OpenGL "begin to draw" code 
void GL_OBJ::startDraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // (0,0,5) == The eye (ie where are we looking from)
    // (0,0,0) == The reference point (ie what point are we looking at)
    // (0,1,0) == The up vector
    gluLookAt(0,0,5, 0,0,0, 0,1,0);
}


// This is our "standard" OpenGL "blit to screen, we've drawn everything" code
void GL_OBJ::endDraw() { SDL_GL_SwapBuffers(); }

// Free memory
GL_OBJ::~GL_OBJ()
{
}

void Quit(int ret_val)
{
    SDL_Quit();                                            // shuts down SDL stuff

    exit(ret_val);                                         // quit the program
}

/*************** Private class functions *******************/

// Sets pixel format
bool GL_OBJ::setPixelFormat(GLint bbp)
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
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   bbp);     // size of depth buffer 
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0);       // we aren't going to use the stencil buffer
    SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 0);     // this and the next three lines set the bits allocated (per pixel) -
    SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 0);   // - for the accumulation buffer to 0
    SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 0);

    return true; // Success
}
