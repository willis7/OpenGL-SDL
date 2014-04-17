//*******************************************************************************//
//                                          
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      
//                                                                       
//      $Author:    TheTutor    thetutor@gametutorials.com      
//                                          
//      $Program:   Particle
//                                      
//      $Description:   Shows you one way to do a "particle"
//                                          
//      $Date:      11/2/01                         
//                                          
//*******************************************************************************//



Files:  main.cpp        (The Source File containing "window" code)
        OGL.cpp         (The Source File containing the "OpenGL" code)
        particle.cpp    (The Source File containing the "particle" code)
        OGL.h           (A Header File that holds prototypes)
        particle.h      (A Header File that holds prototypes)
        P1.bmp, P2.bmp  (The "particle" bitmaps)

Libraries:      OpenGL and SDL

Controls :      F1      - This toggles between fullscreen and windowed mode
                Escape  - Quits the application.


Instructions:   
        After unzipping the files, be in the 'Particle' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './Particle' at the command prompt.

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


TheTutor
thetutor@gametutorials.com
www.GameTutorials.com

Ported to Linux/SDL by
    gOLeM   <golem@gametutorials.com>
    insane  <evilsmile@gametutorials.com>
