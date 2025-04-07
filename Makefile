CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -IC:/FreeImage/include
LDFLAGS = -LC:/FreeImage/lib -lfreeimage

SRC_DIR = src
MODULES_DIR = $(SRC_DIR)/modules
BIN_DIR = bin
OBJ_DIR = $(SRC_DIR)/obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(MODULES_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXEC = $(BIN_DIR)/tucil2_13523160

all: $(EXEC)

$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(MODULES_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(EXEC)
	./$(EXEC)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)