//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     digiben@gametutorials.com            //
//                                                                       //
//      $Program:       Texturing                                        //
//                                                                       //
//      $Description:   Texture maps a shape                             //
//                                                                       //
//      $Date:          7/3/01                                           //
//                                                                       //
//***********************************************************************//


#include "main.h"                                        // This includes our header file

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

unsigned int g_Texture[MAX_TEXTURES];                           // This will reference to our texture data stored with OpenGL (only positive numbers)

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////////////////////////////////////////////////////////////
//
// The code in this tutorial was taken from the Camera4 tutorial.  This is our last camera 
// tutorial.  We might create a bezier curve camera tutorial though in the future.  This 
// tutorial shows how to strafe the camera right or left.  
//


///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);    // Init OpenGL with the global rect
    
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // This is where we load all of our texture.  We are loading
    // just one in this case, but you can add many many more if you want.
    // We pass in our global textureArray, our file we want to load, and the texture ID we want associated with it.

    CreateTexture(g_Texture, "bitmap.bmp", 0);          // Load "bitmap.bmp" into openGL as a texture

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

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
        RenderScene();                              // Redraw the scene every frame
    } // while( ! done)
}

///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                   // Reset The matrix
    
        //    Position      View       Up Vector
    gluLookAt(0, 0, 6,     0, 0, 0,     0, 1, 0);       // This determines where the camera's position and view is

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
    
    // Below we draw a texture mapped square. Remember, GL_QUADS draws a 4 point polygon.
    // In order to assign a texture map to a polygon, we need to call glBindTexture().
    // This passes in the type of texture map (always use GL_TEXTURE_2D) and the index
    // into our generated texture array - g_Texture[].  If we want to use another
    // texture map, and we have multiple loaded, we just change the index into the array.

    // Bind the texture stored at the zero index of g_Texture[]
    glBindTexture(GL_TEXTURE_2D, g_Texture[0]);

    // Display a quad texture to the screen
    glBegin(GL_QUADS);

        // glTexCoord2f() takes the X and Y offset (or U and V) into the bitmap.
        // Then, the next point sent to be rendered attaches that part of the bitmap
        // to itself.  The (U, V) coordinates range from (0, 0) being the top left corner
        // of the bitmap, to (1, 1) being the bottom left corner of the bitmap.
        // You can go above 1 but it just is wrapped around back to zero and repeats the texture.
        // Try setting the 1's to 2's and see what it does, then try setting them to 0.5's.
        // The higher the number, the more instances of the texture will appear on the square,
        // Where the lower the number, it stretches the incomplete texture over the surface of the square.
        // For every vertice we need a U V coordinate, as shown below.  You might have to play
        // around with the values to make it texture correctly, otherwise it will be flipped, upside down,
        // or skewed.  It also depends on where you are looking at it.  We are looking down the -Z axis.

        // Display the top left vertice
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1, 1, 0);

        // Display the bottom left vertice
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1, -1, 0);

        // Display the bottom right vertice
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1, -1, 0);

        // Display the top right vertice
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1, 1, 0);

    glEnd();                                            // Stop drawing QUADS

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    SDL_GL_SwapBuffers();                               // Swap the backbuffers to the foreground
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
//
// There you have it!  That is how to texture map a primative.  It's pretty simple
// once you get the texture loaded.  That is the only challenging part.
// 
// Here are the steps to texture mapping:
// 
// 1) Turn on texture mapping by saying:  
//
//    glEnable(GL_TEXTURE_2D);
//
//    This is usually done in the beginning of the program.  BUT, you will
//    want to turn it back off before you draw primatives using color instead.
//    So, say:  glDisable(GL_TEXTURE_2D)  - Then draw your colored shapes, then
//    turn it back on by saying glEnable(GL_TEXTURE_2D);
// 
// 2) Load a texture.  Once you have the data of the texture you loaded, you need to
//    register it with OpenGL.  The first step to do this is:
//
//    glGenTextures(1, &textureArray[textureID]);
//
//    This generates a unique texture reference to the desired index of our texture array.
//    We then pass this data into glBindTexture() to set the current texture being mapped.
//
//    glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
//
//    We NEED to call glBindTexture() after we generate the texture using glGenTextures, 
//    otherwise it won't be assigned the data from the next function, gluBuild2DMipmaps();
//
//    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
//
//    What is a mip map you ask?  Mip maps are a bunch of different scaled sizes of the texture.
//    The reason for this is because it makes the texture look nicer if we are farther away or
//    up close to the texture.  It picks the best size depending on the distance we are from it.
//    It is a good idea to make textures that are a power of 2, like 2, 4, 8, 16, 32, 64, 128, 256, etc...
//    This makes it so it can get a smoother representation when it scales, otherwise it could look worse when scaled.
//    After we build the mip maps, we then need to set the type of filter we want to use.
//
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);   
//
//    If we use GL_LINEAR it will look smoother, but takes a little speed hit.  If we use
//    GL_NEAREST it will be a bit faster, but will look pixelated and blotchy.  The MIN filter
//    is used whenever the pixel being textured maps to an area greater than one texture element. 
//    The MAX filter is used when the pixel being textured maps to an area less than or equal to one texture element
//
// 3) After we load the texture, we just need to call glBindTexture() to set that as the current
//    texture being mapped.
// 
// 4) Next, we need to give each vertex a texture coordinate, also called (U, V) coordinates.
//    We do that by calling:
//
//    glTexCoord2f(U, V);
//
//    This assigns a part of the bitmap to the next vertice passed in.  It's like putting a
//    sticker on paper, you just match the correct corner with the correct paper corner.
//    You have to be carefull when you do this or else the image will be flipped or upside down.
//    It depends on the direction of your camera though.  The order is VERY important.
// 
// 5) Lastly, you need to delete the textures after you are finished.  You do this by calling:
// 
//    glDeleteTextures(1, &textureArray[textureIndex]);
// 
// That's it!  Let us know at GameTutorials.com if this tutorial helps you.  We would love to
// see what you come up with.
// 
// DigiBen
// www.GameTutorials.com
//
// Below is a function that might come in handy for creating a quad or a cube easily.
// The VECTOR3D data type is just a structure that as 3 floats:  float x, y, z;
// It makes it easier instead of passing in 3 parameters for the center.  Basically it
// takes a center point in 3D space, then allows you to size the QUAD, by doing this you
// also choose which axis it goes down, or which axis's.

/*
///////////////////////////////// RENDER QUAD \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This renders a textured or non texture quad to the screen
/////
///////////////////////////////// RENDER QUAD \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderQuad(VECTOR3D vCenter, float widthX, float widthY, float widthZ, int textureID)
{   
    // Bind the correct texture to the quad if the ID isn't a negative number
    if(textureID >= 0) glBindTexture(GL_TEXTURE_2D, g_Texture[textureID]);

    // Display a quad texture to the screen
    glBegin(GL_QUADS);

        // Display the top left vertice
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(vCenter.x - widthX, vCenter.y - widthY, vCenter.z - widthZ);

        // Display the bottom left vertice
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(vCenter.x - widthX, vCenter.y + widthY, vCenter.z - widthZ);

        // Display the top right vertice
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(vCenter.x + widthX, vCenter.y + widthY, vCenter.z + widthZ);

        // Display the bottom right vertice
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(vCenter.x + widthX, vCenter.y - widthY, vCenter.z + widthZ);

    glEnd();
}
*/
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
