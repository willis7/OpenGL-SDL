//***********************************************************************
//                                   
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      
//                                                                       
//      $Author:    DigiBen     DigiBen@GameTutorials.com    
//                                      
//      $Program:   Normals                     
//                                          
//      $Description:   This shows how to find the normal of a polygon.  
//                                          
//      $Date:      7/8/01                      
//                                      
//***********************************************************************

Files:      Main.cpp   (The Source File containing the most worked with code)
            3DMath.cpp (The Source File containing the reusable math code)
            3DMath.h   (Header File that holds the 3DMath prototypes and VECTOR3D struct)
            Init.cpp   (The Source File containing the rarely changed code)
            main.h     (The header file that holds the global variables and prototypes)

Libraries:  opengl and SDL 

Instructions:
        After unzipping the files, be in the 'Normals' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './Normals' at the command prompt.
            2. To quit hit the ESC key     .
            3. Press F1 to toggle between fullscreen and windowed mode
            4. Right arrow key moves the top of the triangle forwards and up
            5. Right left key moves the top of the triangle backwards and down 

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
