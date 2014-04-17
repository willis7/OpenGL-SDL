//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        DigiBen        digiben@gametutorials.com       //
//                                                                       //
//        $Program:        RenderToTexture                               //
//                                                                       //
//        $Description:    Demonstrates how to render screen to a texture//
//                                                                       //
//        $Date:            2/27/02                                      //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file

UINT g_Texture[MAX_TEXTURES];                            // This will reference to our texture data stored with OpenGL UINT is an unsigned int (only positive numbers)

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
//
// This tutorial shows you how to render to a texture map.  In the application
// we render a spinning cube to a texture map, then bind that texture map to a
// spinning QUAD.  You won't ever see the spinning cube rendered, but you will see
// it in the constantly updated texture map.  This technique is done by creating
// a empty texture that has RGB data initialized to 0.  The size of the texture
// map will be at most 512 by 512.  Anything bigger than this won't work (depending
// on your video card) or will be a huge memory footprint that will slow your program
// down immensely.  Next we shrink the current viewport to either 128 x 128, 256 x 256
// or 512 x 512.  The smaller the viewport, the decrease in quality for the texture.   
// Once the new viewport is set, we render the cube, bind the render texture and
// call glCopyTexImage2D().  This wonderful hardware optimizes function copies the 
// screen pixel data to the current bound texture.  Now we should have our texture
// map rendered too.  We will want to clear the screen, then put our viewport back to
// it's original size.  We can now use this texture to texture map a QUAD to see
// what the rendered result was.  More detail will be explain in the code.
// 
// Let's go over the controls for this app:
//
// '1' - This changes the texture size to 128 (LOW QUALITY)
// '2' - This changes the texture size to 256 (ACCEPTABLE QUALITY)
// '3' - This changes the texture size to 512 (HIGH QUALITY)
// ESCAPE Key - This quits the program
//
// This tutorial was essentially taken from the texture mapping tutorial
//

             
// This stores our current rotation
float g_RotateX = 0, g_RotateY = 0, g_RotateZ = 0;

// This will store the size of the viewport that we render the texture from.
int g_Viewport = 512;

// This is our prototype for the function that creates the empty texture to render to
void CreateRenderTexture(UINT textureArray[], int size, int channels, int type, int textureID);


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init(void)
{
    InitializeOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);        // Init OpenGL with the global rect


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    glEnable(GL_DEPTH_TEST);                            // Turn depth testing on
    glEnable(GL_TEXTURE_2D);                            // Enable Texture Mapping

    // This is where we initialize our empty texture to be rendered too.
    // We pass in the texture array to store it, the texture size,
    // the channels (3 for R G B), the type of texture (RGB) and texture ID.
    CreateRenderTexture(g_Texture, 512, 3, GL_RGB, 0);

    // Create the texture map for the spinning cube and assign it to texture ID 1
    CreateTexture(g_Texture, "Brick.bmp", 1);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


}


