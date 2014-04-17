//***********************************************************************
//                                       
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      
//                                                                       
//      $Author:    DigiBen     digiben@gametutorials.com       
//                                          
//      $Program:   Camera                          
//                                          
//      $Description:   Move a camera back and forth     
//                                       
//      $Date:      6/18/01                          
//                                          
//***********************************************************************

Files:      Main.cpp   (The Source File containing the most worked with code)
            3DMath.cpp (The Source File containing the reusable math code)
            3DMath.h   (Header File that holds the 3DMath prototypes and VECTOR3D struct)
            Camera.cpp (The Source File containing the camera code)
            Camera.h   (The header file that holds the camera class prototypes)
            Init.cpp   (The Source File containing the rarely changed code)
            main.h     (The header file that holds the global variables and prototypes)

            World.raw  (The ASCII text file holding the raw vertices for the world)
            TexturedWorld.jpg (This file is not used in the tutorial, it is
                        just to show you how it would look with textures)

Libraries:  opengl and SDL 

Instructions:
        After unzipping the files, be in the 'CameraWorldCollision' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './CameraWorldCollision' at the command prompt.
            2. To quit hit the ESC key     .
            3. Press F1 to toggle between fullscreen and windowed mode

        Keys Used:
            1. W or up arrow moves forward
            2. S or down arrow moves backward
            3. A or left arrow moves left
            4. D or right arrow moves right

        and the mouse is used to move the direction in which the camera points

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
