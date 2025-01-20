# Architecture (x86 or x64)
ARCHITECTURE ?= x86

# Set architecture-specific flags and windres command
ifeq ($(ARCHITECTURE),x32)
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
LDFLAGS = -lgdi32 -luser32 -mwindows -municode -static -s $(ARCHITECTURE_FLAGS)

ROOT = alphares
BIN = $(ROOT)/bin
OBJ = $(ROOT)/obj
LIB = $(ROOT)/lib
SRC = $(ROOT)/src
INCLUDE = $(ROOT)/include
LIBRARY = $(LIB)/simpleini
RESOURCES = $(ROOT)/resources
RC = $(RESOURCES)/resources.rc
MANIFEST = $(RESOURCES)/alphares.exe.manifest
RES_OBJ = $(OBJ)/resources.o

# Source and object files
SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SOURCES))
EXECUTABLE = alphares

# Library
LIBS = -lgdi32 -luser32 -mwindows
INCLUDES = -I$(INCLUDE) -I$(LIBRARY)

.PHONY: all clean

# Main build target
all: | $(BIN) $(OBJ)
	$(MAKE) $(EXECUTABLE)

# Creating the "bin" and "obj" directories
$(BIN) $(OBJ):
	@if not exist "$@" mkdir "$@"

# Linking
$(EXECUTABLE): $(OBJECTS) $(RES_OBJ) $(MANIFEST)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(RES_OBJ) -o $(BIN)/$(EXECUTABLE)_$(ARCHITECTURE) $(LIBS) $(INCLUDES) $(LDFLAGS) -static -s

# Compiling C++ source files
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compiling resources and embedding the manifest
$(RES_OBJ): $(RC) $(MANIFEST)
	$(WINDRES) --input $< --output $@ --output-format=coff

# Full clean-up
distclean:
	rm -f $(OBJ)/*.o $(BIN)/$(EXECUTABLE)_x86.exe $(BIN)/$(EXECUTABLE)_x64.exe $(RES_OBJ)

# Clean-up
clean:
	rm -f $(OBJ)/*.o $(RES_OBJ)
