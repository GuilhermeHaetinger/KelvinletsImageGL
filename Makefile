CC=g++
MAIN=./src/main.cpp

run:
	$(CC) -o OpenGLWindow $(MAIN) -lglfw -lGLEW -lglut -lGL
	
clean:
	rm -f *.out *.o
