# Architecture (32 or 64)
ARCHITECTURE ?= 64

# Set architecture-specific flags and windres command
ifeq ($(ARCHITECTURE),32)
	MINGW32 = C:/msys64/mingw32/bin

    CXX = $(MINGW32)/g++
    ARCHITECTURE_FLAGS = -m32
    WINDRES = $(MINGW32)/windres --target=pe-i386
else
	MINGW64 = C:/msys64/mingw64/bin

    CXX = $(MINGW64)/g++
    ARCHITECTURE_FLAGS = -m64
    WINDRES = $(MINGW64)/windres --target=x86_64-w64-mingw32
endif

CXXFLAGS = -Wall -Wextra -std=c++20 -DUNICODE -D_UNICODE -O2 $(ARCHITECTURE_FLAGS)
LDFLAGS = $(ARCHITECTURE_FLAGS)

ROOT = alphares
BIN = $(ROOT)/bin
LIB = $(ROOT)/lib
SRC = $(ROOT)/src
INCLUDE = $(ROOT)/include
LIBRARY = $(LIB)/simpleini
RESOURCES = $(ROOT)/resources
RC = $(RESOURCES)/resources.rc
RES_OBJ = $(RESOURCES)/resources.o

# Source and object files
SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = alphares

# Library
LIBS = -lgdi32 -luser32 -mwindows
INCLUDES = -I$(INCLUDE) -I$(LIBRARY)

.PHONY: all clean

# Main build target
all: | $(BIN)
	$(MAKE) $(EXECUTABLE)

# Creating the "bin" directory
$(BIN):
	mkdir -p $(BIN)

# Linking
$(EXECUTABLE): $(OBJECTS) $(RES_OBJ)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(RES_OBJ) -o $(BIN)/$(EXECUTABLE)_$(ARCHITECTURE) $(LIBS) $(INCLUDES) $(LDFLAGS) -static -s

# Compiling C++ source files
$(SRC)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compiling resources
$(RES_OBJ): $(RC)
	$(WINDRES) $< -O coff -o $@

# Full clean-up
distclean:
	rm -f $(SRC)/*.o $(BIN)/$(EXECUTABLE)_32.exe $(BIN)/$(EXECUTABLE)_64.exe $(RES_OBJ)

# Clean-up
clean:
	rm -f $(SRC)/*.o $(RES_OBJ)

