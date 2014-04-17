//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     digiben@gametutorials.com            //
//                                                                       //
//      $Program:       Multitexturing                                   //
//                                                                       //
//      $Description:   Shows how to multitexture a polygon              //
//                                                                       //
//      $Date:          1/26/02                                          //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file
#include <GL/glext.h>

unsigned int g_Texture[MAX_TEXTURES];                           // This will reference to our texture data stored with OpenGL (only positive numbers)

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This tutorial was taken from the original texture mapping tutorial.  Though not much code
// is added to this tutorial, it sure does something powerful, multitexturing.  With the
// constant upgrades from OpenGL, single pass multitexturing was thrown in along the way.
// The API's for multitexturing are what is called "extensions".  You either need the prototypes
// copied into your program, or you can carry around the glext.h file that holds all of
// the recent extensions created.  If you notice, there are tons of them out there.
// Extensions are things that are not a part of the normal opengl API, but have been added
// in as extras.  You have to have the latest version of OpenGL to allow multitexturing.
// I believe version 1.3 is the current version, at least for Microsoft windows.  To check
// if you have multitexturing enabled, try using the function:  glGetString(GL_EXTENSIONS)
// This returns a (char *) that holds all the extensions that are available to you.
// 
// * What is multitexturing? *
//
// Multitexturing is having 2 or more layers of textures on top of each other.  This is used
// for caustic effects, light maps, shadows, along with a few other graphic techniques.  You can
// sometimes accomplish the same effects with normal multitexturing, but you need to perform
// more than one pass, which means you draw the geometry more than once.  For instance, you
// could draw your wall, then just a tiny bit in front of it, draw a light map on the same wall.
// One problem with this is that if you wanted to cast a light map over a small part of a terrain, you
// would have some trouble doing so.  
//
// This application gives 2 examples of multitexturing.  The first one is a simple light map,
// while the second one is a neat background with fog rolling over top of it.
// To use the extensions, we need to load the functions from the OpenGL DLL.  Let take a look
// at the code
//
//


///////////////////////////////// IsExtensionSupported \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This Function checks whether a particular extension is supported or not
/////    
///////////////////////////////// IsExtensionSupported \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool isExtensionSupported(string ext)
{
    int pos = 0;
    bool supported = false;

    int n = ext.length();

    string extensions ((char *)glGetString(GL_EXTENSIONS));

    while( !supported )
    {
        if ( extensions.compare(pos, n, ext) >= 0 )                    // if the string `ext' is a part of the string `extensions'
        {
            return true;
        }
        pos = extensions.find(' ' , pos) + 1;

        if( pos <= 0 )
            return false;
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
    
        //    Position      View       Up Vector
    gluLookAt(0, 0, 6,     0, 0, 0,     0, 1, 0);       // This determines where the camera's position and view is


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
    
    // Now comes the fun part.  We need to specify which texture goes to what ID.  The ID's
    // go up to 31, with 0 a valid ID.  That means you could define up to 32 textures, but
    // of course you card would have to handle that many.  We only use 2.  To assign a texture
    // to an ID, we just set our texture ID active with glActiveTexture(), then call glBindTexture()
    // with out texture index.  Then, whenever we use glMultiTexCoord2fARB(), we just give it the
    // texture ID, such as GL_TEXTURE0_ARB, and it knows that the texture coordinates are for
    // the texture that was found to that ID.

    // Set the first texture ID to be active, then bind our brick texture to this ID.
    // Also, before you bind your texture, you MUST call: glEnable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,  g_Texture[0]);

    // Set the second texture ID to be active, then bind our light map texture to this ID.
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,  g_Texture[1]);

    // Because we want to translate the textured QUADS to different locations, we push
    // on a matrix so that the other QUAD is not effect by our translation of the previous one.
    // We do not have to push and pop on another matrix for our other QUAD because there is no
    // geometry rendered after it.  The matrix will be nixed anyway.
    glPushMatrix();

        // This translates the current matrix to move the first QUAD over to the left
        // of the screen.
        glTranslatef(-2, 0, 0);

        // Display a multitextured quad to the left side of the screen
        glBegin(GL_QUADS);

            // Now, instead of using glTexCoord2f() we use glMultiTextCoord2fARB() to
            // specify the texture coordinates for each texture.  This allows us to move
            // one texture to a totally different place on the geometry.  A perfect example
            // of this is shadows.  glMultiTexCoord2fARB() takes the texture ID we wish
            // to specify, with it's texture coordinates afterwards.  We just use the same
            // texture coordinates for each texture.

            // Display the top left vertice with the texture coordinates for both textures
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
            glVertex3f(-1, 1, 0);

            // Display the bottom left vertice with the textures for both textures
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
            glVertex3f(-1, -1, 0);

            // Display the bottom right vertice with the textures for both textures
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
            glVertex3f(1, -1, 0);

            // Display the top right vertice with the textures for both textures
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
            glVertex3f(1, 1, 0);

        // Stop drawing 
        glEnd();                                            

    // Here we pop the last matrix so that it does not effect our next QUAD.  Now we
    // are rendering with the original identity matrix.
    glPopMatrix();

    // Next, we want to draw the second example of multitexuring on the right of the screen.
    // We can reassign new textures to our GL_TEXTURE*_ARB ID's.  We could have used
    // GL_TEXTURE2_ARB and GL_TEXTURE3_ARB, but no need.

    // Activate the first texture ID and bind the tree background to it
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D,  g_Texture[2]);

    // Activate the second texture ID and bind the fog texture to it
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D,  g_Texture[3]);

    // To make the fog look cool, we scroll it's texture over top of the background picture.
    // To do this, we just create a static float to hold the current count.  Since our texture
    // is GL_WRAP, it will have a seamless effect.

    // Create the counter variable
    static float wrap = 0;

    // Move the QUAD over to the right of the screen
    glTranslatef(2, 0, 0);

    // Display a multitextured quad texture to the screen
    glBegin(GL_QUADS);

        // Display the top left vertice with each texture's texture coordinates.  We subtract
        // the "wrap" variable to product a scrolling effect with the fog.  It makes it look
        // More real.
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f - wrap, 1.0f);
        glVertex3f(-1, 1, 0);

        // Display the bottom left vertice with each texture's coodinates
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f - wrap, 0.0f);
        glVertex3f(-1, -1, 0);

        // Display the bottom right vertice with each texture's coodinates
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f - wrap, 0.0f);
        glVertex3f(1, -1, 0);

        // Display the top right vertice with each texture's coodinates
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f - wrap, 1.0f);
        glVertex3f(1, 1, 0);

    // Stop drawing QUADS
    glEnd();                                            

    // Each frame we increase the wrap by a small amount.  The larger the amount 
    // we increase it, the faster the fog scrolls.
    wrap += 0.0015f;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Swap the backbuffers to the foreground
    SDL_GL_SwapBuffers();                                   
}

