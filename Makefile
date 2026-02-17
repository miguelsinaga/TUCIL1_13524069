CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/queens_solver

SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/QueensSolver.cpp $(SRC_DIR)/BoardValidator.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build completed: $(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
