CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c23 -D_POSIX_C_SOURCE=200809L -g
INC = -I include
SRC = src/
BUILD_DIR = build
EXEC = $(BUILD_DIR)/main

SRCS = $(wildcard $(SRC)*.c)
OBJS = $(patsubst $(SRC)%.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

all: $(EXEC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXEC): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC)%.c | $(BUILD_DIR)
	$(CC) $(INC) $(CFLAGS) -MMD -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(EXEC)
	./$(EXEC) $(ARGS)

-include $(DEPS)

.PHONY: all clean
