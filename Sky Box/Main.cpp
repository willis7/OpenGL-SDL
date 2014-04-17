//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        DigiBen        digiben@gametutorials.com       //
//                                                                       //
//        $Program:        SkyBox                                        //
//                                                                       //
//        $Description:    This shows how to create a textured sky box   //
//                                                                       //
//        $Date:            11/2/01                                      //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // Our header file

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This tutorial shows you how to create a sky box.  A sky box is just a 3D box that
// has textures assigned to it which create an illusion that you are actually in
// a intricate world.  This is great because it it's fast to draw and looks cool in
// the background.  The textures used in this tutorial were created by Nick Coombe at
// http://www.planethalflife.com/crinity  | email: crinity@email.com.  I appreciate
// his artistic talent and for letting me use these for this tutorial and on our CD.
// How this works is, we create a box, and then assign the top, bottom, left, right,
// back and front textures to each side of the cube accordingly.  This will make it
// look like we are in a cool world, not an ugly cube :).  I struggled with adding
// camera support to this tutorial, but eventually gave in.  Though it might complicate
// an easy tutorial I figured there is no way to really get the effect without being
// able to look around.  You can use the mouse to look around, and the arrow keys
// to move within the sky box.  This tutorial was taken partly from the RotateCube
// tutorial.  I grabbed the camera and texture code from the TextureMap and Camera3 tuts.

// Below we create the new needed variables for this tutorial

#define SPEED    0.03f                                    // This is how fast our camera moves

// These defines correspond to an index within the g_Texture[] array.
// We will use these defines to bind the appropriate texture to the
// right side of the box.

#define BACK_ID        0                                    // The texture ID for the back side of the cube
#define FRONT_ID    1                                    // The texture ID for the front side of the cube
#define BOTTOM_ID    2                                    // The texture ID for the bottom side of the cube
#define TOP_ID        3                                    // The texture ID for the top side of the cube
#define LEFT_ID        4                                    // The texture ID for the left side of the cube
#define RIGHT_ID    5                                    // The texture ID for the right side of the cube

UINT g_Texture[MAX_TEXTURES] = {0};                        // This holds the texture info, referenced by an ID

          //     Position  View Vector   Up Vector
CCamera    g_Camera(0, 0, 0,     0, 0, 6,     0, 1, 0);        // Create our camera object and initialize its position around the origin


///////////////////////////////// CREATE SKY BOX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This creates a box centered around X Y Z.  Instead of colors at
/////   each vertices there is texture maps for each side of the box to 
/////   create the illusion of the sky box.  You can set the width, height and length
/////   of the sky box to create different perspectives for different sky box textures.
/////
///////////////////////////////// CREATE SKY BOX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateSkyBox(float x, float y, float z, float width, float height, float length)
{
    // This is the most important function of this tutorial.  This function
    // used to just create a silly colored cube in the RotateCube tutorial,
    // but now it creates something beautiful.  You'll notice we added
    // some more parameters to the function.  This way we can change the perspective
    // of the sky box.  It doesn't really look good if it's a perfect cube.  Some
    // textures look better at different ratios.  We assign the sky box textures
    // to each side of the box creating the illusion of a detailed 3D world.
    // You will notice I had to change the texture coordinates for every one
    // to be flipped correctly.  Also, depending on your culling, the vertex
    // order might need to be changed around.  I don't use culling in this tutorial
    // so it will work fine here, but be sure to remember this if you do.

    // Bind the BACK texture of the sky map to the BACK side of the cube
    glBindTexture(GL_TEXTURE_2D, g_Texture[BACK_ID]);

    // Since we want the sky box to be centered around X, Y, and Z for ease,
    // we do a little math to accomplish this.  We just change the X, Y and Z
    // to perform this task.  If we just minus half the width, height and length
    // from x, y and z it will give us the desired result.  Now when we create the
    // box it will center it around (x, y, z)

    // This centers the sky box around (x, y, z)
    x = x - width  / 2;
    y = y - height / 2;
    z = z - length / 2;

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);        
        
        // Assign the texture coordinates and vertices for the BACK Side
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x,            y,            z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x,            y + height, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,            z);
    glEnd();

    // Bind the FRONT texture of the sky map to the FRONT side of the box
    glBindTexture(GL_TEXTURE_2D, g_Texture[FRONT_ID]);

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);    
    
        // Assign the texture coordinates and vertices for the FRONT Side
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,            y,            z + length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,            y + height, z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,            z + length);
    glEnd();

    // Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
    glBindTexture(GL_TEXTURE_2D, g_Texture[BOTTOM_ID]);

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);        
    
        // Assign the texture coordinates and vertices for the BOTTOM Side
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,            y,            z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,            y,            z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,            z + length); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,            z);
    glEnd();

    // Bind the TOP texture of the sky map to the TOP side of the box
    glBindTexture(GL_TEXTURE_2D, g_Texture[TOP_ID]);

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);        
        
        // Assign the texture coordinates and vertices for the TOP Side
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,            y + height,    z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,            y + height,    z + length);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glEnd();

    // Bind the LEFT texture of the sky map to the LEFT side of the box
    glBindTexture(GL_TEXTURE_2D, g_Texture[LEFT_ID]);

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);        
        
        // Assign the texture coordinates and vertices for the LEFT Side
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,            y,            z);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x,            y,            z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x,            y + height,    z + length); 
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,            y + height,    z);
    glEnd();

    // Bind the RIGHT texture of the sky map to the RIGHT side of the box
    glBindTexture(GL_TEXTURE_2D, g_Texture[RIGHT_ID]);

    // Start drawing the side as a QUAD
    glBegin(GL_QUADS);        

        // Assign the texture coordinates and vertices for the RIGHT Side
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,            z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,            z + length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,    z + length); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,    z);
    glEnd();
}


