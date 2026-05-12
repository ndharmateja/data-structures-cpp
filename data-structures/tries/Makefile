# AI Generated
# Compiler and Flags
CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O3

# Target executable name
TARGET   = test_trie

# Source files
SRC      = test_trie.cpp
# Headers (to track changes for recompilation)
DEPS     = trie.hpp

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