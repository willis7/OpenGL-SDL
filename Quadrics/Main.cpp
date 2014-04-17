//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        DigiBen        digiben@gametutorials.com       //
//                                                                       //
//        $Program:        Quadrics                                      //
//                                                                       //
//        $Description:    Let's you create and texture Quadric shapes   //
//                                                                       //
//        $Date:            7/12/01                                      //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file

UINT  g_Texture[MAX_TEXTURES];                            // This will reference to our texture data stored with OpenGL UINT is an unsigned int (only positive numbers)

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This is effected when we click the LEFT mouse button.  It determines if
// the cylinder is draw in lines or in fill mode.  This was we can see through it.

int   g_RenderMode = GLU_FILL;                            // This holds the cylinders render state

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *



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
        
    CreateTexture(g_Texture, "bitmap.bmp",    0        );
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

                case SDL_MOUSEBUTTONDOWN :                  // if a mouse button was pressed
                    if( event.button.button == SDL_BUTTON_LEFT )        // and it's the left mouse button
                    {
                        if( g_RenderMode == GLU_FILL )      // check if we were in GLU_FILL mode
                            g_RenderMode = GLU_LINE;          // then we need to switch to GLU_LINE mode
                        else
                            g_RenderMode = GLU_FILL;          // Otherwise, we want GLU_FILL now.
                    }

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

float rotateX    = 0.0f;
float positionX     = 0.0f;
float directionX = 0.02f;

