CC			= g++

TARGET		= Triangle 

$(TARGET):	Main.o 
	$(CC) -o $(TARGET) Main.o `sdl-config --libs` -lGL -lGLU -lm

Main.o:		Main.cpp
	$(CC) -c Main.cpp `sdl-config --cflags`

clean:
	rm -f $(TARGET) Main.o core