///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
    // Enable key repeat 
    if ( SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL) )
    {
        cerr << "Failed enabling key repeat : " << SDL_GetError() << endl;
        Quit(1);
    }

    InitializeOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);    // Init OpenGL
        
    // Below we read in the 6 texture maps used for the sky box.
    // The ID's are important, so don't mix them up.  There is a
    // texture for every side of the box.

    CreateTexture(g_Texture, "Back.bmp",    BACK_ID        );
    CreateTexture(g_Texture, "Front.bmp",    FRONT_ID    );
    CreateTexture(g_Texture, "Bottom.bmp",    BOTTOM_ID    );
    CreateTexture(g_Texture, "Top.bmp",    TOP_ID        );
    CreateTexture(g_Texture, "Left.bmp",    LEFT_ID        );
    CreateTexture(g_Texture, "Right.bmp",    RIGHT_ID    );
}


///////////////////////////////// KEY PRESSED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function handles the input more effectively than in the WinProc()
/////
///////////////////////////////// KEY PRESSED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void KeyPressed(SDL_keysym * keysym)
{
    switch(keysym -> sym)
    {
        case SDLK_F1 :                      // If the key is F1 
            ToggleFullScreen();             // Toggle between FullScreen and windowed mode
            break;

        case SDLK_ESCAPE :                  // if the key is ESCAPE
            Quit(0);

        case SDLK_LEFT :                     // If we hit the LEFT arrow key
             // We want to rotate around the Y axis so we pass in a Y speed.
             g_Camera.RotateView(0, -SPEED, 0);        // Rotate our camera LEFT by the negative SPEED
             break;

        case SDLK_RIGHT :                    // If we hit the RIGHT arrow key 
             // Use a positive Y speed to rotate around the Y axis
             g_Camera.RotateView(0, SPEED, 0);        // Rotate our camera LEFT by the negative SPEED
             break;
        
        case SDLK_UP :                         // If we hit the UP arrow key
             g_Camera.MoveCamera(SPEED);                // Move our camera forward by a positive SPEED
             break;
        
        case SDLK_DOWN :                     // If we hit the DOWN arrow key
             g_Camera.MoveCamera(-SPEED);                // Move our camera backward by a negative SPEED
             break;

        default:
             break;
    }
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
                    KeyPressed( & event. key.keysym );                  // callback for handling keystrokes, arg is key pressed
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

                case SDL_MOUSEMOTION :                      // If the mouse is moved 
                    g_Camera.MoveCameraByMouse();           // Check the mouse camera movement
                    break;

                default:                                    // any other event
                    break;                                  // nothing to do
            } // switch
        } // while( SDL_ ...

        RenderScene();                                      // draw our OpenGL scene
    } // while( ! done)
}

///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                    // Reset The matrix
    

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

        // Give openGL our camera position, then camera view, then camera up vector
    gluLookAt(g_Camera.m_vPosition.x, g_Camera.m_vPosition.y, g_Camera.m_vPosition.z,    
              g_Camera.m_vView.x,      g_Camera.m_vView.y,     g_Camera.m_vView.z,    
              g_Camera.m_vUpVector.x, g_Camera.m_vUpVector.y, g_Camera.m_vUpVector.z);

    // Here we create our sky box.  It will be centered around (0, 0, 0) with
    // a width of 400, height of 200 and length of 400.  You might wonder why we
    // don't make it a perfect cube?  Well, if we make the height half the size
    // of the width and length it looks better.  Try putting the height to 400 and
    // you will notice that everything looks really close up.  You will want to tweak
    // this ratio most likely for every sky maps you use.  Some look better at different
    // perspectives and ratios.  Try changing the height to 100.  It looks more flat
    // and at a greater distance.  

    // Draw the sky box centered at (0, 0, 0)
    CreateSkyBox(0, 0, 0, 400, 200, 400);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


    SDL_GL_SwapBuffers();                                    // Swap the backbuffers to the foreground
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This tutorial shows you how to create a sky box.  A sky box is just a box that
// has textures assigned to it which create an illusion that you are actually in
// a intricate world.  This is great because it it's fast to draw and looks cool in
// the background.  There are a lot of these textures all over the web.
// How this works is, we create a box, and then assign the top, bottom, left, right,
// back and front textures to each side of the box accordingly.  This will make it
// look like we are in a cool world, not an ugly box :).  I struggled with adding
// camera support to this tutorial, but eventually gave in.  Though it might complicate
// an easy tutorial I figured there is no way to really get the effect without being
// able to look around.  You can use the mouse to look around, and the arrow keys
// to move within the sky box.  This tutorial was taken partly from the RotateCube
// tutorial.  I grabbed the camera and texture code from the TextureMap and Camera3 tuts.
//
// One IMPORTANT thing when using textures for sky maps is that
// the default GL_TEXTURE_WRAP_S and ""_WRAP_T texture property is GL_REPEAT.
// We need to turn this to GL_CLAMP_TO_EDGE, otherwise it creates ugly seems
// in our sky box.  GL_CLAMP_TO_EDGE does not repeat when bound to an object.
// To set this texture property we added these 2 lines to CreateTexture();
//
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//
// The textures used in this tutorial were created by Nick Coombe at
// http://www.planethalflife.com/crinity  | email: crinity@email.com.  I appreciate
// his artistic talent and for letting me use these for this tutorial and on our CD.
// Check out his site, it is REALLY cool.  There are tons more of these textures.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
