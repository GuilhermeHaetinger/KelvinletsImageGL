CC=g++
SRC=./src/
BIN=./bin/
MAIN=$(SRC)main.cpp
MAIN_OBJECT=$(BIN)main.o
KELVIN=./kelvin
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
OPENCV_FLAGS=-lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab -lopencv_videoio -O2 -m64
LIB_PATH="-L/usr/local/lib/"

$(KELVIN): $(SHADER_COMPILER_OBJECT) $(IMAGE_OBJECT) $(KELVINLETS_OBJECT) $(RENDERABLE_IMAGE_OBJECT) $(GL_ABSTRACTION_OBJECT) $(MAIN_OBJECT)
	$(CC) $(LIB_PATH) -o kelvin $(MAIN_OBJECT) $(SHADER_COMPILER_OBJECT) $(IMAGE_OBJECT) $(RENDERABLE_IMAGE_OBJECT) $(GL_ABSTRACTION_OBJECT) $(KELVINLETS_OBJECT) $(FLAGS) $(OPENCV_FLAGS)

$(MAIN_OBJECT): $(MAIN)
	$(CC) -o $(MAIN_OBJECT) -c $(MAIN) $(FLAGS) $(OPENCV_FLAGS)

$(SHADER_COMPILER_OBJECT): $(SHADER_COMPILER)
	$(CC) -o $(SHADER_COMPILER_OBJECT) -c $(SHADER_COMPILER) $(FLAGS) $(OPENCV_FLAGS)

$(KELVINLETS_OBJECT): $(KELVINLETS)
	$(CC) -o $(KELVINLETS_OBJECT) -c $(KELVINLETS) $(FLAGS) $(OPENCV_FLAGS)

$(IMAGE_OBJECT): $(IMAGE)
	$(CC) -o $(IMAGE_OBJECT) -c $(IMAGE) $(FLAGS) $(OPENCV_FLAGS)

$(RENDERABLE_IMAGE_OBJECT): $(RENDERABLE_IMAGE)
	$(CC) -o $(RENDERABLE_IMAGE_OBJECT) -c $(RENDERABLE_IMAGE) $(FLAGS) $(OPENCV_FLAGS)

$(GL_ABSTRACTION_OBJECT): $(GL_ABSTRACTION)
	$(CC) -o $(GL_ABSTRACTION_OBJECT) -c $(GL_ABSTRACTION) $(FLAGS) $(OPENCV_FLAGS)

clean:
	rm -f $(BIN)* */.#* kelvin

test: kelvin
	./kelvin ./testImages/mandrill.png 0.4 1.0
