CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -DUNICODE -D_UNICODE -O2

ROOT = alphares
LIB = $(ROOT)/lib
SRC = $(ROOT)/src
INCLUDE = $(LIB)/simpleini
LIBRARY = $(LIB)/simpleini
RESOURCES = $(ROOT)/resources.rc
RES_OBJ = $(ROOT)/resources.o

# Source and Object files
SOURCES = $(SRC)/alphares.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = alphares

# Library
LIBS = -lgdi32 -lmsvcrt
INCLUDES = -I$(INCLUDE)

# First target
all: $(EXECUTABLE) $(RES_OBJ)

# Linking
$(EXECUTABLE): $(OBJECTS) $(RES_OBJ)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(RES_OBJ) -o $(EXECUTABLE) $(LIBS) $(INCLUDES) -mwindows

# Compiling C++ source files
$(SRC)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compiling resources
$(RES_OBJ): $(RESOURCES)
	windres $< -O coff -o $@

# Clean-up
clean:
	rm -f $(SRC)/*.o $(EXECUTABLE).exe $(RES_OBJ)
