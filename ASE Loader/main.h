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

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This file includes all of the model structures that are needed to load
// in a .ASE file.  If you intend to do animation you will need to add on
// to this.  These structures only support the information that is needed
// to load the objects in the scene and their associative materials.

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
    BYTE  color[3];             // The color of the object (R, G, B) from 0 to 255
    float fColor[3];            // The color of the object (R, G, B) from 0 to 1
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

// This holds our model information.  This should also turn into a robust class.
// We use STL's (Standard Template Library) vector class to ease our link list burdens. :)
struct t3DModel 
{
    int numOfObjects;                   // The number of objects in the model
    int numOfMaterials;                 // The number of materials for the model
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
//// This file includes all the structures that you need to hold the model data.
//// Of course, if you want a robust loader, you need some more things for animation, etc..
//// If you intend to use this code, I would make the model and object structures classes.
//// This way you can have a bunch of helper functions like Import(), Translate(), Render()...
////
//// The Import() function would decide which loader it needed to call, depending on the extension.
////
////
////
//// Ben Humphrey (DigiBen)
//// Game Programmer
//// DigiBen@GameTutorials.com
//// Co-Web Host of www.GameTutorials.com
////
////
////
