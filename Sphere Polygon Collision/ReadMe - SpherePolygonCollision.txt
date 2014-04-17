//***********************************************************************
//                                                                               
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -              
//                                                                      
//              $Author:        DigiBen         DigiBen@GameTutorials.com                
//                                                               
//              $Program:       SpherePolygonCollision                           
//                                                                                      
//              $Description:   This demonstrates sphere and polygon collision   
//                                                                       
//              $Date:          1/23/02
//                                                                                      
//***********************************************************************


Files:      Main.cpp   (The Source File containing the most worked with code)
            3DMath.cpp (The Source File containing the reusable math code)
            3DMath.h   (Header File that holds the 3DMath prototypes and VECTOR3D struct)
            Init.cpp   (The Source File containing the rarely changed code)
            main.h     (The header file that holds the global variables and prototypes)

Libraries:  opengl and SDL 

Instructions:
        After unzipping the files, be in the 'SpherePolygonCollision' directory and for

        Compiling:
            1. Just type 'make' at the command prompt.
        Running:
            1. Just type './SpherePolygonCollision' at the command prompt.
            2. To quit hit the ESC key     .
            3. Press F5 to toggle between fullscreen and windowed mode
                4. LEFT ARROW - Moves the sphere left along it's current plane
                5. RIGHT ARROW - Moves the sphere right along it's current plane
                6. UP ARROW - Moves the sphere up along it's current plane
                7. DOWN ARROW - Moves the sphere down along it's current plane
                8. F3 - Moves the sphere towards the front of the triangle's plane
                9. F4 - Moves the sphere towards the back of the triangle's plane
               10. F1 - Rotates the camera left
               11. F2 - Rotates the camera right


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
