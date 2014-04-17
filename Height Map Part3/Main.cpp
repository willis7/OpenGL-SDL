//***********************************************************************//
//                                                                       //
//      - "Talk to me like a 3 year old!" Programming Lessons -          //
//                                                                       //
//      $Author:        DigiBen         DigiBen@GameTutorials.com        //
//                                                                       //
//      $Program:       HeightMap3                                       //
//                                                                       //
//      $Description:   Show how to apply a detail texture to a terrain  //
//                                                                       //
//      $Date:          5/28/02                                          //
//                                                                       //
//***********************************************************************//

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
//
// Now that you most likely know and understand how to create a terrain from a
// height map and texture it, we move on to the next step.  We get to apply a
// second texture on top of our current texture to give a fake detail and/or almost
// fony bump mapping to it.  This technique is done by blending and tiling a
// second texture over top of our current terrain texture with multitexturing.  Just
// so the detail texture doesn't over power the terrain texture, we increase the
// gamma on the detail texture to produce a nice blend between the two.
// 
// It is assumed that you have looked over the multitexturing and previous height
// map tutorials before attempting to tackle this one.  If you are just interested
// in the detail texturing, you can skip over to Terrain.cpp and look at all the
// code between the * NEW * blocks.
// 
// If for some reason your video card can't support textures over 256 x 256, you
// can easily just shrink the textures to this tutorial down with any paint program.
//
//

#include "main.h"                                        // This includes our header file
#include "Camera.h"                                     // This is our camera header file

#include <math.h>

// This controls if we have detail texturing on or off
bool g_bDetail = true;

// This handles the current scale for the texture matrix for the detail texture
int g_DetailScale = 16;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


// This is our global camera object
CCamera g_Camera;                               

// This holds the height map data
BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];            

// This tells us if we want lines or fill mode
bool  g_bRenderMode = true;                     

// This holds the texture info by an ID
unsigned int g_Texture[MAX_TEXTURES] = {0};             

#define BACK_ID     11      // The texture ID for the back side of the cube
#define FRONT_ID    12      // The texture ID for the front side of the cube
#define BOTTOM_ID   13      // The texture ID for the bottom side of the cube
#define TOP_ID      14      // The texture ID for the top side of the cube
#define LEFT_ID     15      // The texture ID for the left side of the cube
#define RIGHT_ID    16      // The texture ID for the right side of the cube