void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                    // Reset The matrix

        //       Position      View       Up Vector
    gluLookAt(0, 0, 6,     0, 0, 0,     0, 1, 0);        // This determines where the camera's position and view is

    // Bind the texture stored at the zero index of g_Texture[]
    glBindTexture(GL_TEXTURE_2D, g_Texture[0]);


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // To use Quadrics, we need to create a new one first.  This is done below.
    // The GLUquadricObj type is defined with the GLU32 library and associated header file.

    GLUquadricObj *pObj = gluNewQuadric();                // Get a Quadric off the stack

    // Instead of generating the U V coordinates of a Quadric, it does it for us.
    // This is REALLY helpful and easy to work with.  We need to tell OpenGL that we intend
    // for it to turn on texturing for our particular Quadric.  All we need to do this is
    // pass in our object, and then a "true" which says we want texturing.

    gluQuadricTexture(pObj, true);                        // This turns on texture coordinates for our Quadric

    // Below we rotate both of the Quadrics along the X-Axis.
    // This gives them that spining effect.

    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);                // Make the Quadrics spin along the X axis
    rotateX += 0.2f;                                    // Increase the rotation

    // Since a cylinder is created going down the Z axis, we want to rotate
    // it so it is going down the X axis.  This way the sphere can go through it.
    // We don't want to translate and rotate the sphere, so we use another matrix to
    // do this stuff.  That way it only effects the cylinder.  It's like a scope if(1) { }

    glPushMatrix();                                        // Start a new matrix scope

        // We translate the cylinder over by half of it's length so it's centered on the screen

        glTranslatef(-1.5f, 0.0f, 0.0f);                // Move the cylinder over so it's centered around (0, 0, 0)

        // Then we want to rotate it 90 degrees so it is along the X-Axis

        glRotatef(90, 0.0f, 1.0f, 0.0f);                // Rotate the cylinder around 90 degrees

        // We can set the draw style the Quadric.  The main options are: (GLU_FILL, GLU_LINE, GLU_POINT)
        // We want to be able to see through the cylinder so we made a global variable that is
        // triggered by the left mouse click to switch back and forth between GLU_FILL and GLU_LINE.
        // The default is GLU_FILL.  We need to pass in the object we wish to effect, then the style.

        gluQuadricDrawStyle(pObj, g_RenderMode);            // Draw the cylinder in lines

        // When we want to draw a Quadric, we just pass it into the desired shape function.
        // Since we want a cylinder, we use the gluCylinder() function.
        // It's parameters are :
        // (The object, the base radius, the top radius, height, definition X, definition Y)
        // The object we pass in holds all the settings.
        // The base radius is the radius of the bottom of the cylinder.
        // The top radius is obviously the radius of the top of the cylinder.
        // The height is how long the cylinder is.
        // The X and Y definition basically tells how many points to create it from.
        // The higher these numbers, the more vertices the cylinder will have. 
        // Too much can make it really slow.

        gluCylinder(pObj, 1, 1, 3, 20, 20);                // Draw our cylinder

    glPopMatrix();                                        // End the scope of the new matrix

    // Now that we popped the last matrix, anything we do here after will effect the sphere.

    // We want to make the sphere move back and forth, so we use glTranslatef() to do that.
    // We give it our current X position, and set the rest to 0.  We will go just along the X-Axis.
    // We then add the speed afterwards, which could be a negative or positive number, depending on directionX.
        
    glTranslatef(positionX, 0.0f, 0.0f);                // Set the new X position for the sphere
    positionX += directionX;                            // Add the speed to the current position

    // Since the cylinder could be in GLU_LINE mode, we just set the sphere explicitly to GLU_FILL.
    // Just because we used another matrix doesn't matter because we are using the same object (pObj).

    gluQuadricDrawStyle(pObj, GLU_FILL);                // Draw the sphere normally

    // Now we create a sphere.  We can use the same object if we want since the cylinder is already drawn.
    // The parameters for gluSphere() are:
    // (the object, the sphere's radius, the definition X, the definition Y);
    // These are pretty intuitive, but once again, the definition X and Y 
    // effects the amount of vertices the sphere has, the higher the number.

    gluSphere(pObj, 0.5f, 20, 20);                        // Draw the sphere with a radius of 0.5
    
    SDL_GL_SwapBuffers();                                // Swap the backbuffers to the foreground

    // After the scene has been drawn and we swapped the back buffer to the front,
    // we want to check to see if the sphere needs to turn around.  We just made it
    // go along a X path of -2 to 2.  If it goes past those coordinates, we change directions.

    if(positionX >= 2.0f || positionX <= -2.0f)            // Check if we went past our X-Axis limit
        directionX *= -1;                                // Turn the direction the opposite way

    // Since we are done rendering the Quadrics, we can free the object.
    // gluDeleteQuadric() takes the object to be released. If you have a lot 
    // of Quadrics, you might not want to allocate and free them every frame.

    gluDeleteQuadric(pObj);                                // Free the Quadric

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This tutorial was built from the texture map tutorial, so make sure you understand
// the texturing tutorial before you take this on.  
// 
// * So what the heck is a Quadric ??!?!??  *
//
// A quadric is basically a data type that allows you to draw a sphere and cylinder
// really simply.  It comes from the GLU library.  They are great for texturing and
// lighting without having to work with the UV coordinates or the normals.
//
// Quadrics are simple aren't they?  That is what makes them great, they are easy
// to use and easy to create.  They can also create their own normals for lighting.
// Check out the gluQuadricNormals() function for more information.
// 
// Here is a quick overview of Quadrics:
//
// 1) To create a Quadric, you need to first allocate one from the stack.
//    To do this, you call the gluNewQuadric() function.  This returns a pointer
//    to a GLUquadricObj.  You will then use the GLUquadricObj to pass into Quadric 
//    functions() so you can set the properties of that object.
// 
// 2) Next, you can just start drawing them by calling 1 of the 2 functions:
//    gluSphere() and gluCylinder().  Read in RenderScene() for more info on those.
// 
// 3) Before you render the Quadrics, you can change their draw style by calling:
//    gluDrawDStyle().  Some options include GLU_LINE GLU_POINT and GLU_FILL.
//    If you want the object to be texture mapped, after you call glBindTexture(),
//    called gluQuadricTexture() and pass in the object, then pass in true.
//    Make sure you do this BEFORE you call the gluSphere() or gluCylinder() functions.
//
// That's it!  They are drawn from the origin, so you will have to move them around with
// glRotatef() and glTranslate()/
//
// Let us know if this tutorial helped out, or you have some feedback or questions.
// 
// Good Luck!
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
