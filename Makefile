# Compiler
CXX := g++

# Flag for compilation
CFLAGS = -std=c++11

# Target executable
TARGET := output

SRC_DIR := src
BUILD_DIR := build
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))


$(TARGET): $(OBJ_FILES)
	$(CXX) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR/%.hpp)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) -c -o $@ $<

# Clean up
clean:
	rm -rf $(BUILDDIR) $(TARGET)

# Declare phony targets (in case of file with same name)
.PHONY: clean
