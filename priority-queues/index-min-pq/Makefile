# AI Generated
# Compiler and Flags
CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O3

# Target executable name
TARGET   = test_index_min_pq

# Source files
SRC      = test_index_min_pq.cpp
# Headers (to track changes for recompilation)
DEPS     = index_min_pq.hpp

# Default rule
all: $(TARGET)

# Link and Compile
$(TARGET): $(SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Helper to run the program
run: all
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(TARGET)

# Phony targets (not actual files)
.PHONY: all run clean