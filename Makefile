# Variables
CC = gcc
CFLAGS = -Wall -Iinclude
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
OBJ_DIR = obj
EXEC = $(BIN_DIR)/shell


SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(EXEC)

.PHONY: clean-obj
clean-obj:
	rm -rf $(OBJ_DIR)/*.o
