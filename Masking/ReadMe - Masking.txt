//***********************************************************************
//                                             
//    - "Talk to me like I'm a 3 year old!" Programming Lessons -         
//                                                                      
//    $Author:      DigiBen        digiben@gametutorials.com     
//                                        
//    $Program:     Masking                    
//                                            
//    $Description: Demonstrates transparency using masking    
//                                            
//    $Date:        1/29/02                        
//                                            
//***********************************************************************

Files:  Main.cpp   (The Source File containing the most worked with code)
        Init.cpp   (The Source File containing the rarely changed code)
        Main.h     (The header file that holds the global variables and prototypes)
        Makefile   (The makefile, it stores the rules for compiling, linking)

        CrossHair.bmp                Texture 
        floor.bmp                    Texture
        CrossHairMask.bmp            Texture
        Wall.bmp                     Texture 


Libraries:      OpenGL and SDL

Controls:       LEFT / RIGHT Arrow Keys or 'a' / 'd' - This rotates the camera's view in a circle
                UP / DOWN Arrow Keys or 'w' / 's' - This makes our scope zoom in and out
                SPACE BAR - This turns night vision on and off
                F1 - Toggles between fullscreen and windowed mode
                ESCAPE Key - This quits the program


Instructions:    
        After unzipping the files, be in the 'Masking' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './Masking' at the command prompt.

TROUBLESHOOTING INFO:
        1.     If it doesn't compile make sure you have the SDL and OpenGL
            libraries installed on your system.

        2.    The code is amply commented. However, if you still find it
            difficult to follow the SDL code check out the documentation
            (might be installed to /usr/share/doc/SDL-devel-*/)
            The documentation is fantastic and makes SDL look very easy
            and intuitive to learn.

        3.  If you don't have the SDL libraries(developer libraries also ) you
            can download from the official site www.libsdl.org

        4.    If you don't have the OpenGL libraries, download the drivers
            for your video card from the manufacturer's site 
            and developer libraries from www.Mesa3d.org

        5.  If it doesn't run try setting the color depth in the program to
            something else (maybe 32 bit). You will have to change the macro
            in main.h for this.

        6.    If it doesn't go fullscreen try setting a lower desktop
            resolution.

        7.    If you still have some problems with the code
             - for OpenGL stuff mail the author
             - for SDL stuff mail the guys who ported this tutorial to SDL
        
        8.  Make sure that u are using a 24 bit bitmap.


Ben Humphrey
Game Programmer
digiben@gametutorials.com
www.GameTutorials.com

Ported to Linux/SDL by
    gOLeM    <golem@gametutorials.com>    
    insane    <evilsmile@gametutorials.com>    
