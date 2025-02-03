# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -g

# Source files
SRCS = main.cpp auxiliary/print.cpp auxiliary/syntax.cpp auxiliary/deduction.cpp auxiliary/parser.cpp auxiliary/pre_process.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = main

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation of source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean objects and executable
clean:
	rm -f $(OBJS) $(TARGET)