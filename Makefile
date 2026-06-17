# ==============================================================================
# 1. Operating System Detection & Compatibility Layer
# ==============================================================================

ifeq ($(OS),Windows_NT)
    # Force use of Windows CMD to avoid Bash path/command conflicts
    SHELL      := cmd.exe
    TARGET_EXT := .exe
    # Convert forward slashes to backslashes for Windows CMD internal commands
    FIX_PATH    = $(subst /,\,$1)
    # Windows CMD syntax for creating directories safely
    MKDIR       = if not exist $(call FIX_PATH,$1) mkdir $(call FIX_PATH,$1)
    # Windows CMD syntax for removing directories recursively and quietly
    RMDIR       = if exist $(call FIX_PATH,$1) rmdir /s /q $(call FIX_PATH,$1)
else
    # Linux / macOS Settings
    TARGET_EXT :=
    FIX_PATH    = $1
    MKDIR       = mkdir -p $1
    RMDIR       = rm -rf $1
endif

# ==============================================================================
# 2. Compiler Configuration
# ==============================================================================

CXX      := g++
CXXFLAGS := -std=c++20 -O3 -Wall -Wextra -pedantic

# Directories
SRC_DIR  := src
INC_DIR  := include
EXT_DIR  := external/pugixml
OBJ_DIR  := obj
BIN_DIR  := bin

INCLUDES := -I$(INC_DIR) -I$(EXT_DIR)
TARGET   := $(BIN_DIR)/YerevanNav$(TARGET_EXT)

# ==============================================================================
# 3. Source and Object Files Tracking
# ==============================================================================

SOURCES  := $(SRC_DIR)/main.cpp \
            $(SRC_DIR)/MapGraph.cpp \
            $(EXT_DIR)/pugixml.cpp

OBJECTS  := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

# ==============================================================================
# 4. Build Rules
# ==============================================================================

all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJECTS)
	@$(call MKDIR,$(BIN_DIR))
	@echo Linking executable: $@
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@
	@echo Build successful!

# Rule to compile src/*.cpp into obj/src/*.o
$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(call MKDIR,$(dir $@))
	@echo Compiling: $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to compile external/pugixml/%.cpp into obj/external/pugixml/%.o
$(OBJ_DIR)/$(EXT_DIR)/%.o: $(EXT_DIR)/%.cpp
	@$(call MKDIR,$(dir $@))
	@echo Compiling third-party: $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo Cleaning project...
	@$(call RMDIR,$(OBJ_DIR))
	@$(call RMDIR,$(BIN_DIR))
	@echo Clean complete.

.PHONY: all clean