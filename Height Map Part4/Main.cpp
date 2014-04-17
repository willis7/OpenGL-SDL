//***********************************************************************//
//                                                                       //
//      - "Talk to me like a 3 year old!" Programming Lessons -          //
//                                                                       //
//      $Author:        DigiBen         DigiBen@GameTutorials.com        //
//                                                                       //
//      $Program:       HeightMap4                                       //
//                                                                       //
//      $Description:   Shows how to add volumetric fog to the terrain   //
//                                                                       //
//      $Date:          5/30/02                                          //
//                                                                       //
//***********************************************************************//
//

# include <GL/glx.h>

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
//
// In this tutorial we build from our past terrain tutorials to bring you a
// neat effect that can be summed up in 1 word, "sweeeet".  Volumetric fog
// is used in many popular games, include Quake.  This technique allows you to
// specify a bounding volume that only will have fog.  In many game levels you
// will notice that large endless "pits of death" use this effect.  We are going
// to use this to apply to our terrain.  It gives our scene a nice morning look.
// Of course the huge bright sun might destroy that morning look, but you can
// just imagine that it just came out :)
// 
// The thought of creating volumetric fog might at first seem scary and complicated,
// but in fact it's as easy at applying a texture map with glTexCoord2*() functions.
// Since we will be using OpenGL's hardware fog and not our own 2 pass/blend technique,
// we will need to load an extension: "glFogCoordfEXT".  This will be done upon
// initialization of our program, right after the multitexture extensions are loaded.
// Some video cards might not support this feature, like the Voodoo* cards.  If you
// find yourself in this predicament, just use vertex coloring to fake it.
//
// It is assumed that you have read through the "Fog" tutorial, since we will be
// using the standard fog function.  If not, you can still easily follow along, as
// the functions are self explanatory.
//
// To apply volumetric fog, the steps are simple.  First you want to initialize the
// fog like normal, then you will set a new property using glFogi() with the new
// defines listed in main.h, then just call glFogCoordfEXT() with a depth value
// before each vertex that should have fog.  If you pass in a 0 it should ignore
// the fog.  The higher the value the more dense the fog.
//
// Currently, the fog doesn't change when the camera moves.  To make it more realistic
// you might want to try and come up with a ratio that will change the GL_FOG_END value
// depending on the y position of the camera.  If the camera is above or in the fog, you
// would want to change the fog's density.
//
//

#include "main.h"                                        // This includes our header file
#include "Camera.h"                                     // This is our camera header file

#include <math.h>

// This stores the desired depth that we want to fog
float g_FogDepth = 50.0f;


// This controls if we have detail texturing on or off
bool g_bDetail = true;

// This handles the current scale for the texture matrix for the detail texture
int g_DetailScale = 16;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This is our fog extension function pointer to set a vertice's depth
PFNGLFOGCOORDFEXTPROC glFogCoordfEXT = NULL;

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

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // If we have loaded the mulitextures correctly, then we can now test to see
    // if the video card supports hardware accelerated fog.  We do the same things
    // for every extension. 
    
    if( !isExtensionSupported(string("GL_ARB_FOG_COORD") ) )
    {
        cout << "Error , fogCoord not supported" << endl;
        Quit(1);
    }
 
     // If we have loaded the mulitextures correctly, then we can now test to see
     // if the video card supports hardware accelerated fog.  We do the same things
     // for every extension.  First we tell wglGetProcAddress() which extension
     // we want, which then returns a function pointer.  Afterwards, the function
     // pointer is checked to make sure the current video card drivers or setup
     // support it.
     //
     // Find the correct function pointer that houses the fog coordinate function
    glFogCoordfEXT  =  (PFNGLFOGCOORDFEXTPROC) glXGetProcAddressARB((const GLubyte *)"glFogCoordfEXT");

    // It is assumed that by getting here, we should be able to do volumetric fog
    // with this video card.  Now comes the setup and initialization.  Just like
    // when we create normal fog, we need to turn on GL_FOG, give a fog color,
    // as well as give the start and end distance for the thickness of the fog.
    // The new information that will need to be given will be to glFogi().
    // The new flags we defined tell OpenGL that we want per vertex fog.
    // Notice that we don't use GL_FOG_DENSITY.  It doesn't seem to have any effect.

    // Pick a tan color for our fog with a full alpha
    float fogColor[4] = {0.8f, 0.8f, 0.8f, 1.0f};

    glEnable(GL_FOG);                       // Turn on fog
    glFogi(GL_FOG_MODE, GL_LINEAR);         // Set the fog mode to LINEAR (Important)
    glFogfv(GL_FOG_COLOR, fogColor);        // Give OpenGL our fog color
    glFogf(GL_FOG_START, 0.0);              // Set the start position for the depth at 0
    glFogf(GL_FOG_END, 50.0);               // Set the end position for the detph at 50

    // Now we tell OpenGL that we are using our fog extension for per vertex
    // fog calculations.  For each vertex that needs fog applied to it we must
    // use the glFogCoordfEXT() function with a depth value passed in.
    // These flags are defined in main.h and are not apart of the normal opengl headers.
    glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


    // Here we read read in the height map from the .raw file and put it in our
    // g_HeightMap array.  We also pass in the size of the .raw file (1024).
    LoadRawFile("Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);

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
//// In this version of the height map series we added volumetric fog to our terrain.
//// This is done by using the glFogCoordEXT() extensions, which some cards might
//// not support (Voodoo cards).  We added some code to load the extension in Init(),
//// along with some global variables.  To change the height of the fog you can use
//// the - or - keys to increase and decrease the height.
////
////
//// This concludes the fourth tutorial in the height map series.
////
////
//// Ben Humphrey (DigiBen)
//// Game Programmer
//// DigiBen@GameTutorials.com
//// Co-Web Host of www.GameTutorials.com
////
////