///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    InitializeOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);    // Init OpenGL with the global rect

    if( !isExtensionSupported(string("GL_ARB_MULTI_TEXTURE") ) )
    {
        cout << "Error , MultiTexturing not supported" << endl;
        SDL_Quit();
    }
    // There will be 4 textures loaded to demonstrate the effect.  We use our own CreateTexture to
    // load the bitmaps and save them in OpenGL texture memory.

    CreateTexture(g_Texture, "Bitmap.bmp", 0);              // Load the brick wall into our first texture index
    CreateTexture(g_Texture, "Lightmap.bmp", 1);            // Load the light map into our second texture index
    CreateTexture(g_Texture, "Cove.bmp", 2);                // Load the background picture into our third texture index
    CreateTexture(g_Texture, "fog.bmp", 3);                 // Load the fog into our fourth texture index

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
        RenderScene();                              // Redraw the scene every frame
    } // while( ! done)
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// That is it!  I bet you thought there was more to this didn't you?  Well, there
// is actually.  We just skimmed the surface.  You will want to be able to
// fade certain textures, and have the background picture be more bright,
// or vice versa.  You will want to use the EXT_texture_env_combine extension.
// More on this extension in a future tutorial.  This extension will also
// allow you to do bump mapping extremely fast.
//
// Let's review:
//
// 1) Before we do anything, we need to create some function pointers to hold the
// extensions that are read in.  In order to use these data types, we need to have
// the prototypes either copied in to our program, or just include glext.h.
// you can just stick this file into your include\GL\ directory, where all your other
// opengl include files are: /usr/include/GL/
//
// 2) We check whether our card has multitexturing supports multitexturing or not.
// For this we get the glGetString(GL_EXTENSIONS) ;
// in this string we check for GL_ARB_MULTI_TEXTURE
// If we find it in that string then , Multitexturing is supported .
// Otherwise it is not supported
//
// 3) When we are about to render, we need to to assign our texture to a multitexture ID.
// That way, we can pass in that ID to specify which texture we are giving texture coordinates too.
//
// glActiveTextureARB(GL_TEXTURE0_ARB);
// glEnable(GL_TEXTURE_2D);
// glBindTexture(GL_TEXTURE_2D,  g_Texture[0]);
//
// 4) Now, when we want to give texture coordinates for our texture, we simply pass in the
// texture ID we want and it's texture coordinates to glMultiTexCoord2fARB().
//
// glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
// glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f - wrap, 1.0f);
// glVertex3f(-1, 1, 0);
//
// That's it!  Let us know at GameTutorials.com if this tutorial helps you.  We would love to
// see what you come up with.
// 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
