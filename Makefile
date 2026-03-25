SRC_DIR = src
INC_DIR = include
EXT_DIR = external
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/simulation

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic \
           -I$(INC_DIR) \
           -isystem $(EXT_DIR)/imgui \
           -isystem $(EXT_DIR)/rlimgui
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC_FILES   = $(wildcard $(SRC_DIR)/*.cpp)
IMGUI_SRC   = $(wildcard $(EXT_DIR)/imgui/*.cpp)
RLIMGUI_SRC = $(wildcard $(EXT_DIR)/rlimgui/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES)) \
            $(patsubst $(EXT_DIR)/imgui/%.cpp, $(BUILD_DIR)/%.o, $(IMGUI_SRC)) \
            $(patsubst $(EXT_DIR)/rlimgui/%.cpp, $(BUILD_DIR)/%.o, $(RLIMGUI_SRC))

all: $(TARGET)

$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(EXT_DIR)/imgui/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -w -c $< -o $@

$(BUILD_DIR)/%.o: $(EXT_DIR)/rlimgui/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -w -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	@mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
