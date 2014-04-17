//***********************************************************************
//
//  - "Talk to me like I'm a 3 year old!" Programming Lessons -
//
//  $Author:    DigiBen     digiben@gametutorials.com
//
//  $Program:   MD2 Loader
//
//  $Description:   Demonstrates how to load a Quake2 .Md2 file
//
//  $Date:      2/6/02
//
//***********************************************************************

Files:      Main.cpp   (The Source File containing the most worked with code)
            Init.cpp   (The Source File containing the rarely changed code)
            Main.h     (The header file that holds the global variables and prototypes)
            MD2.cpp    (The Source File containing the MD2 loading code)
            MD2.h      (The header file that holds the Quake2 definitions and
                        structures)
            tris.md2   (The actual md2 model that is to be loaded)
            hobgoblin.bmp   (The texture file for the character to be loaded)

Libraries:  opengl and SDL 

Instructions:
        After unzipping the files, be in the 'MD2Loader' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './Md2Loader' at the command prompt.
            2. To quit hit the ESC key     .
            3. Press F1 to toggle between fullscreen and windowed mode
            4. Right arrow key spins the model to the right
            5. left key spins the model to the left 
            6. Right Mouse Button - turns the lighting On/Off
            7. Left Mouse Button - Changes the render mode from normal to
               wireframe 

TROUBLESHOOTING INFO:
        1.  If it doesn't compile make sure you have the SDL and OpenGL
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

        5.  If you still have some problems with the code
             - for OpenGL stuff mail the author
             - for SDL stuff mail the guys who ported this tutorial to SDL

Ben Humphrey
Game Programmer
digiben@gametutorials.com
www.GameTutorials.com

ported to Linux/SDL by
    gOLeM       <golem@gametutorials.com>
    EvilSmile   <evilsmile@gametutorials.com>
