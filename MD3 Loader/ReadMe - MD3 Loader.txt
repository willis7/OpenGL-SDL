//***********************************************************************
//
//  - "Talk to me like I'm a 3 year old!" Programming Lessons -
//
//  $Author:    DigiBen     digiben@gametutorials.com
//
//  $Program:   MD3 Loader
//
//  $Description:   Demonstrates how to load Quake3 .Md3 files
//
//  $Date:      3/10/02
//
//***********************************************************************


Files:      Main.cpp   (The Source File containing the most worked with code)
            Init.cpp   (The Source File containing the rarely changed code)
            Main.h     (The header file that holds the global variables and prototypes)
            MD3.cpp    (The Source File containing the MD3 loading code)
            MD3.h      (The header file that holds the Quake3 definitions and
                        structures)
            Lara\      (The directory that stores all the character data
                        files)

Libraries:  opengl and SDL 

Instructions:
        After unzipping the files, be in the 'MD3Loader' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './Md3Loader' at the command prompt.
            2. To quit hit the ESC key     .
            3. Press F1 to toggle between fullscreen and windowed mode
            4. Right arrow key spins the model to the right
            5. left arrow key spins the model to the left 
            6. up arrow key moves the camera closer to the model
            7. down arrow key moves the camera farther from the model
            8. w - Changes the render mode from normal to wireframe 
               

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
