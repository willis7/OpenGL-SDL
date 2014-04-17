//***********************************************************************//
//                                                                       //
//      - "Talk to me like a 3 year old!" Programming Lessons -          //
//                                                                       //
//      $Author:        DigiBen         DigiBen@GameTutorials.com        //
//                                                                       //
//      $Program:       Height Map                                       //
//                                                                       //
//      $Description:   This shows how render a height map from a file.  //
//                                                                       //
//      $Date:          7/12/01                                          //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file
#include "Camera.h"                                     // This is our camera header file

#include <math.h>
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// Here we make an array of bytes to hold our height map data.  Since we are reading
// in a .raw file that just stores values from 0 to 255, we can use them as height values,
// with 255 being the highest point, and 0 being the lowest point.

// This holds the height map data
BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];                    

// This tells us if we want lines or fill mode
bool  g_bRenderMode = true;                                 

// This is our global camera object
CCamera g_Camera;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeGL(SCREEN_WIDTH, SCREEN_HEIGHT);    // Init OpenGL with the global rect
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Here we read read in the height map from the .raw file and put it in our
    // g_HeightMap array.  We also pass in the size of the .raw file (1024).

    LoadRawFile("Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);   

    // Here we set the camera in a obscure position to get a 
    // good outside view of the terrain.  

    // Give our camera a decent starting point in the world
    g_Camera.PositionCamera(1200, 300, 1150,  1199, 300, 1150,  0, 1, 0);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Enable Key Repeat
    if( SDL_EnableKeyRepeat(100,SDL_DEFAULT_REPEAT_INTERVAL) )
    {
        cerr << "Failed enabling key repeat" << endl;
        Quit(1);
    }
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

        g_Camera.Update();
        RenderScene();                                      // draw our OpenGL scene
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


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
    
    // Give OpenGL our camera position
    g_Camera.Look();

    // If we pass the g_HeightMap data into our RenderHeightMap() function it will
    // render the terrain in QUADS.  If you are going to make any use of this function,
    // it might be a good idea to put in an (X, Y) parameter to draw it at, or just use
    // OpenGL's matrix operations (glTranslatef() glRotate(), etc...)

    RenderHeightMap(g_HeightMap);                       // Render the height map

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


    SDL_GL_SwapBuffers();                               // Swap the backbuffers to the foreground
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Not to much extra going on in this file.  We load and make the call to RenderHeightMap().
// You can turn wire frame mode on and off by clicking the left mouse button. The camera
// controls are as normal.  The mouse and the obvious keyboard keys move around.
// 
// Let's go over the steps that we accomplished during this tutorial: (Explained more in Terrain.h)
// 
// 1) First, we need to read the height map from the .raw file.  This is simple because
//    there is no header to a .raw file, it is just the image bits.  This file format
//    isn't what you generally want to use because you have to either know what the
//    size and type are, or guess, but I thought it fitting for this tutorial.
// 
// 2) After we read our height map data, we then needed to display it.  This was
//    also a simple function because we are just making QUADS with a set size.
//    I chose to do 16 by 16 quads, but you can change this to what ever you want.
//    With our height map array, we treated it as a 2D array and did 2 for loops
//    to draw each quad for each row and column.  Instead of doing lighting, I
//    just gave each vertex a green intensity, depending on it's height.  This makes
//   the terrain look like there is lighting applied.  This also makes it easier to
//    see the definition in the terrain until lighting and texture maps are applied.
//    
// That's it!
// 
// If you want to create your own .raw files, you can either use Photoshop and use the 
// Render->Clouds option, then save it as a .raw format, or use Paint Shop Pro.  There are
// probably many more programs that doing it, but those are the most popular ones.  You can
// also create your own in code, then fwrite() the bytes to a .raw file.  That's all it is.
// If you are interested in learning how to generate random height map values, let me know
// and I will be happy to send you some source code.  The technique is called Perlin Noise.
//
// If you have any feedback or suggestions, please send them.  I am always interested.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
  
