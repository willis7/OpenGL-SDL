//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        DigiBen        digiben@gametutorials.com       //
//                                                                       //
//        $Program:        Specular Lighting                             //
//                                                                       //
//        $Description:    Demonstrates specular lighting                //
//                                                                       //
//        $Date:            9/22/01                                      //
//                                                                       //
//***********************************************************************//

#include "main.h"                                        // This includes our header file

// This tutorials shows you how to add specular lighting to your scene.
// Specular lighting is the intense white light created on shiny objects,
// like metalic surfaces.  The code for this tutorial is created off of
// the Quadric tutorial.  Very little code is added or changed.  If you
// are not comfortable with Quadrics, I left the comments explaining what
// is going on.  If you are familiar with them, you can just focus on
// the Init() function.  That is all that is need to set the specular lighting.
// Also, this tutorial still allows the user to click the left mouse button
// and turn the cylinder to and from wire frame mode.  We did add the option
// now to increase and decrease the shininess value by using the '-' and '+' keys.


// This is effected when we click the LEFT mouse button.  It determines if
// the cylinder is draw in lines or in fill mode.  This was we can see through it.

int   gRenderMode = GLU_FILL;                            // This holds the cylinders render state


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This is the intensity of the specular highlighting.  The lower this
// number is, the more intense the specular is.  1 is full specular, and
// around 50 is the lowest amount.  You can use the '+' and '-' keys to
// change this value when running the application.
float g_Shine = 10.0;

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

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // This array is used to determine the specular color.  Usually the
    // color is white, so we will use pure white.  You change the
    // color to what ever you want though, depending on the color of the light source.
    float specularColor[3] = {1.0, 1.0, 1.0};

    // This is where we actually set the specular color and intensity.

    // To change the material properties of our objects, we use glMaterialfv().
    // GL_FRONT_AND_BACK says that we want this applied to both sides of the polygons.
    // GL_SPECULAR say that we want to set the specular color.
    // specularColor is our color array, so we pass that in lastly.
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

    // The same thing applies as above, but this time we pass in
    // GL_SHININESS to say we want to change the intensity of the specular.
    // We then pass in an address to our shine intensity, g_Shine.
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &g_Shine);

    // That's it!  That is how we add specular.  Simple huh?  It is such a cool
    // effect, as well as being easy to implement.  To get the full effect, I
    // suggest adding specular to some objects that aren't so rounded.  I didn't
    // want to complicate the tutorial so I just chose a sphere and a cylinder.
    // The only other thing we do with specular in this tutorial is down in
    // the WinProc().  I give the user the option of changing the intensity.
    // If you want to change the DIFFUSE color use GL_DIFFUSE and pass in a
    // array with the color values to glMaterialfv().

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

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

        case SDLK_KP_PLUS :                          // Check if we hit the PLUS key
        case SDLK_EQUALS  :
            g_Shine -= 2;                             // Increase the specular value
            
            if( g_Shine < 0 ) g_Shine = 0;             // Make sure that we dont go below 0 (Full Shine)
            
            // We can set the intensity of the specular any time we want.
            // Here we pass in the new shine value
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &g_Shine);

            break;
        
        case SDLK_KP_MINUS :                         // Check if we hit the MINUS key
        case SDLK_MINUS    :

            g_Shine += 2;

            if( g_Shine > 50 ) g_Shine = 50;         // make sure that we dont go above 50

            // We can set the intensity of the specular any time we want.
            // Here we pass in the new shine value
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &g_Shine);

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

                case SDL_MOUSEBUTTONDOWN :
                    if( event.button.button == SDL_BUTTON_LEFT )        // if the left mouse button was clicked
                    {
                        if( gRenderMode ==  GLU_FILL )                    // check if we are in GLU_FILL mode
                            gRenderMode = GLU_LINE ;                    // then we need to switch to GLU_LINE mode
                        else
                            gRenderMode = GLU_FILL ;                    // otherwise we want GLU_FILL now
                    }
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
    static float rotateX    = 0.0f;                        // Make a static rotation variable
    static float positionX    = 0.0f;                        // Make a static position variable
    static float directionX = 0.02f;                    // Make a static direction variable

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                    // Reset The matrix

        //       Position      View       Up Vector
    gluLookAt(0, 0, 6,     0, 0, 0,     0, 1, 0);        // This determines where the camera's position and view is

    // To use Quadrics, we need to create a new one first.  This is done below.
    // The GLUquadricObj type is defined with the GLU32 library and associated header file.

    GLUquadricObj *pObj = gluNewQuadric();                // Get a Quadric off the stack


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Below we rotate both of the Quadrics along the X-Axis.
    // This gives them that spining effect.

    glRotatef(rotateX, 1.0f, 1.0f, 1.0f);                // Make the Quadrics spin along the X axis
    rotateX += 0.8f;                                    // Increase the rotation

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // Since a cylinder is created going down the Z axis, we want to rotate
    // it so it is going down the X axis.  This way the sphere can go through it.
    // We don't want to translate and rotate the sphere, so we use another matrix to
    // do this stuff.  That way it only effects the cylinder.  It's like a scope if(1) { }

    glPushMatrix();                                        // Start a new matrix scope

        glColor3ub(150, 150, 150);

        // We translate the cylinder over by half of it's length so it's centered on the screen

        glTranslatef(-1.5f, 0.0f, 0.0f);                // Move the cylinder over so it's centered around (0, 0, 0)

        // Then we want to rotate it 90 degrees so it is along the X-Axis

        glRotatef(90, 0.0f, 1.0f, 0.0f);                // Rotate the cylinder around 90 degrees

        // We can set the draw style the Quadric.  The main options are: (GLU_FILL, GLU_LINE, GLU_POINT)
        // We want to be able to see through the cylinder so we made a global variable that is
        // triggered by the left mouse click to switch back and forth between GLU_FILL and GLU_LINE.
        // The default is GLU_FILL.  We need to pass in the object we wish to effect, then the style.

        gluQuadricDrawStyle(pObj, gRenderMode);            // Draw the cylinder in lines

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

        gluCylinder(pObj, 1, 1, 3, 40, 3);                // Draw our cylinder

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

    glColor3ub(255, 255, 0);                            // Set the color of the sphere to yellow

    // Now we create a sphere.  We can use the same object if we want since the cylinder is already drawn.
    // The parameters for gluSphere() are:
    // (the object, the sphere's radius, the definition X, the definition Y);
    // These are pretty intuitive, but once again, the definition X and Y 
    // effects the amount of vertices the sphere has, the higher the number.

    gluSphere(pObj, 0.5f, 25, 25);                        // Draw the sphere with a radius of 0.5
    
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
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// In this tutorial we show how to add specular lighting to your objects.
// This is simple to add.  All you need to do is call glMaterialfv() and
// give it the values for GL_SPECULAR and GL_SHININESS.
// 
// Once again, the sepcular hilighting is best viewed with a more abstract
// and less rounded object.
// 
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
// © 2001 GameTutorials
