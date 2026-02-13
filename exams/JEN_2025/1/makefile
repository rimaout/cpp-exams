#EXEXUTION_PARAMETERS=

# 2. Compiler Settings
# --------------------
CC = g++
CFLAGS = -O3 -std=c++17 -I../../../my_libs
DFLAGS = -g -Wall -Wextra

# 3. Directory Variables
# ----------------------
SRC_DIR = src
BUILD_DIR = .build
BIN_DIR = .bin

# Final executable names
TARGET = $(BIN_DIR)/exam_executable

# 4. File Discovery
# -----------------
# Find source files for each module
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

# Create corresponding object lists (.cpp -> .o)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# 5. Build Rules
# --------------

all: directories $(TARGET)

debug: CFLAGS += $(DFLAGS)	# Add debug flags to CFLAGS for the debug target
debug: all
# Usage:
#  - use `make` to build the project
#  - use `make debug` to build with debug flags

# Create output directory structure
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/core
	@mkdir -p $(BUILD_DIR)/utils
	@mkdir -p $(BIN_DIR)

# Rule: Link the executable
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	@$(CC) $(OBJS) -o $(TARGET)
	@echo "✓ Build successful! Run with: ./$(TARGET)"
 
# Rule: Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# 6. Utilities
# ------------

clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR) 
	@echo "✓ Clean complete."

run: $(TARGET)
	./$(TARGET) $(EXEXUTION_PARAMETERS)

fresh: all run

# ==========================================
# .PHONY Protection
# ==========================================
# This tells Make that 'all', 'clean', and 'run' are abstract COMMANDS, not files.
# It prevents the build from breaking if you accidentally create a file named "clean" or "run".
.PHONY: all debug directories clean run fresh
