# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -g

# Source files
SRCS = main_test.cpp test_pre_process.cpp test_parser.cpp test_deduction.cpp test_syntax.cpp ../src/auxiliary/print.cpp ../src/auxiliary/syntax.cpp ../src/auxiliary/parser.cpp ../src/auxiliary/pre_process.cpp ../src/auxiliary/deduction.cpp ../src/auxiliary/combinatorics.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = main_test

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