//***********************************************************************//
//                                                                                                                                               //
//              - "Talk to me like I'm a 3 year old!" Programming Lessons -              //
//                                                                       //
//              $Author:                DigiBen         DigiBen@GameTutorials.com                        //
//                                                                                                                                               //
//              $Program:               BSP Loader                                                                               //
//                                                                                                                                               //
//              $Description:   Loads faces and textures from a Quake3 BSP file  //
//                                                                                                                                               //
//              $Date:                  5/9/02                                                                                   //
//                                                                                                                                               //
//***********************************************************************//

#include "main.h"                                                               // This includes our main header file
#include "Camera.h"                                                             // Include our camera header file
#include "Quake3Bsp.h"                                                  // Include our quake3 header file

#include "SDL.h"

#define SPEED   200.0f                                                  // This is how fast our camera moves

CCamera g_Camera;                                                               // This is our global camera object

UINT g_Texture[MAX_TEXTURES] = {0};                             // This holds the texture info, referenced by an ID


bool movingForward = false, movingBackward = false, movingLeft = false, movingRight = false;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
//
// The code base for this tutorial was primarily taken from the Time Based Movement
// tutorial.  This tutorial is the first in a series that will show you how to
// load Quake3 .bsp files.  In this version we will just focus on loading the faces
// and the textures.  The next tutorial will show how to apply lightmaps to our level.
//
// What is a .bsp file?  Quake3 uses the .bsp file format (IBSP) to load its level data.
// Quake3 levels are stored in .pk3 files, which are just .zip files renamed.  You can
// rename them to .zip files and unzip them to get at it's data.  In tutorial, except
// near the end, we will not be reading from a zip file, but from the unzipped version.
// This is because it's a ton of code to read from a zip file and that isn't the point
// of the beginning tutorials.  
//
// * IMPORTANT *
//
// This tutorial series in not intended to recreate Quake3.  The whole point of this
// is to introduce you to game level formats so that you can use their tools to create
// your own games, and/or to get a good idea on how to create your own level format.
// Here is a list of the topics that I will be covering in the series:
//
// - Normal level geometry with textures
// - Multi-texturing with lightmaps
// - Basic entity parsing
// - Drawing with nodes and leafs
// - Frustum Culling and PVS (Portals|Clusters) handling 
// - Basic shader parsing
// - Reading from the .pk3 file (.zip file)
// - Collision detection with brushes (maybe if this doesn't take forever)
//
// Anything more than that I probably won't do right away.  I intend to gradually
// add more to this over time possibly, but I don't want to just focus on Quake stuff.
// There are tons of other tutorials to write :)  If you guys want to add on to our
// tutorials and submit them to us, that would be AWESOME.
//
// Building a 3D game from scratch is pretty hard.  With these tutorials I hope this
// will help you to avoid having to write your own level editor from scratch, since
// you can just use q3Radiant and other tools that ID provides free that create .bsp files.
// In case you are wondering, I won't be going over how to use the level editor, except
// the basics on how to compile your .bsp file.  There are too many tutorials on the tools
// already out there, plus it's so complex I would never get around to other tutorials.
// 
// * HTML COMPANION *
//
// Be sure to read the HTML file that should come with this tutorial.  This explains a
// bunch about the .bsp file format.  I won't be going into a huge amount of detail in
// the comments because much of it is talked about and shown in the HTML file.  
//
// * NEWBIE WARNING * 
//
// I would like to point out that though you might be able to understand everything
// in this tutorial, this is not a newbie tutorial on how to render a game level.  It
// is assumed that you have at LEAST loaded other types of file formats such as .3ds,
// .ase, .obj, .md2, .md3, or any others.  In order to create this series fast enough,
// I can't expound on every concept.  If you don't understand much of what is done here,
// first try loading in other file formats.  
//
// I might add, that I will not be doing the best optimizations possible for the 
// rendering, so I don't confused the usage of the data, but I will discuss at the end 
// on how you can get the best speed possible.  On a large level I get on average of
// 150 fps to 200 fps in most areas on a Geforce2 64mb.  If I put in the extra
// optimizations I should be getting around an average of 250 fps.
//
// I also want to mention is that I started the tutorial with a tiny 1 room
// level because it's always a good idea to start with something super small when
// learning to load in data from a file format.  That way it's reasonable to debug it
// if something goes wrong.  Probably about the third tutorial we will use a large level.
// If you decide to go and get a .bsp file somewhere else, you might find that not all
// of the textures load.  This is because many levels used the textures that came with
// the original game.  Quake searches for the texture and if it doesn't find it, it will
// search the rest of the base .pk3 files for the correct texture.  Sometimes you will
// also find that the texture isn't named the same as the file name.  To get around this
// you can just check with a strstr() to see if the texture is in part of the file name.
// To get other missing textures to load, you will sometimes need to get the file name
// from the shader files.
//
// Since there isn't a ton of information about the .bsp file format around, other than
// some huge engines and such, I did the best I could to get all the information fast so
// I could create this series.  If there is anything wrong or missing from my descriptions,
// please let me know so that I can correct it.
//
//

// This is our object that loads and handles the .bsp file loading and rendering
CQuake3BSP g_Level;

// These tell us if we are rendering triangles and the textures.
// We added a texture boolean so in the next tutorial you can see
// the difference of just lightmaps or just textures or both.
bool g_RenderMode = true;
bool g_bTextures  = true;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear The Screen And The Depth Buffer
        glLoadIdentity();                                                                       // Reset The matrix

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

                case SDL_KEYDOWN :                                      // if the user has pressed a key

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
                                                        g_RenderMode = !g_RenderMode;                   // Change the rendering mode
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
                        g_bTextures = !g_bTextures;                                             // Turn texture mapping on/off

                                if(!g_bTextures)                                                                // If we don't want texture mapping
                                {
                            glDisable(GL_TEXTURE_2D);                                   // Disable texture mapping
                                }
                    }
                            break;

                case SDL_MOUSEMOTION :                      // If the mouse is moved 
                    g_Camera.Update();                                  // Check the mouse camera movement
                    break;


                default:                                    // any other event
                    break;                                  // nothing to do
            } // switch
        } // while( SDL_ ...
                g_Camera.Update();                                  // update the position of the camera before drawing the frame
        RenderScene();                                      // draw our OpenGL scene
    } // while( ! done)
}



/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Pretty easy to grasp isn't it?  We didn't do much in this tutorial, but we
// got the basics down, and once you have those, the rest is easy.  The next
// tutorial will implement light maps.  The level looks pretty boring without
// any lighting.
//
// The controls for this tutorial are:
// 
// w, s, UP_ARROW, DOWN_ARROW - Move the camera forward and backward
// a, d, RIGHT_ARROW, LEFT_ARROW - Strafe the camera left and right
// Mouse movement - Moves the view for first person mode
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




