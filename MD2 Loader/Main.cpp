//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     digiben@gametutorials.com            //
//                                                                       //
//      $Program:       MD2 Loader                                       //
//                                                                       //
//      $Description:   Demonstrates how to load a Quake2 MD2 Model      //
//                                                                       //
//      $Date:          2/6/02                                           //
//                                                                       //
//***********************************************************************//


#include "main.h"                                        // This includes our header file
#include "Md2.h"

#include <math.h>

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This tutorial will demonstrate how to load a .Md2 file.  This 3D file format
// was created for Quake2 character models with key frame animation.  
// If you have any desire to create your own file, I suggest finding MilkShape 3D.
// This program allows you to import and export almost every popular 3D format,
// along with the ability to use it as a modeler.  It's a $20 shareware program,
// you can't beat that!  I chose the .Md2 format after viewing the internet and
// see the countless characters already on the internet for free download.
// This is good news for programmers that don't have art programs or can't
// do any art.  All the animations and weapons are done for you!  This is a
// great start to learning animation as well, since the Quake formats are so
// simple it makes me shed a tear of joy.  This tutorial only loads the model,
// where as the next tutorial will show how to do key frame animation using
// interpolation.  Next, we will tackle skeletal animation with the famous
// Quake3 .Md3 format.  This makes learning animation easy because it isn't
// complicated by a $3000 modeler.  Plus, there are hundreds of models out
// there on the internet to test and put in your own games (with permission of course).
// The best place to find models is at www.planetquake.com.  Usually there are
// featured models weekly to download.
//
// Since there is no animation for the model, we just have it spinning in the
// center of the screen.  The controls are: 
//
// Left Mouse Button - Changes the Render mode from normal to wireframe.
// Right Mouse Button - Turns lighting On/Off
// Left Arrow Key - Spins the model to the left
// Right Arrow Key - Spins the model to the right
// Escape - Quits
//
// If you decide to go find models you might find them in a format like .pk3.
// This is just a .zip format, so rename it to .zip and unzip it.  There should
// be .Md2 and texture files.  I think the textures are normally in .pcx, so you
// will have to save them as BMP's to run in here.
//
//
//
//

#define FILE_NAME  "tris.md2"                           // This is the 3D file we will load.
#define TEXTURE_NAME "hobgoblin.bmp"

unsigned int g_Texture[MAX_TEXTURES] = {0};                     // This holds the texture info, referenced by an ID

CLoadMD2 g_LoadMd2;                                     // This is MD2 class.  This should go in a good model class.
t3DModel g_3DModel;                                     // This holds the 3D Model info that we load in

int   g_ViewMode      = GL_TRIANGLES;                   // We want the default drawing mode to be normal
bool  g_bLighting     = true;                           // Turn lighting on initially
float g_RotateX       = 0.0f;                           // This is the current value at which the model is rotated
float g_RotationSpeed = 0.5f;                           // This is the speed that our model rotates.  (-speed rotates left)

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////



