# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Detect platform
ifeq ($(OS),Windows_NT)
    # Windows-specific settings
    CXXFLAGS += -IC:/FreeImage/include
    LDFLAGS = -LC:/FreeImage/lib -lfreeimage
else
    # Linux-specific settings
    CXXFLAGS += -I/usr/include
    LDFLAGS = -L/usr/lib -lfreeimage -lstdc++
endif

# Directories
SRC_DIR = src
MODULES_DIR = $(SRC_DIR)/modules
BIN_DIR = bin
OBJ_DIR = $(SRC_DIR)/obj

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(MODULES_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXEC = $(BIN_DIR)/tucil2_13523160

# Targets
all: $(EXEC)

$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXEC)