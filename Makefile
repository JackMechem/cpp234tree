CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17
BUILD    := build
TARGET   := $(BUILD)/rbtree
SRC      := main.cpp
OBJ      := $(BUILD)/main.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD)/%.o: %.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)

run: $(TARGET)
	./$(TARGET)

