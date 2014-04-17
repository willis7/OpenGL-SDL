//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     digiben@gametutorials.com            //
//                                                                       //
//      $Program:       MD3 Loader                                       //
//                                                                       //
//      $Description:   Demonstrates how to loader a Quake3 MD3 Model    //
//                                                                       //
//      $Date:          3/10/02                                          //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file
#include "Md3.h"

#include <math.h>

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

#include "Md3.h"                                        // Include the MD3 header file.

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

unsigned int g_Texture[MAX_TEXTURES] = {0};         // This holds the texture info, referenced by an ID

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This tutorial shows you how to load a Quake3 character with a weapon.  The file format
// for these models are .MD3.  Unlike the .MD2 files, the character is broken up into 3
// different files:  Head.md3, Upper.md3 and Lower.md3.  If you want a weapon, that is also
// a separate .MD3 file.  This will be the first of 2 .MD3 tutorials.  This one will focus
// just on the loading of the files and rendering the static meshes to the screen.  The next
// tutorial will implement the key frame and bone animation.  Ya better study up on those
// quaternions :)
// 
// Though we are just drawing a static model with a gun, the amount of code to do so is
// pretty massive compared to the .MD2 file loader.  This is because there are different 
// parts of the body to be linked together.  This file format is a great introduction into
// skeletal animation.  Half life's .MDL format takes this a step further and cans the "canned" 
// animations that the .MD3 file stores.  What I mean by this is that Quake3 models have a mix
// of bone animation and vertex key frame animation.  Each of the 3 body parts have their
// own list of animations like the .MD2 file format has, but then they need to be connected
// to their parent node.  For instance, the head model is connected to the upper body model,
// and the upper body model is connected to the lower legs.  In our case, when our Lara croft
// does a back flip and lands on her face for one of her death scenes, the upper body is 
// attached to the legs, so it goes with it.  This in turn makes the head follow because
// it's attached to the upper body.  Each of the body parts are linked to what are known as
// "tags".  The benefit of have separate animations for different body parts is that
// you can have the character doing the shooting animation, and have it not matter what
// the legs are doing.  When the character jumps or lands, you don't want to have to make an
// animation for every case, like a jumping and shooting animation, a jumping and gesture
// animation, a jumping and pick up gun animation; you get the point.
//
// Another notable thing to understand right off is that each mesh (separate object) has a skin
// assigned to it.  The .skin file is a text file that stores the texture map names for
// that mesh.  You will find a .skin for each head, lower and upper body part.  There is
// also a .cfg config file that is also a text file, which contains the animation information.
// The numbers stored in that file tell us the start and end frames, looping frames and the
// animations frames per second.  It can also store things like footprint sounds and offsets.
// This will be explained in the .MD3 Animation tutorial.  
//
// Finally, you will usually find a .shader file with every weapon you use.  This has a ton 
// of information about the weapon.  There is actually a scripting language associated with 
// shader files.  For our purposes, I cut out all the complex garbage and just saved the 
// texture maps needed for the weapon. the order these texture maps are laid out in the file 
// is important.  They correspond to the index of the gun's objects.  The .MD3 format is one 
// model, but has many sub-objects stored in it as well.  That is why we use the t3DModel 
// structure to store the .MD3 info for each file, where in the previous file loading 
// tutorials we just used 1 for the whole scene.
//
// Usually all this information is stored in .PK3 file, which is a .ZIP file with a the quake3
// directory structure of Models\ Textures\ Sounds\ etc...  When you download a character from
// the internet or wherever, just rename the .PK3 file to .ZIP.  Then unzip the file and extract
// the goods.  The texture files are most likely TARGA (.tga) files.  I converted our model's
// textures to .bmp's so you don't have to sift through TGA loader code.
//
// You might be wondering why when you go to wire frame you see a small Lara head in the middle 
// of her torso.  This is because the head model does not have animations in the Quake3
// config file, but the artist wanted to have her head and pony tail animate so they made it
// a sub-object in the torso "upper.md3" mesh.  
//
// Now that you understand the basics of a Quake3 character, let's get into the code!
// I would, however, advise you NOT to just cut and paste this code.  1) Because we will
// sue the pants off of you! (hehe, just kidding) and 2) The code is somewhat advanced
// that it should be drooled over and completely rewritten from scratch to understand what is
// really going on.  You are only hurting yourself (and possibly some hungry children in Africa)
// if you just copy and paste this code into your project.  I did, so you have to too! *Pout* :)
//
// It is assumed that by reading this tutorial you understand a decent amount of OpenGL and
// are comfortable with advanced topics.  This is a extremely advanced topic and should not
// be tackled until you get to a certain level of understanding in 3D.  I would recommend
// people who might not be up to that level to try looking at the MD2 Loader and animation.
// This is still an advanced tutorial, but not as complicated.  It only gets harder from here.
// The next MD3 Animation tutorial deals with quaternions which can throw you for a loop.
// Once again, this is an advanced tutorial so make sure you can handle it.  I don't want to
// get a ton of emails asking for help in understanding some of these concepts and I come to
// find out you have never written a file loader in your life.  I know programming is cool
// and fun, but don't be stupid and try to do more than you are able.  You will never be a good
// programmer if you jump up levels like a mad man, and it irritates others when you ask help
// on topics that you can't fully understand because you don't have that base.  Can you tell
// I get a lot of these emails and visit the message board a lot? :)  Don't get me wrong, I
// love to help people, but it's frustrating to try and teach others about doing sprite
// animation when they don't even know how to load a bitmap (as an example of way over your head).
//
// The controls for this application are:
//
// Left Arrow Key - Spins the model to the left
// Right Arrow Key - Spins the model to the right
// Up Arrow Key - Moves the camera closer to the model
// Right Arrow Key - Moves the camera farther away from the model
// W - Changes the Render mode from normal to wire frame.
// Escape - Quits
//
//

