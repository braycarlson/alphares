CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -DUNICODE -D_UNICODE -O2

ROOT = alphares
LIB = $(ROOT)/lib
SRC = $(ROOT)/src
INCLUDE = $(ROOT)/include
LIBRARY = $(LIB)/simpleini
RESOURCES = $(ROOT)/resources.rc
RES_OBJ = $(ROOT)/resources.o

# Source and Object files
SOURCES = $(SRC)/alphares.cpp $(SRC)/configuration.cpp $(SRC)/window.cpp $(SRC)/ui.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = alphares

# Library
LIBS = -lgdi32 -luser32 -mwindows
INCLUDES = -I$(INCLUDE) -I$(LIBRARY)

# First target
all: $(EXECUTABLE) $(RES_OBJ)

# Linking
$(EXECUTABLE): $(OBJECTS) $(RES_OBJ)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(RES_OBJ) -o $(EXECUTABLE) $(LIBS) $(INCLUDES)

# Compiling C++ source files
$(SRC)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compiling resources
$(RES_OBJ): $(RESOURCES)
	windres $< -O coff -o $@

# Clean-up
clean:
	rm -f $(SRC)/*.o $(EXECUTABLE).exe $(RES_OBJ)
