//*****************************************************************************//
//
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -
//
//      $Author:    DigiBen     digiben@gametutorials.com
//
//      $Program:   BSP Loader 3
//
//      $Description:   Loads bsp tree data and culls portals and leafs
//
//      $Date:      5/15/02
//
//****************************************************************************//


Files:  Main.cpp   (The Source File containing the most worked with code)
        Camera.cpp (The Source File containing the camera code)
        Camera.h   (The header file that holds the camera class)
        Init.cpp   (The Source File containing the rarely changed code)
        Main.h     (The header file that holds the global variables and prototypes)
        Image.cpp  (The source file that holds the image loading code)
        Image.h    (The header file that holds the image variables and prototypes)
        Quake3Bsp.cpp   (The source file that holds the Quake3 BSP class)
        Quake3Bsp.h     (The header file that holds the Quake3 BSP classes and structures)

Libraries:      opengl , jpeg , SDL 


Controls Used:  w, s, UP_ARROW, DOWN_ARROW - Move the camera forward and backward
         a, d, RIGHT_ARROW, LEFT_ARROW - Strafe the camera left and right
         Mouse movement - Moves the view for first person mode
         Right Mouse Button - Turns textures on and off
         Space Bar - Turns wireframe on and off
         ESCAPE - Quits the program

Instructions:   
        After unzipping the files, be in the 'BspLoader3_OGL' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './BspLoader3' at the command prompt.

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

            If you don't have the jpeg library, then download it
            from www.ijg.org:

        4.  If you don't have the OpenGL libraries, download the drivers
            for your video card from the manufacturer's site 
            and developer libraries from www.Mesa3d.org

        5.  If it doesn't run try setting the color depth in the program to
            something else (maybe 32 bit). You will have to change the macro
            in main.h for this.

        6.  If it doesn't go fullscreen try setting a lower desktop
            resolution.

        7.  If you still have some problems with the code
             - for OpenGL and BSP stuff mail the author
             - for SDL stuff mail the guys who ported this tutorial to SDL
                   (or u could post ur queries at the gametutorials forums)


Ben Humphrey (DigiBen)
Game Programmer
digiben@gametutorials.com
Co-Web Host of www.GameTutorials.com

Ported to Linux/SDL by
    gOLeM       <golem@gametutorials.com>
    EvilSmile   <evilsmile@gametutorials.com>