// Since loading a Quake3 character contains so many files, we want to store them all in
// one directory.  We create a define called MODEL_PATH which stores the path name that our
// character files are stored in.  The next define, MODEL_NAME, stores the prefix for the
// mesh, skin and animation files.  For instance, "lara" is then added to "_upper.md3" to
// make "lara_upper.md3".  The same goes for the other files.  You could just take advantage
// of the fact that each character could be in a different file and always load in the files
// as their original names like "upper.md3", "lower.md3", etc...
// The last define is our gun model file name.  The .MD3 extension will be added in for us.

#define MODEL_PATH  "lara"              // This stores the folder name the character is in
#define MODEL_NAME  "lara"              // This stores the prefix for all the character files
#define GUN_NAME    "Railgun"           // This stores the name of our gun to load

CModelMD3 g_Model;                      // This is our global md3 character model instance

float g_RotateX       =    0.0f;        // This is the current value at which the model is rotated
float g_RotationSpeed =    0.1f;        // This is the speed that our model rotates. 
float g_TranslationZ  = -120.0f;        // This stores our distance away from the model
bool  g_RenderMode    =    true;        // This tells us if we are in wire frame mode or not


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

    // To load a Quake3 character it's as simple as a first grade math test.  With our
    // g_Model instance, we just pass in the path and the character name prefix to LoadModel().
    // If we want, we can check the result to see if the character was loaded.

    // Load the 3 body part meshes and their skins and texture
    bool bResult = g_Model.LoadModel(MODEL_PATH, MODEL_NAME);

    // Next, if we want to give our character a gun, we just pass in the name of the gun
    // and the path it's stored in.  I just stuck the gun in the same path as our character.

    // Load the gun and attach it to our character
    bResult = g_Model.LoadWeapon(MODEL_PATH, GUN_NAME);

    // To get a decent speed increase, we turn on back face culling so we aren't drawing
    // the back sides of the models polygons.

    glEnable(GL_CULL_FACE);                             // Turn back face culling on
    glCullFace(GL_FRONT);                               // Quake3 uses front face culling apparently

    glEnable(GL_TEXTURE_2D);                            // Enables Texture Mapping
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing

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

    // As you can see below, to draw the model it's a piece of cake with our CModelMD3 class.
    // You'll notice that we added a translation variable to our camera's z position.  This
    // allows us to zoom in and zoom out without adding some camera code.

    // Give OpenGL our position,            then view,          then up vector
    gluLookAt(  0, 5.5f, g_TranslationZ,    0, 5.5f, 0,         0, 1, 0);
    
    // We want the model to rotate around the axis so we give it a rotation
    // value, then increase/decrease it. You can rotate right or left with the arrow keys.

    glRotatef(g_RotateX, 0, 1.0f, 0);           // Rotate the object around the Y-Axis
    g_RotateX += g_RotationSpeed;               // Increase the speed of rotation

    // Now comes the moment we have all been waiting for!  Below we draw our character.

    g_Model.DrawModel();                        //  Render our character to the screen

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

                /*case SDL_MOUSEBUTTONDOWN :
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
                    break;*/

                default:                                    // any other event
                    break;                                  // nothing to do
            } // switch
        } // while( SDL_ ...
        RenderScene();                                      // draw our OpenGL scene

    } // while( ! done)
}
/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
// 
// As you can see, loading and display the character and weapon is easy from
// the client side.  The rest of the underlying code though is a bit of a chore :)
// The controls used in this tutorial are the arrow keys, w and the escape key.
// See if you can see her baby in wire frame mode *grin*
//
// Let's go to a chunk of the explanation of .MD3 model loading at the bottom of Md3.cpp:
//
// The first thing you have to understand about characters in Quake3, is that the model
// is split into multiple files.  There is a upper.md3, a head.md3 and a lower.md3.
// These hold the upper body, the head and legs models.  Only the upper body and the
// lower body have animations assigned to them.  This is why you will see a little
// head in the middle of the body in wire frame mode because the Lara Croft model
// needed animation in her head, so the artist cut off the head and put it in a secret
// spot (her tummy... oooooow), then made her head and pony tail part of the upper body.
// A cool trick eh? (just wanted the Canadians to feel like I support them).  
// 
// Speaking of animation, so how does this stuff all work?  Is it bone animation or isn't it?
// Well, yes and now.  First, yes, there is bone animation because the legs are the parent
// node, and then the upper body attaches to the legs, then the head attaches to the upper 
// body (or in Lara's model, the torso).  Wherever the legs move, the upper body is then
// translated and rotated to the correct position that it should be, and then the head
// is in the same matrix scope so it goes where the upper body goes.  Once again, we do not
// handle rotating in this tutorial.  That requires a ton of matrix code that will be used
// in the next animation tutorial with quaternions (The room goes silent.....).
//
// And as for the second point, no the animation isn't exactly bone animation.  Each
// animation is stored in vertex key frames, just like the .MD2 files were.  The only
// difference is that we have 3 body parts to keep track of.  That way it is more realistic
// in the way of, you can have lara croft doing something totally different with her legs,
// than what the top part of her is doing.  Such as, when she is doing the jumping animation,
// her upper body could be doing a shooting, picking up, or dropping animation.  Make sense?
// So to the question of if this is bone animation, I can say a firm!!!..... Yes/No :)
//
// The .md3 files has skins and shader files associated with them.  It has a skin if it's
// a body part and a shader file if it's a weapon model.  These files store, for the most
// part, the textures associated with each model.  Sometimes the model has multiple textures,
// one for each sub-object.  I left the skin files the same, but I deleted a lot of the
// garbage that we didn't need in the .shader file.  Look at LoaderShader() for more info.
//
// There is also a config (.cfg) file for the animations of the model.  This stores the
// first frame, the amount of frames for that animation, the looping frame count (not used),
// and the frames per second that that animation should run.  We don't do anything with this
// file in this tutorial, but in the next tutorial we will.
//
// Finally, this brings us to the most confusing part in the .md3 model format, the tags.
// What the heck are tags?  Tags can be thought of as joints.  If you look in the .skin files
// you will see the tags on a separate line, right before the texture info.  These are basically
// joints that we need to connect other models too.  These are the ones I have seen:
//
// tag_torso  - The joint that connects the hips and the upper body together
// tag_head   - The joint that connects the neck and the head model too
// tag_weapon - The tag that connects the hand of the upper body and the weapon too.
//
// Now, that's the easy part to understand... it's like the Velcro that each body part
// sticks too right?  Well, the next part is where the confusion comes in.  Besides being
// a joint it stores the bone rotations and translations that need to be applied to the
// model that is connected to that joint.  For instance, we never rotate or translate the
// lower.md3 model.  That is all taken care of in the vertex key frame animation, but, the
// model stores tags for the "tag_torso" joint, which tells the upper body where to move and
// rotate, depending on what the legs are doing.
//
// A good example of this is in the death animations.  One of Lara's death animations makes
// her do a back flip and land on her face.  Well, the legs animation has key frames that
// it interpolates to, to perform this back flip, but the rest of the model doesn't.  To
// compensate for that, each frame, the lower body model sends a message to the upper body
// model to move and rotate to a certain degree and position, along a certain axis.  In
// programming terms, this means that we push on a new matrix and then apply some translations
// and rotations to the rest of the body parts (upper, head and weapon parts).  You don't
// directly apply it to each one, you just push on a matrix, apply the rotations and 
// translations, then anything drawn after that will be apart of that new matrix.  Simple huh?
// The rotation is stored in a 3x3 rotation matrix.  That is why we didn't bother with it
// in this tutorial because there is no animation so no need to add more code to scare you away.
// The translation is a simple (x, y, z) position that can be easily passed in to glTranslatef().
// This can all be seen in DrawLink().  In the next tutorial, we will not use glTranslatef(),
// but we will just include the translation in our matrix that we create to rotate the model.
// Then we can just do a simple call to glMultMatrix() to do both the rotation and translation
// at the same time.
//
// As a final explanation about the tags, let me address the model linking.  This just
// attaches 2 models together at the desired tag (or joint). For example, we attach the
// the upper.md3 model to the lower.md3 model at the "tag_torso" joint.  Now, when ever
// the legs do their animation, it sends the translation and rotation messages to all of
// it's children, which in this case, is just the upper body model, which effects the
// upper body's children (The weapon and head models).  The link array has nothing to do
// with the file format being loaded in.  That is just the way that I chose to handle it.
// I found some source code at www.planetquake.com by Lonerunner.  This guy/gal is awesome!
// He has a ton of formats that he did, which can be found at Romka's site too.  
//
// That is pretty much the crux of the .MD3 loading.  There is some bone animation to be
// loaded, but I never really figured it out.  The position was always (0, 0, 0) so I don't
// know what those are for.  I was going to display the bones, but it doesn't seem to save
// the position.  Who knows... if you know how, let me know.  Maybe I am ignoring something.
// It has min/max values, but I didn't bother to try and figure it out.
// 
// I would like to point out, this is just the solution I came up with.  There is probably
// a lot more intelligent ways to handle this, but it works great for me.  When the Quake3
// source code comes out, maybe we will all learn a few things from the mast'ehr :)
//
// I would like The author of this fabulous model who allowed me to use it, his handle is:
//
//          - Pornstar (nickelbag@nyc.com).  Tasteless name, but he sure does a cool model :)
//
//
// Let me know if this helps you out!
// 
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
// The Quake2 .Md3 file format is owned by ID Software.  This tutorial is being used 
// as a teaching tool to help understand model loading and animation.  This should
// not be sold or used under any way for commercial use with out written consent
// from ID Software.
//
// Quake, Quake2 and Quake3 are trademarks of ID Software.
// Lara Croft is a trademark of Eidos and should not be used for any commercial gain.
// All trademarks used are properties of their respective owners. 
//
//
