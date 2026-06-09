# Native (Apple Silicon / Intel) build for GhostBuster.
# Audio uses miniaudio (Audio.cpp) instead of the original x86_64-only FMOD.

CXX      := clang++
GLFW     := $(shell brew --prefix glfw)
GLM      := $(shell brew --prefix glm)

CXXFLAGS := -std=c++14 -O2 -DGL_SILENCE_DEPRECATION \
            -I$(GLFW)/include -I$(GLM)/include -I.
LDFLAGS  := -L$(GLFW)/lib -lglfw \
            -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo \
            -framework CoreFoundation -framework AudioToolbox

# Build every .cpp in this directory.
SRCS := $(wildcard *.cpp)
TARGET := ghostbuster

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
