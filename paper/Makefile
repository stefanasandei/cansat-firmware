SRC_DIR = src
BUILD_DIR = build
TEX_FILE = CanSat_Lunateeks.tex

# Default target
all: compile

# Target to create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Target to compile the LaTeX document into the build directory
# Run two times to generate cross references for the table of contents
compile: $(BUILD_DIR)
	cd $(SRC_DIR) && pdflatex -output-directory=../$(BUILD_DIR) $(TEX_FILE)
	cd $(SRC_DIR) && pdflatex -output-directory=../$(BUILD_DIR) $(TEX_FILE)

# Clean target to remove generated files
clean:
	rm -rf $(BUILD_DIR)/*

# Phony targets
.PHONY: all compile clean

