# Makefile for compiling hamming_calculator.cpp

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# Executable name
TARGET = hamming_calculator

# Source file
SRCS = hamming_calculator.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to link object files into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Rule to clean generated files
clean:
	rm -f $(OBJS) $(TARGET)