#ifndef OGL_H
#define OGL_H

#include <GL/gl.h>          // Header File For The OpenGL32 Library
#include <GL/glu.h>         // Header File For The GLu32 Library
#include <stdlib.h>         // for routines like exit()

// This is our OpenGL object that just sets up OpenGL for us

class GL_OBJ
{
    public:

        // Constructor -- Just set everything to NULL
        GL_OBJ() {}
    
        // Inits "OpenGL" -- Uses bitsPerPixel to specify the 
        // color bits and depth bits -- Returns true on success, false on failure
        bool Init(GLint bitsPerPixel);

        // Sets the view port and field of view
        // Returns true on success, false on failure
        bool setViewPort(GLint vp_wid, GLint vp_hgt, GLdouble fov);

        void startDraw(); // Gets ready to draw
        void endDraw(); // Blit to screen
        bool setPixelFormat(GLint bitsPerPixel);

        ~GL_OBJ(); // Deconstructor
};


#endif    // OGL_H
