CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c23 -D_GNU_SOURCE -g
INC = -I include
SRC_DIR = src
BUILD_DIR = build
EXEC = $(BUILD_DIR)/main

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

.PHONY: all clean run embed embed_clean embed_run

all: $(EXEC)

$(EXEC): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(INC) $(CFLAGS) -MMD -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(EXEC)
	./$(EXEC) $(ARGS)

EMBED_BUILD_DIR = $(BUILD_DIR)/tcc
EMBED_EXEC = $(EMBED_BUILD_DIR)/main

EMBED_CFLAGS = -Wall -Wextra -std=c23 -g

EMBED_OBJS = $(patsubst $(SRC_DIR)/%.c, $(EMBED_BUILD_DIR)/%.o, $(SRCS))

embed: CC = tcc
embed: CFLAGS = $(EMBED_CFLAGS)
embed: $(EMBED_EXEC)

$(EMBED_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

$(EMBED_EXEC): $(EMBED_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

embed_clean:
	rm -rf $(EMBED_BUILD_DIR)

embed_run: embed
	./$(EMBED_EXEC) $(ARGS)