///////////////////////////////// CREATE SKY BOX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This creates a box centered around X Y Z.  Instead of colors at
/////   each vertices there is texture maps for each side of the box to 
/////   create the illusion of the sky box.  You can set the width, height and length
/////   of the sky box to create different perspectives for different sky box textures.
/////
///////////////////////////////// CREATE SKY BOX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateSkyBox(float x, float y, float z, float width, float height, float length)
{
    // Turn on texture mapping if it's not already
    glEnable(GL_TEXTURE_2D);

    // Bind the BACK texture of the sky map to the BACK side of the cube
    glBindTexture(GL_TEXTURE_2D, g_Texture[BACK_ID]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // This centers the sky box around (x, y, z)
    x = x - width  / 2;
    y = y - height / 2;
    z = z - length / 2;

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);      
        
        // Assign the texture coordinates and vertices for the BACK Side
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,          z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x,         y + height, z);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x,         y,          z);
        
    glEnd();

    // Bind the FRONT texture of the sky map to the FRONT side of the box
    glBindTexture(GL_TEXTURE_2D, g_Texture[FRONT_ID]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);  
    
        // Assign the texture coordinates and vertices for the FRONT Side
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,         y,          z + length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y + height, z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,          z + length);
    glEnd();

    // Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
    glBindTexture(GL_TEXTURE_2D, g_Texture[BOTTOM_ID]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);      
    
        // Assign the texture coordinates and vertices for the BOTTOM Side
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,         y,          z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y,          z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,          z + length); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,          z);
    glEnd();

    // Bind the TOP texture of the sky map to the TOP side of the box
    glBindTexture(GL_TEXTURE_2D, g_Texture[TOP_ID]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);      
        
        // Assign the texture coordinates and vertices for the TOP Side
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,         y + height, z + length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y + height, z);
        
    glEnd();

    // Bind the LEFT texture of the sky map to the LEFT side of the box
    glBindTexture(GL_TEXTURE_2D, g_Texture[LEFT_ID]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);      
        
        // Assign the texture coordinates and vertices for the LEFT Side
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y + height, z); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x,         y + height, z + length); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x,         y,          z + length);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,         y,          z);     
        
    glEnd();

    // Bind the RIGHT texture of the sky map to the RIGHT side of the box
    glBindTexture(GL_TEXTURE_2D, g_Texture[RIGHT_ID]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);      

        // Assign the texture coordinates and vertices for the RIGHT Side
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,          z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,          z + length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glEnd();
}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeGL(SCREEN_WIDTH, SCREEN_HEIGHT);    // Init OpenGL with the global rect

    if( !isExtensionSupported(string("GL_ARB_MULTI_TEXTURE") ) )
    {
        cout << "Error , MultiTexturing not supported" << endl;
        Quit(1);
    }

    // Here we read read in the height map from the .raw file and put it in our
    // g_HeightMap array.  We also pass in the size of the .raw file (1024).
    LoadRawFile("Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
    
    glEnable(GL_DEPTH_TEST);                            // Enables depth testing
    glEnable(GL_TEXTURE_2D);                            // Enable texture mapping
    glEnable(GL_CULL_FACE);                             // Turn on back face culling

    CreateTexture(g_Texture,    "Terrain.bmp" , 0         );    // Load the terrain texture
    CreateTexture(g_Texture,    "Detail.bmp"  , 1         );    // Load the terrain texture
    CreateTexture(g_Texture,    "Back.bmp"    , BACK_ID   );    // Load the Sky box Back texture
    CreateTexture(g_Texture,    "Front.bmp"   , FRONT_ID  );    // Load the Sky box Front texture
    CreateTexture(g_Texture,    "Bottom.bmp"  , BOTTOM_ID );    // Load the Sky box Bottom texture
    CreateTexture(g_Texture,    "Top.bmp"     , TOP_ID    );    // Load the Sky box Top texture
    CreateTexture(g_Texture,    "Left.bmp"    , LEFT_ID   );    // Load the Sky box Left texture
    CreateTexture(g_Texture,    "Right.bmp"   , RIGHT_ID  );    // Load the Sky box Right texture

    // Give our camera a decent starting point in the world
    g_Camera.PositionCamera( 280, 35, 225,  281, 35, 225,  0, 1, 0);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Enable Key Repeat
    if( SDL_EnableKeyRepeat(100,SDL_DEFAULT_REPEAT_INTERVAL) )
    {
        cerr << "Failed enabling key repeat" << endl;
        Quit(1);
    }
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
    
    // Get the current position of the camera
    CVector3 vPos       = g_Camera.Position();
    CVector3 vNewPos    = vPos;

    // Check if the camera is below the height of the terrain at x and z,
    // but we add 10 to make it so the camera isn't on the floor.
    if(vPos.y < Height(g_HeightMap, (int)vPos.x, (int)vPos.z ) + 10)
    {
        // Set the new position of the camera so it's above the terrain + 10
        vNewPos.y = (float)Height(g_HeightMap, (int)vPos.x, (int)vPos.z ) + 10;

        // Get the difference of the y that the camera was pushed back up
        float temp = vNewPos.y - vPos.y;

        //  Get the current view and increase it by the different the position was moved
        CVector3 vView = g_Camera.View();
        vView.y += temp;

        // Set the new camera position.
        g_Camera.PositionCamera(vNewPos.x,  vNewPos.y,  vNewPos.z,
                                vView.x,    vView.y,    vView.z,    0, 1, 0);                               
    }

    // Give OpenGL our camera position
    g_Camera.Look();

    // Render the height map
    RenderHeightMap(g_HeightMap);                       

    // Create the sky box and center it around the terrain
    CreateSkyBox(500, 0, 500, 2000, 2000, 2000);

    // Swap the backbuffers to the foreground
    SDL_GL_SwapBuffers();                                   
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
                    if( event.key.keysym.sym == SDLK_SPACE )
                    {
                        // To get a few different ideas of what a detail texture
                        // could look like, we want to change it's scale value.

                        // Times the current scale value by 2 and loop when it get's to 128
                        g_DetailScale = (g_DetailScale * 2) % 128;

                        // If the scale value is 0, set it to 1 again
                        if(g_DetailScale == 0)
                            g_DetailScale = 1;
                    }
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

                case SDL_MOUSEBUTTONDOWN :
                    if( event.button.button == SDL_BUTTON_LEFT )
                    {
                        g_bRenderMode = !g_bRenderMode;

                        if( g_bRenderMode )
                            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                        else
                            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                    }

                default:                                    // any other event
                    break;                                  // nothing to do
            } // switch
        } // while( SDL_ ...

        g_Camera.Update();
        RenderScene();                                      // draw our OpenGL scene
    } // while( ! done)
}

/////////////////////////////////////////////////////////////////////////////////
////
//// * QUICK NOTES *
////
//// Nothing was added to this file except the checking for the space bar,
//// along with the global variables at the top.
////
//// This concludes the third tutorial in the height map series.
////
////
//// Ben Humphrey (DigiBen)
//// Game Programmer
//// DigiBen@GameTutorials.com
//// Co-Web Host of www.GameTutorials.com
////
////
////
