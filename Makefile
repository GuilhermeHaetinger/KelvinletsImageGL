CC=g++
MAIN=./src/main.cpp

run:
	$(CC) -o kelvin $(MAIN) -lpthread -lX11 -std=c++11 -lglfw -lGLEW -lglut -lGL
	
clean:
	rm -f *.out *.o kelvin