///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeGL(SCREEN_WIDTH, SCREEN_HEIGHT);    // Init OpenGL with the global rect

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

    // First we need to actually load the .MD2 file.  We just pass in an address to
    // our t3DModel structure and the file name string we want to load ("tris.md2").
    // We also need to give the texture name we will be using.  This is because there
    // are usually a lot of textures with each character.  You choose the best one.
    // It seems that all of the Quake2 characters .md2 files are called: "tris.md2"

    g_LoadMd2.ImportMD2(&g_3DModel, FILE_NAME, TEXTURE_NAME);       

    // There is no color information for these models, as well as only one
    // texture.  If everything goes well, it should load the TEXTURE_NAME file.

    // Go through all the materials
    for(int i = 0; i < g_3DModel.numOfMaterials; i++)
    {
        // Check to see if there is a file name to load in this material
        if(strlen(g_3DModel.pMaterials[i].strFile) > 0)
        {
            // Use the name of the texture file to load the bitmap, with a texture ID (i).
            // We pass in our global texture array, the name of the texture, and an ID to reference it. 
            cout << "CreateTexture " << endl;
            CreateTexture(g_Texture, g_3DModel.pMaterials[i].strFile, i);           
            cout << g_3DModel.pMaterials[i].strFile << endl;
        }

        // Set the texture ID for this material
        g_3DModel.pMaterials[i].texureId = i;
    }

    // Here, we turn on a lighting and enable lighting.  We don't need to
    // set anything else for lighting because we will just take the defaults.
    // We also want color, so we turn that on

    glEnable(GL_LIGHT0);                                // Turn on a light with defaults set
    glEnable(GL_LIGHTING);                              // Turn on lighting
    glEnable(GL_COLOR_MATERIAL);                        // Allow color

    // To make our model render somewhat faster, we do some front back culling.
    // It seems that Quake2 orders their polygons clock-wise.

    glEnable(GL_CULL_FACE);                             // Turn culling on
    glCullFace(GL_FRONT);                               // Quake2 uses front face culling apparently

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


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

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

    // Give OpenGL our position,    then view,      then up vector
    gluLookAt(      0, 1.5f, 90,        0, 0.5f, 0,         0, 1, 0);
    
    // We want the model to rotate around the axis so we give it a rotation
    // value, then increase/decrease it. You can rotate right of left with the arrow keys.

    glRotatef(g_RotateX, 0, 1.0f, 0);                       // Rotate the object around the Y-Axis
    g_RotateX += g_RotationSpeed;                           // Increase the speed of rotation

    // Make sure we have valid objects just in case. (size() is in the vector class)
    if(g_3DModel.pObject.size() <= 0) return;

    // Get the current object that we are displaying
    t3DObject *pObject = &g_3DModel.pObject[0];

    // Render lines or normal triangles mode, depending on the global variable
    glBegin(g_ViewMode);

        // Go through all of the faces (polygons) of the object and draw them
        for(int j = 0; j < pObject->numOfFaces; j++)
        {
            // Go through each corner of the triangle and draw it.
            for(int whichVertex = 0; whichVertex < 3; whichVertex++)
            {
                // Get the index for each point in the face
                int index = pObject->pFaces[j].vertIndex[whichVertex];

                // Get the index for each texture coord in the face
                int index2 = pObject->pFaces[j].coordIndex[whichVertex];
            
                // Give OpenGL the normal for this vertex.  Notice that we put a 
                // - sign in front.  It appears that because of the ordering of Quake2's
                // polygons, we need to invert the normal
                glNormal3f(-pObject->pNormals[ index ].x, -pObject->pNormals[ index ].y, -pObject->pNormals[ index ].z);
                    
                // Make sure there was a UVW map applied to the object or else it won't have tex coords.
                if(pObject->pTexVerts) 
                {
                    glTexCoord2f(pObject->pTexVerts[ index2 ].x, pObject->pTexVerts[ index2 ].y);
                }
                
                // Pass in the current vertex of the object (Corner of current face)
                glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
            }
        }

    glEnd();

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


    SDL_GL_SwapBuffers();                           // Swap the backbuffers to the foreground
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
                        if(g_ViewMode == GL_TRIANGLES) {                // We our drawing mode is at triangles
                            g_ViewMode = GL_LINE_STRIP;                 // Go to line stips
                        } 
                        else {
                            g_ViewMode = GL_TRIANGLES;                  // Go to triangles
                        }
                    }

                    if( event.button.button == SDL_BUTTON_RIGHT )
                    {
                        g_bLighting = !g_bLighting;
                        if(g_bLighting)
                        {
                            glEnable(GL_LIGHTING);
                        }
                        else
                        {
                            glDisable(GL_LIGHTING);
                        }
                    }
                    break;

                default:                                    // any other event
                    break;                                  // nothing to do
            } // switch
        } // while( SDL_ ...
        RenderScene();                                      // draw our OpenGL scene

    } // while( ! done)
//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

    // When we are done, we need to free all the model data
    // We do this by walking through all the objects and freeing their information

    // Go through all the objects in the scene
    for(int i = 0; i < g_3DModel.numOfObjects; i++)
    {
        // Free the faces, normals, vertices, and texture coordinates.
        delete [] g_3DModel.pObject[i].pFaces;
        delete [] g_3DModel.pObject[i].pNormals;
        delete [] g_3DModel.pObject[i].pVerts;
        delete [] g_3DModel.pObject[i].pTexVerts;
    }

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This tutorial shows how to load a .Md2 file.  This is a good addition to an engine.
// In the next tutorial we will show you how to handle the key frame animation.
// I think this is the best and easiest 3D file format I have worked with, especially
// for animation.
// 
// * What's An STL (Standard Template Library) Vector? *
// Let me quickly explain the STL vector for those of you who are not familiar with them.
// To use a vector you must include <vector> and use the std namespace: using namespace std;
// A vector is an array based link list.  It allows you to dynamically add and remove nodes.
// This is a template class so it can be a list of ANY type.  To create a vector of type
// "int" you would say:  vector<int> myIntList;
// Now you can add a integer to the dynamic array by saying: myIntList.push_back(10);
// or you can say:  myIntList.push_back(num);.  The more you push back, the larger
// your array gets.  You can index the vector like an array.  myIntList[0] = 0;
// To get rid of a node you use the pop_back() function.  To clear the vector use clear().
// It frees itself so you don't need to worry about it, except if you have data
// structures that need information freed from inside them, like our objects.
//
// Once again I should point out that the coordinate system of OpenGL and .Md2 files are different.
// Since Quake2 Models have the Z-Axis pointing up (strange and ugly I know! :), 
// we need to flip the y values with the z values in our vertices.  That way it
// will be normal, with Y pointing up.  Also, because we swap the Y and Z we need to negate 
// the Z to make it come out correctly.  This is also explained and done in ReadMd2Data().
//
// I would like to thank Daniel E. Schoenblum <dansch@hops.cs.jhu.edu> for help
// with explaining the file format.  You can check it out at:  
// 
// http://www.ugrad.cs.jhu.edu/~dansch/md2/#model_magic
//
// Let me know if this helps you out!
// 
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
// The Quake2 .Md2 file format is owned by ID Software.  This tutorial is being used 
// as a teaching tool to help understand model loading and animation.  This should
// not be sold or used under any way for commercial use with out written conset
// from ID Software.
//
// Quake and Quake2 are trademarks of id Software.
// All trademarks used are properties of their respective owners. 
//
//
