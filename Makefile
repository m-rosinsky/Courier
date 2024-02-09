# Compiler selection.
CC = g++

# Compiler flags.
CFLAGS = -Wall -Wextra -pedantic -std=c++17

# Dirs.
BINS = bins
OBJS = objs
SRCS = src

# Binary.
OUTPUT_BIN = courier

# Main file.
SRC_MAIN = main.cpp

# Rules.
all: setup compile link

setup:
	@echo "Creating subdirs..."

	@echo "  [+] Creating ./$(BINS)..."
	@mkdir -p $(BINS)

	@echo "  [+] Creating ./$(OBJS)..."
	@mkdir -p $(OBJS)

	@echo "done"

compile: setup
	@echo "Compiling sources..."

	@echo "  [+] Compiling $(OBJS)/error.o"
	@$(CC) $(CFLAGS) -o $(OBJS)/error.o -c $(SRCS)/error.cpp

	@echo "  [+] Compiling $(OBJS)/lexer.o"
	@$(CC) $(CFLAGS) -o $(OBJS)/lexer.o -c $(SRCS)/lexer/lexer.cpp

	@echo "  [+] Compiling $(OBJS)/parser.o"
	@$(CC) $(CFLAGS) -o $(OBJS)/parser.o -c $(SRCS)/parser/parser.cpp

	@echo "done"

link: setup compile
	@echo "Linking binaries..."

	@echo "  [+] Linking $(BINS)/$(OUTPUT_BIN)..."
	@$(CC) $(CFLAGS) -o $(BINS)/$(OUTPUT_BIN) $(SRCS)/$(SRC_MAIN) $(OBJS)/*.o

	@echo "done"

clean:
	@echo "Cleaning..."

	@echo "  [-] Cleaning $(BINS)"
	@$(RM) -rf $(BINS)

	@echo "  [-] Cleaning $(OBJS)"
	@$(RM) -rf $(OBJS)

	@echo "done"
