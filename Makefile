# Compiler
CXX := g++

# Flag for compilation
CFLAGS = -Wno-implicit-function-declaration -std=c++11

# Target executable
TARGET := output

# Directories
BUILDDIR := build

# Source files
SRCFILES := $(wildcard src/*.cpp)

# Object files (generated in the build directory)
OBJFILES := $(patsubst src/%.cpp, $(BUILDDIR)/%.o, $(SRCFILES))

all: $(TARGET)

$(TARGET): $(OBJFILES)
		$(CXX) $(CFLAGS) -o $@ $^

# Compiling source files into object files
$(BUILDDIR)/%.o: src/%.cpp src/%.hpp
	@mkdir $(BUILDDIR)
	$(CXX) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILDDIR) $(TARGET)

# Declare phony targets (in case of file with same name)
.PHONY: clean
