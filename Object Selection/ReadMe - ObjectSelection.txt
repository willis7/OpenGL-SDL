//***********************************************************************
//                                          
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      
//                                                                       
//      $Author:    DigiBen     digiben@gametutorials.com       
//                                          
//      $Program:   ObjectSelection                     
//                                      
//      $Description:   Let's you select objects in 3D  
//                                          
//      $Date:      7/26/01                         
//                                          
//***********************************************************************


Files:  Main.cpp   (The Source File containing the most worked with code)
        Init.cpp   (The Source File containing the rarely changed code)
        Main.h     (The header file that holds the global variables and prototypes)
        Makefile   (The makefile, it stores the rules for compiling, linking)
        Sun.bmp    (The sun texture map)
        Earth.bmp  (The Earth texture map)
        Pluto.bmp  (The Pluto texture map)

Libraries:      OpenGL and SDL

Controls :      Left Mouse Click - This can be used to select a planet object
                Escape - Quits the application.


Instructions:   
        After unzipping the files, be in the 'ObjectSelection' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './ObjectSelection' at the command prompt.

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

        5.  If it doesn't run try setting the color depth in the program to
            something else (maybe 32 bit). You will have to change the macro
            in main.h for this.

        6.  If it doesn't go fullscreen try setting a lower desktop
            resolution.
        
        7.  If it gives a segmentation fault make sure that the image is a 24
            bit bitmap

        8.  If you still have some problems with the code
             - for OpenGL stuff mail the author
             - for SDL stuff mail the guys who ported this tutorial to SDL


Ben Humphrey
Game Programmer
digiben@gametutorials.com
www.GameTutorials.com

Ported to Linux/SDL by
    gOLeM   <golem@gametutorials.com>
    insane  <evilsmile@gametutorials.com>
