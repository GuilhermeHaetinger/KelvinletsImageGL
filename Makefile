CC=g++
SRC=./src/
BIN=./bin/
MAIN=$(SRC)main.cpp
MAIN_OBJECT=$(BIN)main.o
SHADER_COMPILER=$(SRC)ShaderCompiler.cpp
SHADER_COMPILER_OBJECT=$(BIN)ShaderCompiler.o
IMAGE=$(SRC)Image.cpp
IMAGE_OBJECT=$(BIN)Image.o
RENDERABLE_IMAGE=$(SRC)RenderableImage.cpp
RENDERABLE_IMAGE_OBJECT=$(BIN)RenderableImage.o
GL_ABSTRACTION=$(SRC)glImageAbstraction.cpp
GL_ABSTRACTION_OBJECT=$(BIN)glImageAbstraction.o
KELVINLETS=$(SRC)KelvinletsObject.cpp
KELVINLETS_OBJECT=$(BIN)KelvinletsObject.o



FLAGS=-lpthread -lX11 -std=c++11 -lglfw -lGLEW -lglut -lGL

run: ShaderCompiler.o Image.o KelvinletsObject.o RenderableImage.o glImageAbstraction.o main.o
	$(CC) -o kelvin $(MAIN_OBJECT) $(SHADER_COMPILER_OBJECT) $(IMAGE_OBJECT) $(RENDERABLE_IMAGE_OBJECT) $(GL_ABSTRACTION_OBJECT) $(KELVINLETS_OBJECT) $(FLAGS)

main.o:
	$(CC) -o $(MAIN_OBJECT) -c $(MAIN) $(FLAGS)

ShaderCompiler.o:
	$(CC) -o $(SHADER_COMPILER_OBJECT) -c $(SHADER_COMPILER) $(FLAGS)

KelvinletsObject.o:
	$(CC) -o $(KELVINLETS_OBJECT) -c $(KELVINLETS) $(FLAGS)

Image.o:
	$(CC) -o $(IMAGE_OBJECT) -c $(IMAGE) $(FLAGS)

RenderableImage.o:
	$(CC) -o $(RENDERABLE_IMAGE_OBJECT) -c $(RENDERABLE_IMAGE) $(FLAGS)

glImageAbstraction.o:
	$(CC) -o $(GL_ABSTRACTION_OBJECT) -c $(GL_ABSTRACTION) $(FLAGS)

clean:
	rm -f $(BIN)* */.#* kelvin

test: run
	./kelvin ./testImages/mandrill.png 0.4 1.0
