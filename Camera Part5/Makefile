CC			= g++

TARGET		= Camera5

$(TARGET):	Main.o Init.o Camera.o
	$(CC) -o $(TARGET) Main.o Init.o Camera.o `sdl-config --libs` -lGL -lGLU -lm

Main.o:		Main.cpp main.h
	$(CC) -c Main.cpp `sdl-config --cflags`

Init.o:		In .cpp main.h
	$(CC) -c Init.cpp `sdl-config --cflags`

Camera.o:	Camera.cpp main.h
	${CC} -c Camera.cpp `sdl-config --cflags`

clean:
	rm -f $(TARGET) Main.o Init.o Camera.o core
