//***********************************************************************
//
//  - "Talk to me like I'm a 3 year old!" Programming Lessons -
//
//  $Author:    DigiBen     digiben@gametutorials.com
//
//  $Program:   MD2 Animation
//
//  $Description:   Demonstrates how to animation Quake2 .Md2 file with key
//  frame animation
//
//  $Date:      2/10/02
//
//***********************************************************************


Files:  Main.cpp   (The Source File containing the most worked with code)
        Init.cpp   (The Source File containing the rarely changed code)
        Main.h     (The header file that holds the global variables and prototypes)
        Md2.cpp    (The Source File containing the Md2 loading and animating
                        code)
        Md2.h      (The header file that holds quake 2 definitions and
                        structures)
        hobgoblin.bmp (The texture map for the player model)
        Tris.md2      (The .MD2 that will be loaded and animated)
        

Libraries:      opengl , SDL 


Controls Used: Left Mouse Button - Changes Render Mode from normal to
                    wireframe
               Right Mouse Button - This cycles through animations of the
                    model
               Left Arrow Key - Spins the model to the left
               Right Arrow Key - Spins the model to the right
               ESCAPE - quits the program
               F1 - toggles fullscreen on and off
               
Instructions:   
        After unzipping the files, be in the 'MD2Animation' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './Md2Animation' at the command prompt.

TROUBLESHOOTING INFO:
        1.  If it doesn't compile make sure you have the SDL ,OpenGL and jpeg
            libraries installed on your system.

        2.  The code is amply commented. However, if you still find it
            difficult to follow the SDL code check out the documentation
            (might be installed to /usr/share/doc/SDL-devel-*/)
            The documentation is fantastic and makes SDL look very easy
            and intuitive to learn.

        3.  If you don't have the SDL libraries(developer libraries also ) you
            can download from the official site www.libsdl.org

        4.  If you don't have the OpenGL libraries, download the drivers
            for your video card from the manufacturer's site 
            and developer libraries from www.Mesa3d.org

        5.  If it doesn't run try setting the color depth in the program to
            something else (maybe 32 bit). You will have to change the macro
            in main.h for this.

        6.  If it doesn't go fullscreen try setting a lower desktop
            resolution.

        7.  If you still have some problems with the code
             - for OpenGL stuff mail the author
             - for SDL stuff mail the guys who ported this tutorial to SDL


Ben Humphrey (DigiBen)
Game Programmer
digiben@gametutorials.com
Co-Web Host of www.GameTutorials.com

Ported to Linux/SDL by
    gOLeM       <golem@gametutorials.com>
    EvilSmile   <evilsmile@gametutorials.com>
