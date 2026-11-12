CXX = clang++
CXXFLAGS = -std=c++17 -I/Users/29glenn/C/raylib/src

LIBS = /Users/29glenn/C/raylib/src/libraylib.a \
       -framework OpenGL \
       -framework Cocoa \
       -framework IOKit \
       -framework CoreVideo \
       -framework QuartzCore

SRC = main.cpp block.cpp chunk.cpp player.cpp wall.cpp follower.cpp
TARGET = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)