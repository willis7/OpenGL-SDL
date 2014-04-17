CC			= g++ 

TARGET		= BspLoader3

$(TARGET):	Main.o Init.o Camera.o Quake3Bsp.o Image.o Frustum.o
	$(CC) -o $(TARGET) Main.o Init.o Camera.o Quake3Bsp.o Image.o Frustum.o `sdl-config --libs` -lGL -lGLU -lm -ljpeg

Main.o:		Main.cpp main.h Camera.h
	$(CC) -c `sdl-config --cflags` Main.cpp

Init.o:		Init.cpp main.h image.h
	$(CC) -c `sdl-config --cflags` Init.cpp

Camera.o :  Camera.cpp main.h Camera.h
	$(CC) -c `sdl-config --cflags` Camera.cpp

Quake3Bsp.o :  Quake3Bsp.cpp main.h Camera.h
	$(CC) -c `sdl-config --cflags` Quake3Bsp.cpp

Image.o :   Image.cpp image.h
	$(CC) -c `sdl-config --cflags` Image.cpp
	
Frustum.o   :   Frustum.cpp Frustum.h 
	$(CC) -c `sdl-config --cflags` Frustum.cpp

clean:
	rm -f $(TARGET) Main.o Init.o Camera.o Quake3Bsp.o Image.o core Frustum.o
