# AI generated

# index-min-pq/Makefile
CXX      = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -arch arm64
TARGET   = test_index_min_pq
SRC      = test_index_min_pq.cpp
DEPS     = index_min_pq.hpp

all: $(TARGET)

$(TARGET): $(SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean