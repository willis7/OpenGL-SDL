//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     DigiBen@GameTutorials.com            //
//                                                                       //
//      $Program:       BSP Loader2                                      //
//                                                                       //
//      $Description:   Loads and renders lightmaps for the level        //
//                                                                       //
//      $Date:          5/9/02                                           //
//                                                                       //
//***********************************************************************//


#include "main.h"                               // This includes our main header file
#include "Camera.h"                             // Include our camera header file
#include "Quake3Bsp.h"                          // Include our quake3 header file

#include "SDL.h"

#define SPEED   200.0f                          // This is how fast our camera moves

CCamera g_Camera;                               // This is our global camera object

UINT g_Texture[MAX_TEXTURES] = {0};             // This holds the texture info, referenced by an ID

bool g_bLightmaps = true;
bool movingForward = false, movingBackward = false, movingLeft = false, movingRight = false;

float g_Gamma = 10;

// These tell us if we are rendering triangles and the textures.
// We added a texture boolean so in the next tutorial you can see
// the difference of just lightmaps or just textures or both.
bool g_RenderMode = true;
bool g_bTextures  = true;

// This is our object that loads and handles the .bsp file loading and rendering
CQuake3BSP g_Level;



/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
//
// This is the second tutorial of our Quake 3 BSP Loader series.  In this version
// we implement the lightmaps over top of the textures using multitexturing.  If
// your card doesn't support multitexturing, you can change the code to do 2 passes
// instead.  If your card doesn't do multitexturing, you shouldn't be doing 3D
// programming anyway :)  If you can't run the program because you get an error
// with not supporting multitexturing, make sure your resolution and bits per pixel
// are at a normal value.  Some video cards only support certain settings.
//
// So, what is a lightmap?  A lightmap is a second texture that is used to
// apply over a normal texture to create realism.  These texture are precomputed
// in hours and hours of work on the CPU.  The average level renders in about
// 9 to 13 hours.  The result is incredible though.  I use hardware multitexturing,
// so if your card can't support it, either get a decent 15 dollar one, or change
// the code to do 2 passed.  Some cards might complain and say that they don't
// support multitexturing, but try going into full screen mode, or vice versa.
//
// Lightmaps are stored in the actual .bsp file, so we need to extract them out.
// This is done simply by following the lump structure and passing the RGB data
// into a function we create to turn it into a texture map.  Before doing this,
// we change the gamma intensity up about 1000 percent.  If we didn't change the
// gamma, the level would be really dark.  Quake3 actaully messes with the hardware
// to change the gamma, but if you mess with the lightmaps it works out just great.
//
// In our config file, added a new section called [Gamma].  This allows you to change
// the gamma levels by just editing the .ini file.  It will give you an idea of what
// different values look like.  It's currently set at 10 right now, with 1 being
// no change.
//
// If you haven't done any multitexturing before in OpenGL, check out our tutorial
// on the topic at www.GameTutorials.com.  
//
//

///////////////////////////////// INIT WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function initializes the application
/////
///////////////////////////////// INIT WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);  // Init OpenGL with the global rect


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // To load our .bsp file, we created a config.ini file that stores
    // the name of the level that we are loading.  This way we don't need
    // to change code to test different levels.  We will also add more options
    // to our config file in later tutorials.

    // Here we open the config file and init some variables
    ifstream fin("Config.ini");
    string strLevel = "";
    string strTemp = "";

    // Check if the file was not found or could not be opened
    if(fin.fail())
    {
        // Display an error message and quit the program if file wasn't found
        cerr << "Could not find the Config.ini file!" << endl;
        Quit(0);
        return;
    }

    // Read in the name of the level that will be loaded and close the file
    fin >> strLevel >> strLevel;
    fin.close();

    // Here we load the level and get the result (true == success)
    bool bResult = g_Level.LoadBSP(strLevel.c_str());

    // Make sure the file was found and we received a success
    if(bResult == false)
    {
        // Quit the application
        Quit(0);
        return;
    }

    // Position the camera to the starting point since we have
    // not read in the entities yet, which gives the starting points.
    g_Camera.PositionCamera( 80, 288, 16,   80, 288, 17,    0, 1, 0);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Turn on depth testing and texture mapping
    glEnable(GL_DEPTH_TEST);    
    glEnable(GL_TEXTURE_2D);

    // Enable front face culling, since that's what Quake3 does
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
}

