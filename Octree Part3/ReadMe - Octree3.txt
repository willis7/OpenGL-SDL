//***********************************************************************
//
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -
//
//      $Author:    DigiBen     digiben@gametutorials.com
//
//      $Program:   Octree3
//
//      $Description:   A working octree with a .3ds file format scene
//
//      $Date:      2/16/02
//
//***********************************************************************


Files:  Main.cpp   (The Source File containing the most worked with code)
        Init.cpp   (The Source File containing the rarely changed code)
        main.h     (The header file that holds the global variables and prototypes)
        OCtree.cpp (The Source File containing the octree class definitions
        Octree.h   (The header file that holds the octree class prototypes)
        Camera.cpp (The Source File containing the camera class definitions)
        Camera.h   (The header file that holds the camera class prototypes)
        Frustum.cpp(The Source File containing the frustum class definitions)
        Frustum.h  (The header file that holds the frustum class prototypes)
        Park.3ds   (The 3DS file that will be loaded)
                    

Libraries:      opengl , SDL 


Controls Used: w,s, UP_ARROW and DOWN_ARROW - moves the camera forward and
                                              backward
               a,d, RIGHT_ARROW and LEFT_ARROW - strafes the camera to the 
                                                 left and right
               Mouse Movement - Moves the view for first person mode
               F - turns Fog on and off
               SPACE - Turns on/off the yellow debug lines for the visual node
                       subdivisions
               ESCAPE - quits the program
               F1 - toggles fullscreen on and off
               
Instructions:   
        After unzipping the files, be in the 'Octree3' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './octree3' at the command prompt.

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
