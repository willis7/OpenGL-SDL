#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>                                      // Header File For The OpenGL32 Library
#include <GL/glu.h>                                     // Header File For The GLu32 Library

# include <string>
# include <vector>

#include "SDL.h"

using namespace std;
typedef unsigned char BYTE;


#define SCREEN_WIDTH 800                                // We want our screen width 800 pixels
#define SCREEN_HEIGHT 600                               // We want our screen height 600 pixels
#define SCREEN_DEPTH 16                                 // We want 16 bits per pixel

// We added a lot of structures for our model.  If you want to do animation
// you will need to add some more information.  We will save that for another tutorial.

#define MAX_TEXTURES 100                                // The maximum amount of textures to load

// This is our 3D point class.  This will be used to store the vertices of our model.
class CVector3 
{
public:
    float x, y, z;
};

// This is our 2D point class.  This will be used to store the UV coordinates.
class CVector2 
{
public:
    float x, y;
};

// This is our face structure.  This is is used for indexing into the vertex 
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
struct tFace
{
    int vertIndex[3];           // indicies for the verts that make up this triangle
    int coordIndex[3];          // indicies for the tex coords to texture this face
};

// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.
struct tMaterialInfo
{
    char  strName[255];         // The texture name
    char  strFile[255];         // The texture file name (If this is set it's a texture map)
    BYTE  color[3];             // The color of the object (R, G, B)
    int   texureId;             // the texture ID
    float uTile;                // u tiling of texture  (Currently not used)
    float vTile;                // v tiling of texture  (Currently not used)
    float uOffset;              // u offset of texture  (Currently not used)
    float vOffset;              // v offset of texture  (Currently not used)
} ;

// This holds all the information for our model/scene. 
// You should eventually turn into a robust class that 
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
struct t3DObject 
{
    int  numOfVerts;            // The number of verts in the model
    int  numOfFaces;            // The number of faces in the model
    int  numTexVertex;          // The number of texture coordinates
    int  materialID;            // The texture ID to use, which is the index into our texture array
    bool bHasTexture;           // This is TRUE if there is a texture map for this object
    char strName[255];          // The name of the object
    CVector3  *pVerts;          // The object's vertices
    CVector3  *pNormals;        // The object's normals
    CVector2  *pTexVerts;       // The texture's UV coordinates
    tFace *pFaces;              // The faces information of the object
};
//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This holds our information for each animation of the Quake model.
// A STL vector list of this structure is created in our t3DModel structure below.
struct tAnimationInfo
{
    char strName[255];          // This stores the name of the animation (Jump, Pain, etc..)
    int startFrame;             // This stores the first frame number for this animation
    int endFrame;               // This stores the last frame number for this animation
};

// We added 4 new variables to our model structure.  These will help us handle
// the current animation.  As of now, the current animation will continue to loop
// from it's start from to it's end frame until we right click and change animations.
struct t3DModel 
{
    int numOfObjects;                   // The number of objects in the model
    int numOfMaterials;                 // The number of materials for the model
    int numOfAnimations;                // The number of animations in this model (NEW)
    int currentAnim;                    // The current index into pAnimations list (NEW)
    int currentFrame;                   // The current frame of the current animation (NEW)
    vector<tAnimationInfo> pAnimations; // The list of animations (NEW)
    vector<tMaterialInfo> pMaterials;   // The list of material information (Textures and colors)
    vector<t3DObject> pObject;          // The object list for our model
};

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////



extern int VideoFlags;                                    // The Pixel Format flags
extern SDL_Surface * MainWindow;                        // Our Drawable portion of the window

// This Controls our MainLoop
void MainLoop(void);

// This toggles between the Full Screen mode and the windowed mode
void ToggleFullScreen(void);

void CreateTexture(unsigned int textureArray[],char *strFileName,int textureID);

// This allows us to configure our window for OpenGL and backbuffered
void SetupPixelFormat(void);

// This is our own function that makes creating a window modular and easy
void CreateMyWindow(const char *strWindowName, int width, int height, int VideoFlags);

// This inits our screen translations and projections
void SizeOpenGLScreen(int width, int height);

// This sets up OpenGL
void InitializeGL(int width, int height);

// This initializes the whole program
void Init();

bool isExtensionSupported(string ext);

// This handles the keypress events generated when the user presses a key
void HandleKeyPressEvent(SDL_keysym * keysym);

// This draws everything to the screen
void RenderScene();

// This shuts down SDL and quits program
void Quit(int ret_val);

#endif  // _MAIN_H_

/////////////////////////////////////////////////////////////////////////////////
////
//// * QUICK NOTES *
////
//// This file includes all the structures that you need to hold the model data
//// and animate using key frame interpolation.  If you intend to use this code, I would
//// make the model and object structures classes. This way you can have a bunch of
//// helper functions like Import(), Translate(), Render()...
////
//// This file added a new structure since the last Quake MD2 loading tutorial, tAnimationInfo:
////
//// struct tAnimationInfo
//// {
////    char strName[255];    // This stores the name of the animation (Jump, Pain, etc..)
////    int startFrame;       // This stores the first frame number for this animation
////    int endFrame;         // This stores the last frame number for this animation
//// };
////
//// Each quake animation has a character string name, and a starting frame and ending
//// frame, of the total ~198 frames of animation in the model.  This makes it easy to
//// know where the animation starts and when it stops.  That way our interpolation
//// just happens between those frames.  The nice thing about key frame interpolation is
//// that if you need to move to another animation, you just interpolate you current frame
//// of animation to the first frame of the new animation.  There will usually be a nice
//// transition between animations.
////
//// We finally added some variables to keep track our our animations and to store
//// the current animation and frame our model is in.  These are store in the
//// t3DModel structure:
////
////  int numOfAnimations;                // The number of animations in this model (NEW)
////  int currentAnim;                    // The current index into pAnimations list (NEW)
////  int currentFrame;                   // The current frame of the current animation (NEW)
////  vector<tAnimationInfo> pAnimations; // The list of animations (NEW)
////
//// Like the rest of our lists, we make the animation list a STL vector so we don't have
//// to write our own list code and worry about clean up.
////
//// * What's An STL (Standard Template Library) Vector? *
////
//// Let me quickly explain the STL vector for those of you who are not familiar with them.
//// To use a vector you must include <vector> and use the std namespace: using namespace std;
//// A vector is an array based link list.  It allows you to dynamically add and remove nodes.
//// This is a template class so it can be a list of ANY type.  To create a vector of type
//// "int" you would say:  vector<int> myIntList;
//// Now you can add a integer to the dynamic array by saying: myIntList.push_back(10);
//// or you can say:  myIntList.push_back(num);.  The more you push back, the larger
//// your array gets.  You can index the vector like an array.  myIntList[0] = 0;
//// To get rid of a node you use the pop_back() function.  To clear the vector use clear().
//// It frees itself so you don't need to worry about it, except if you have data
//// structures that need information freed from inside them, like our objects.
////
////
//// Ben Humphrey (DigiBen)
//// Game Programmer
//// DigiBen@GameTutorials.com
//// Co-Web Host of www.GameTutorials.com
////
//// The Quake2 .Md2 file format is owned by ID Software.  This tutorial is being used
//// as a teaching tool to help understand model loading and animation.  This should
//// not be sold or used under any way for commercial use with out written conset
//// from ID Software.
////
//// Quake and Quake2 are trademarks of ID Software.
//// All trademarks used are properties of their respective owners.
////
////
