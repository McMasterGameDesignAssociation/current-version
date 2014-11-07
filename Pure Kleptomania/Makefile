# This Makefile must be put in the "Pure Kleptomania" folder

# The exe crashes if it's in the same dir as 'Assets'.
# For now, the exe will be moved to ./bin after compiling

#SRC := $(wildcard Pure_Kleptomania/*.cpp)
#SRC = $(shell find './Pure Kleptomania/' -maxdepth 1 | grep .cpp | sed 's/ /\\ /g')
SRC = $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
EXE = purekleptomania

CXX = g++
CXXFLAGS = -Wall -std=c++11 -O2 -g -I../FreeGlut/include -I../LibPng/Include
LDFLAGS += -L../FreeGlut/libs/x86 -L../LibPng/Lib/Release
LDLIBS = -lGL -lGLU -lglut -lm -llibpng16 -lzlib
EXT = 

ifeq ($(OS), Windows_NT)
	EXT = .exe
	LDLIBS = -lfreeglut -lglu32 -lopengl32 -llibpng16 -lzlib
endif

# OS X
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	LDLIBS = -framework Carbon -framework OpenGL -framework GLUT -lm -llibpng16 -lzlib
endif

$(EXE)$(EXT): $(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	mkdir -p ./bin/
	mv $(EXE)$(EXT) ./bin/
    
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

run:
	./$(EXE)$(EXT)

clean:
	rm ./$(OBJ) ./$(EXE)$(EXT)