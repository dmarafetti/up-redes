CC = gcc
CFLAGS = -Wall
LDFLAGS = -lpthread

# Source files
SRC = fork.c
LSOF_SRC = fork_execv_lsof.c

# Output directory and binaries
OUT_DIR = dist
OUT_BIN = $(OUT_DIR)/fork
OUT_LSOF = $(OUT_DIR)/fork_execv_lsof

# Target to build all binaries
all: $(OUT_BIN) $(OUT_LSOF)

# Rule to compile the main fork demo
$(OUT_BIN): $(SRC)
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(OUT_BIN) $(SRC) $(LDFLAGS)

# Rule to compile fork+execv lsof test (no pthread)
$(OUT_LSOF): $(LSOF_SRC)
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(OUT_LSOF) $(LSOF_SRC)

# Clean the build artifacts
clean:
	rm -rf $(OUT_DIR)

.PHONY: all clean