///////////////////////////////// MAIN GAME LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function Handles the main game loop
/////
///////////////////////////////// MAIN GAME LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

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

        RenderScene();                                      // draw our OpenGL scene
    } // while( ! done)
}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////////// CREATE BOX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function creates a textured box (if textureID >= 0) with a center and dimension
/////
///////////////////////////////////// CREATE BOX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateBox(int textureID, float x, float y, float z, float width, 
               float height,  float length, float uscale, float vscale)
{
    // This function is a lot like our CreateSkyBox() function in the sky box tutorial.
    // It creates a box with specific dimension around a center point.
    // The first parameter is the texture ID to render from, or < 0 for no texture.
    // The next parameters are the center position, the dimensions and UV scaling.
    // This will be used to render the spinnin cube we see rendered to the texture.
    
    // Check if we have a valid texture ID, then bind that texture to our box
    if(textureID >= 0) glBindTexture(GL_TEXTURE_2D, g_Texture[textureID]);

    // This centers the box around (x, y, z)
    x = x - width  / 2;
    y = y - height / 2;
    z = z - length / 2;

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);        
        
        // Assign the texture coordinates and vertices for the BACK Side
        glTexCoord2f(0.0f,     0.0f); glVertex3f(x,            y,            z);
        glTexCoord2f(0.0f,   vscale); glVertex3f(x,            y + height, z);
        glTexCoord2f(uscale, vscale); glVertex3f(x + width, y + height, z); 
        glTexCoord2f(uscale,   0.0f); glVertex3f(x + width, y,            z);
    
        // Assign the texture coordinates and vertices for the FRONT Side
        glTexCoord2f(uscale,   0.0f); glVertex3f(x,            y,            z + length);
        glTexCoord2f(uscale, vscale); glVertex3f(x,            y + height, z + length);
        glTexCoord2f(0.0f,   vscale); glVertex3f(x + width, y + height, z + length); 
        glTexCoord2f(0.0f,     0.0f); glVertex3f(x + width, y,            z + length);
    
        // Assign the texture coordinates and vertices for the BOTTOM Side
        glTexCoord2f(uscale,   0.0f); glVertex3f(x,            y,            z);
        glTexCoord2f(uscale, vscale); glVertex3f(x,            y,            z + length);
        glTexCoord2f(0.0f,   vscale); glVertex3f(x + width, y,            z + length); 
        glTexCoord2f(0.0f,     0.0f); glVertex3f(x + width, y,            z);

        // Assign the texture coordinates and vertices for the TOP Side
        glTexCoord2f(uscale, vscale); glVertex3f(x,            y + height,    z);
        glTexCoord2f(uscale,   0.0f); glVertex3f(x,            y + height,    z + length);
        glTexCoord2f(0.0f,     0.0f); glVertex3f(x + width, y + height, z + length); 
        glTexCoord2f(0.0f,   vscale); glVertex3f(x + width, y + height, z);
    
        // Assign the texture coordinates and vertices for the LEFT Side
        glTexCoord2f(uscale,   0.0f); glVertex3f(x,            y,            z);
        glTexCoord2f(0.0f,     0.0f); glVertex3f(x,            y,            z + length);
        glTexCoord2f(0.0f,   vscale); glVertex3f(x,            y + height,    z + length); 
        glTexCoord2f(uscale, vscale); glVertex3f(x,            y + height,    z);

        // Assign the texture coordinates and vertices for the RIGHT Side
        glTexCoord2f(0.0f,     0.0f); glVertex3f(x + width, y,            z);
        glTexCoord2f(uscale,   0.0f); glVertex3f(x + width, y,            z + length);
        glTexCoord2f(uscale, vscale); glVertex3f(x + width, y + height,    z + length); 
        glTexCoord2f(0.0f,   vscale); glVertex3f(x + width, y + height,    z);

    // Stop drawing quads
    glEnd();
}


///////////////////////////////// CREATE RENDER TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function creates a blank texture to render too
/////
///////////////////////////////// CREATE RENDER TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
    
void CreateRenderTexture(UINT textureArray[], int size, int channels, int type, int textureID)                                        
{
    // Create a pointer to store the blank image data
    unsigned int *pTexture = NULL;                                            

    // We need to create a blank texture to render our dynamic texture too.
    // To do this, we just create an array to hold the data and then give this
    // array to OpenGL.  The texture is stored on the video card so we can
    // destroy the array immediately afterwards.
    // This function takes the texture array to store the texture, the
    // size of the texture for width and the heigth, the channels (1, 3 or 4),
    // the type (LUMINANCE, RGB, RGBA, etc..) and the texture ID to assign it too.

    // Allocate and init memory for the image array and point to it from pTexture
    pTexture = new unsigned int [size * size * channels];
    memset(pTexture, 0, size * size * channels * sizeof(unsigned int));    

    // Register the texture with OpenGL and bind it to the texture ID
    glGenTextures(1, &textureArray[textureID]);                                
    glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);                    
    
    // Create the texture and store it on the video card
    glTexImage2D(GL_TEXTURE_2D, 0, channels, size, size, 0, type, GL_UNSIGNED_INT, pTexture);                        
    
    // Set the texture quality
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // Since we stored the texture space with OpenGL, we can delete the image data
    delete [] pTexture;                                                                                    
}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
    
