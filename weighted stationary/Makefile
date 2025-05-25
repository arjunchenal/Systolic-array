# # Compiler
# CC = g++

# # SystemC paths (adjust to your installation)
# SYSTEMC_INCLUDE = C:/SystemC/include
# SYSTEMC_LIB = C:/SystemC/lib

# # Flags
# CFLAGS = -std=c++17 -g -I$(SYSTEMC_INCLUDE) -Iinclude -Isrc -Itb
# LDFLAGS = -L$(SYSTEMC_LIB) -lsystemc -lm

# # Directories
# SRC_DIR = src
# INCLUDE_DIR = include
# TB_DIR = testbench
# BUILD_DIR = build

# # Executable outputs
# OUTPUT_EXEC = $(BUILD_DIR)/tb_outputstationary.exe
# WEIGHTED_EXEC = $(BUILD_DIR)/tb_weightedstationary.exe

# # Common source files
# COMMON_SOURCES = $(SRC_DIR)/alu.cpp $(SRC_DIR)/accumulator.cpp

# # Specific sources per testbench
# OUTPUT_SOURCES = $(TB_DIR)/output/testbench_output_stationary.cpp $(SRC_DIR)/macunit_outputstationary.cpp $(COMMON_SOURCES)
# WEIGHTED_SOURCES = $(TB_DIR)/weighted/testbench_weighted_stationary.cpp $(SRC_DIR)/sram.cpp $(COMMON_SOURCES)

# # Default target: build everything
# all: $(OUTPUT_EXEC) $(WEIGHTED_EXEC)

# # Output-stationary executable
# $(OUTPUT_EXEC): $(OUTPUT_SOURCES) | $(BUILD_DIR)
# 	@echo "Building output-stationary testbench..."
# 	$(CC) $(CFLAGS) $(OUTPUT_SOURCES) -o $(OUTPUT_EXEC) $(LDFLAGS)

# # Weighted-stationary executable
# $(WEIGHTED_EXEC): $(WEIGHTED_SOURCES) | $(BUILD_DIR)
# 	@echo "Building weighted-stationary testbench..."
# 	$(CC) $(CFLAGS) $(WEIGHTED_SOURCES) -o $(WEIGHTED_EXEC) $(LDFLAGS)

# # Create build directory if it doesn't exist
# $(BUILD_DIR):
# 	-mkdir $(BUILD_DIR)

# # Run output-stationary simulation and print to terminal
# run_output: $(OUTPUT_EXEC)
# 	@echo "Running output-stationary simulation..."
# 	@./$(OUTPUT_EXEC)

# # Run weighted-stationary simulation and print to terminal
# run_weighted: $(WEIGHTED_EXEC)
# 	@echo "Running weighted-stationary simulation..."
# 	@./$(WEIGHTED_EXEC)

# # Clean everything
# clean:
# 	@echo "Cleaning up..."
# 	-rm -rf $(BUILD_DIR) *.vcd

# # Phony targets
# .PHONY: all clean run_output run_weighted





































































# Compiler
CC = g++

# SystemC paths (adjust to your installation)
SYSTEMC_INCLUDE = C:/SystemC/include
SYSTEMC_LIB = C:/SystemC/lib

# Flags
CFLAGS = -std=c++17 -g -I$(SYSTEMC_INCLUDE) -Iinclude -Isrc -Itb
LDFLAGS = -L$(SYSTEMC_LIB) -lsystemc -lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TB_DIR = testbench
BUILD_DIR = build

# Executable outputs
OUTPUT_EXEC = $(BUILD_DIR)/tb_outputstationary.exe
WEIGHTED_EXEC = $(BUILD_DIR)/tb_weightedstationary.exe

# Common source files
COMMON_SOURCES = $(SRC_DIR)/alu.cpp $(SRC_DIR)/accumulator.cpp

# Specific sources per testbench
OUTPUT_SOURCES = $(TB_DIR)/output/testbench_output_stationary.cpp $(SRC_DIR)/macunit_outputstationary.cpp $(COMMON_SOURCES)
WEIGHTED_SOURCES = $(TB_DIR)/weighted/testbench_weighted_stationary.cpp $(SRC_DIR)/sram.cpp $(COMMON_SOURCES)

# Default target: build everything
all: $(OUTPUT_EXEC) $(WEIGHTED_EXEC)

# Output-stationary executable
$(OUTPUT_EXEC): $(OUTPUT_SOURCES) | $(BUILD_DIR)
	@echo "Building output-stationary testbench..."
	$(CC) $(CFLAGS) $(OUTPUT_SOURCES) -o $(OUTPUT_EXEC) $(LDFLAGS)

# Weighted-stationary executable
$(WEIGHTED_EXEC): $(WEIGHTED_SOURCES) | $(BUILD_DIR)
	@echo "Building weighted-stationary testbench..."
	$(CC) $(CFLAGS) $(WEIGHTED_SOURCES) -o $(WEIGHTED_EXEC) $(LDFLAGS)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	-mkdir $(BUILD_DIR)

# Run output-stationary simulation and save to log
run_output: $(OUTPUT_EXEC)
	@echo "Running output-stationary simulation..."
	@./$(OUTPUT_EXEC) > logs/output_stationary.log 2>&1
	@echo "Simulation output saved to logs/output_stationary.log"

# Run weighted-stationary simulation and save to log
run_weighted: $(WEIGHTED_EXEC)
	@echo "Running weighted-stationary simulation..."
	@./$(WEIGHTED_EXEC) > logs/weighted_stationary.log 2>&1
	@echo "Simulation output saved to logs/weighted_stationary.log"


# Clean everything
clean:
	@echo "Cleaning up..."
	-rm -rf $(BUILD_DIR) *.vcd

# Phony targets
.PHONY: all clean run_output run_weighted