///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
    int i = 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                   // Reset The matrix

    // Give OpenGL our camera position to look at
    g_Camera.Look();

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Easy as pie - just call our render function.  We pass in the camera
    // because in later tutorials we will need it's position when we start
    // dealing with the BSP nodes and leafs.
    g_Level.RenderLevel(g_Camera.Position());

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
    

    // Swap the backbuffers to the foreground
    SDL_GL_SwapBuffers();   
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

                case SDL_KEYDOWN :                      // if the user has pressed a key

                    /////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// * 
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_F1 :                      // If the key is F1 
                            ToggleFullScreen();             // Toggle between FullScreen and windowed mode
                            break;

                        case SDLK_ESCAPE :                  // if the key is ESCAPE
                            Quit(0);
            
                        // If we press the W or the UP arrow key we want to move the camera forward
                        case SDLK_UP :
                        case SDLK_w  :
                            movingForward = true;
                            break;

                        // If we press the s or the DOWN arrow key we want to zoom the scope out
                        case SDLK_DOWN :
                        case SDLK_s    :
                            movingBackward = true;
                            break;

                        // If we press the A or LEFT arrow key we want to strafe the camera left
                        case SDLK_LEFT :
                        case SDLK_a    :
                            movingLeft = true;
                            break;

                        // If we press the D or RIGHT arrow key we want to strafe the camera right
                        case SDLK_RIGHT :
                        case SDLK_d     :
                            movingRight = true;
                            break;

                        case SDLK_SPACE :                   // if we hit the SPACE Key
                            g_RenderMode = !g_RenderMode;           // Change the rendering mode
                            // Change the rendering mode to and from lines or triangles
                            if(g_RenderMode)                
                            {
                                // Render the triangles in fill mode        
                                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  
                            }
                            else 
                            {
                                // Render the triangles in wire frame mode
                                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  
                            }
                            break;

                        default:
                            break;
                    }
                    break;

                case SDL_KEYUP :
                    switch(event.key.keysym.sym)
                    {
                        // If we release the W or the UP arrow key we want to stop moving forward
                        case SDLK_UP :
                        case SDLK_w  :
                            movingForward = false;
                            break;

                        // If we release the s or the DOWN arrow key we want to stop moving backward
                        case SDLK_DOWN :
                        case SDLK_s    :
                            movingBackward = false;
                            break;

                        // If we release the A or LEFT arrow key we want to stop strafing left
                        case SDLK_LEFT :
                        case SDLK_a    :
                            movingLeft = false;
                            break;

                        // If we release the D or RIGHT arrow key we want to stop strafing right
                        case SDLK_RIGHT :
                        case SDLK_d     :
                            movingRight = false;
                            break;
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
                    
                case SDL_MOUSEBUTTONDOWN :                  // if a mouse button was pressed
                    if( event.button.button == SDL_BUTTON_RIGHT )       // and it's the Right mouse button
                    {
                        g_bTextures = !g_bTextures;                     // Turn texture mapping on/off

                        if(!g_bTextures)                                // If we don't want texture mapping
                        {
                            glActiveTextureARB(GL_TEXTURE0_ARB);
                            glDisable(GL_TEXTURE_2D);                   // Disable texture mapping
                        }
                    }
                    if( event.button.button == SDL_BUTTON_LEFT )
                    {
                        g_bLightmaps = !g_bLightmaps;

                        if( !g_bLightmaps)
                        {
                            glActiveTextureARB(GL_TEXTURE1_ARB);
                            glDisable(GL_TEXTURE_2D);
                        }
                    }
                    break;

                case SDL_MOUSEMOTION :                      // If the mouse is moved 
                    g_Camera.Update();                      // Check the mouse camera movement
                    break;


                default:                                    // any other event
                    break;                                  // nothing to do
            } // switch
        } // while( SDL_ ...
        g_Camera.Update();                                  // update the position of the camera before drawing the frame
        RenderScene();                                      // draw our OpenGL scene
    } // while( ! done)
}



///////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES *
//
// With such a little amount of code to add to our rendered, lightmaps give a
// tremendous improvement to the realism of the level.  All that really needed
// to be done was to load in the lightmaps RGB values, change their gamma levels,
// convert them to a texture map and just assign them as a second pass!
//
// Simple, but Sweeeeeet.
//
// The controls for this tutorial are:
//
// w, s, UP_ARROW, DOWN_ARROW - Move the camera forward and backward
// a, d, RIGHT_ARROW, LEFT_ARROW - Strafe the camera left and right
// Mouse movement - Moves the view for first person mode
// Left  Mouse Button - Turns lightmaps on and off
// Right Mouse Button - Turns textures on and off
// Space Bar - Turns wireframe on and off
// ESCAPE - Quits the program
//
// Remember to refer back to the HTML file that comes with this tutorial to
// learn more about the file format.  I skipped a bunch of information because
// it's already in that file.
//
// Be sure to include to correct libraries listed at the top and in the read me.
//
// Let me know what you think of this tutorial, I would love to hear your
// feedback and any cool projects you create with it.
//
// Enjoy!
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
//
//
//
//
