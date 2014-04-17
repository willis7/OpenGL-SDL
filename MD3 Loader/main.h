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

typedef unsigned char BYTE;


#define SCREEN_WIDTH 800                                // We want our screen width 800 pixels
#define SCREEN_HEIGHT 600                               // We want our screen height 600 pixels
#define SCREEN_DEPTH 16                                 // We want 16 bits per pixel

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This file includes all of the model structures that are needed to load
// a .Md3 file.  When it comes to skeletal animation, we need to add quite
// a bit more variables to these structures.  Not all of the data will be used
// because Quake3 models don't have such a need.  I decided to keep the structures
// the same as the rest of the model loaders on our site so that we could eventually
// use a base class in the future for a library.  There was some additions though to our
// t3DModel structure.  Look below to get a better description of what changed and why.


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

// This is our face structure.  This is used for indexing into the vertex 
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
struct tFace
{
    int vertIndex[3];           // indicies for the verts that make up this triangle
    int coordIndex[3];          // indicies for the tex coords to texture this face
};

// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them.
struct tMaterialInfo
{
    char  strName[255];         // The texture name
    char  strFile[255];         // The texture file name (If this is set it's a texture map)
    BYTE  color[3];             // The color of the object (R, G, B)
    int   texureId;             // the texture ID
    float uTile;                // u tiling of texture  
    float vTile;                // v tiling of texture  
    float uOffset;              // u offset of texture
    float vOffset;              // v offset of texture
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


// This our model structure
struct t3DModel 
{
    int numOfObjects;                   // The number of objects in the model
    int numOfMaterials;                 // The number of materials for the model
    vector<tMaterialInfo> pMaterials;   // The list of material information (Textures and colors)
    vector<t3DObject> pObject;          // The object list for our model
    
    int numOfTags;                      // This stores the number of tags in the model
    t3DModel    **pLinks;               // This stores a list of pointers that are linked to this model
    struct tMd3Tag      *pTags;         // This stores all the tags for the model animations
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
//// This file stores all the structures needed to just load a Quake3 character.
//// To animate the character, we will need to add a bit more structures and such.
//// These structures should all be the same pretty much as the other loader tutorials.
////
//// I would like to note that in our t3DModel structure, we put a "struct" in front
//// of the tMD3Tag so that we don't get an error when compiling because it doesn't
//// know what a tMD3Tag is yet.  You can think of this as a "forward declaration", where
//// it allows you to use it, even if it's hasn't been defined yet.  If you have problems
//// with this in your compiler, just rip the structure tMD3Tag out of md3.h and stick
//// it right above the t3DModel structure.
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
//// The Quake3 .Md3 file format is owned by ID Software.  This tutorial is being used
//// as a teaching tool to help understand model loading and animation.  This should
//// not be sold or used under any way for commercial use with out written consent
//// from ID Software.
////
//// Quake, Quake2 and Quake3 are trademarks of ID Software.
//// Lara Croft is a trademark of Eidos and should not be used for any commercial gain.
//// All trademarks used are properties of their respective owners.
////
////
