# Static linking of libpng for linux.
STATIC_LINK = true

BIN_DIR = bin
LINUX_DIR = ../Linux
WINDOWS_DIR = ..
EXE = $(BIN_DIR)/purekleptomania
CXX = g++
CXXFLAGS = -Wall -std=c++11 -O2 -g

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
OBJ := $(addprefix ./$(BIN_DIR)/,$(OBJ))

ifeq ($(shell uname), Linux)
	CXXFLAGS += -I$(LINUX_DIR)/include -I$(LINUX_DIR)/include/GL
	LDFLAGS += -L$(LINUX_DIR)/lib
	LDLIBS = -lm -LGL -lGLU -lglut
	ifeq ($(STATIC_LINK), true)
		LDLIBS += $(LINUX_DIR)/lib/libpng.a $(LINUX_DIR)/lib/libz.a
	else
		LDLIBS += -lpng -lz
	endif
	EXT =
endif

ifeq ($(OS), Windows_NT)
	CXXFLAGS += -I$(WINDOWS_DIR)/FreeGlut/include -I$(WINDOWS_DIR)/LibPng/Include
	LDFLAGS += -L$(WINDOWS_DIR)/FreeGlut/libs/x86 -L$(WINDOWS_DIR)/LibPng/Lib/Debug
	LDLIBS = -lfreeglut -lglu32 -lopengl32 -llibpng16 -lzlib
	EXT = .exe
endif

ifeq ($(shell uname), Darwin)
	LDLIBS = -framework Carbon -framework OpenGL -framework GLUT -lm
	LDLIBS += -llibpng16 -lzlib
	#LDLIBS += $(LINUX_DIR)/lib/libpng.a $(LINUX_DIR)/lib/libz.a
endif

$(EXE)$(EXT): $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BIN_DIR)/%.o : %.cpp
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	./$(EXE)$(EXT)

clean:
	rm ./$(OBJ) ./$(EXE)$(EXT)

#SRC := $(wildcard Pure\\ Kleptomania/*.cpp)
#SRC := $(shell find './Pure Kleptomania/' -maxdepth 1 | grep .cpp | sed 's/ /\\ /g')