void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                    // Reset The matrix


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    //            Position        View         Up Vector
    gluLookAt(0, 0, 4,     0, 0, 0,     0, 1, 0);        // Move the camera back

    // Here we change the background color to a bright blue.
    // We will change the background of the screen to black for when
    // we render the cube to the texture map.  The parameters are (r, g, b, a).
    glClearColor(0.0f, 0.0f, 1.0f, 1);                    // Turn the background color blue    

    // We want to rotate both the cube and the QUAD along the Y axis
    glRotatef(g_RotateY, 0.0f, 1.0f, 0.0f);                // Rotate around the Y-axis

    glPushMatrix();                                        // Push on a new matrix scope

        // We only want to rotate the cube around the x and z axis
        glRotatef(g_RotateX, 1.0f, 0.0f, 0.0f);            // Rotate around the X-axis    
        glRotatef(g_RotateZ, 0.0f, 0.0f, 1.0f);            // Rotate around the Z-axis    
        
        // Now comes the part where we actually render to the texture.
        // First, let's create a small viewport to render to.  This way we
        // don't create a huge texture that is the size of our screen width and height.
        // We can then render from this screen size to the texture map.

        // Shrink the viewport by the current viewport size.  We have an option
        // to change the viewport in real time by pressing 1, 2, or 3.  You will see
        // the difference in quality between 128, 256 and 512 texture sizes.
        glViewport(0, 0, g_Viewport, g_Viewport);                                

        // Now we get to render the spinning cube to the texture.  We just
        // need to create a cube, and it will be effected by the current rotation.
        // The parameters are the texture ID, the center (x,y,z), dimentions and uv scale.
        CreateBox(1,    0, 0, 0,    1, 1, 1,    1, 1);

        // Before we copy the screen to a texture, we need to specify the current
        // texture to draw to by calling glBindTexture() with the appropriate texture 
        glBindTexture(GL_TEXTURE_2D,g_Texture[0]);                

        // Now comes the moment we have all been waiting for, we render the screen
        // to the texture.  We pass in the texture type, detail level (0), pixel format,
        // the x and y position to start from, the width and height to grab, and a border.
        // If you only want a part of the screen, this works great for that.
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, g_Viewport, g_Viewport, 0);

        // Here we clear the screen and depth bits of the small viewport
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);            

        // Set our viewport back to it's normal size
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);    

        // Make the color of the background black for the texture map
        glClearColor(0.0f, 0.0f, 0.0f, 0.5);                    
    
    glPopMatrix();                                        // Return matrix back to normal

    // Now we have a valid texture map of the rendered spinning cube.  Let's
    // bind this texture to our spinning QUAD.
    glBindTexture(GL_TEXTURE_2D, g_Texture[0]);

    glBegin(GL_QUADS);                                    // Start rendering QUADS        
        
        // Create a texture mapped QUAD that has the new texture map on it
        glTexCoord2f(   1, 0.0f);    glVertex3f(-1, -1,  0);
        glTexCoord2f(   1,    1);    glVertex3f(-1,  1,  0);
        glTexCoord2f(0.0f,    1);    glVertex3f( 1,  1,  0); 
        glTexCoord2f(0.0f, 0.0f);    glVertex3f( 1, -1,  0);

    glEnd();

    // Rotate the axises by some fun numbers ("Let's paint a happy little tree...")
    g_RotateX += 1; g_RotateY += 2; g_RotateZ += 1;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


    // Swap the backbuffers to the foreground
    SDL_GL_SwapBuffers();
}


/////////////////////////////////  KEY PRESSED  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function handles the keystroke events generated by the user
/////
/////////////////////////////////  KEY PRESSED  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
    
void KeyPressed(SDL_keysym * keysym)
{
    switch(keysym -> sym)
    {
        case SDLK_F1 :
            ToggleFullScreen();
            break;

        case SDLK_ESCAPE :
            Quit(0);

        case SDLK_1 :
            g_Viewport = 128;               // Change viewport size to 128
            break;

        case SDLK_2 :
             g_Viewport = 256;               // Change viewport size to 256
             break;

        case SDLK_3 :
             g_Viewport = 512;               // Change viewport size to 512
             break;

        default:
             break;
    }
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Not so bad is it?  This technique comes in VERY handy when doing other stuff
// such as fast motion blurring.  Check out this tutorial if you haven't already.
// 
// Once again, this tutorial demonstrates how to render to a texture map.
// We take a spinning cube and render it to a texture, then map that texture
// to a spinning quad to see the result.
// 
// The basic explanation of how this works is that we render the spinning cube
// to a smaller viewport than what we originally start with, then copy those
// pixels to the texture, then restore the viewport.  The smaller the viewport
// we render the texture from, the worse the quality the texture will turn out.
// We can switch between 128, 256 and 512 sizes by using '1', '2' and '3'.
// It's important to note that we start out by creating the texture to 512
// regardless.  You probably aren't really saving any speed by doing 128 in
// this tutorial because it's still rendering the 512 x 512 texture map.  I just
// wanted you to be able to see the difference in quality between the texture sizes.
// You will most likely want to change the 512 passed into CreateRenderTexture()
// to the fixed size you will be using.  I recommend using at LEAST a 256 x 256
// texture size.  If your video card supports it, use 512.  256 is decent, but
// obvious that there is intense scaling of the texture.
//
// The main function to note in this tutorial is:
//
// glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, g_Viewport, g_Viewport, 0);
//
// This function allows us to copy the current screen buffer to the current
// texture selected (by using glBindTexture()).  You call this once you have
// the viewport to the size of the texture that you will be rendering too.  Once
// the texture is rendered to, you will want to clear the screen so you don't
// see the rendered data in the top left or bottom left cornerm then restore
// the original view port.
//
// I hope this helps someone out!
// 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
