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

DEBUG_BUILD_DIR = $(BUILD_DIR)/debug
DEBUG_EXEC = $(DEBUG_BUILD_DIR)/main
DEBUG_OBJS = $(patsubst $(SRC_DIR)/%.c, $(DEBUG_BUILD_DIR)/%.o, $(SRCS))

debug: CC = gcc
debug: CFLAGS += -Og -DDEBUG
debug: clean_debug $(DEBUG_EXEC)

$(DEBUG_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(INC) $(CFLAGS) -MMD -c $< -o $@

$(DEBUG_EXEC): $(DEBUG_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

clean_debug:
	rm -rf $(DEBUG_BUILD_DIR)


EMBED_BUILD_DIR = $(BUILD_DIR)/tcc
EMBED_EXEC = $(EMBED_BUILD_DIR)/main

EMBED_CFLAGS = -Wall -Wextra -std=c23 -g -D__TCC__

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

EMBED_DEBUG_BUILD_DIR = $(EMBED_BUILD_DIR)/debug
EMBED_DEBUG_EXEC = $(EMBED_DEBUG_BUILD_DIR)/main
EMBED_DEBUG_OBJS = $(patsubst $(SRC_DIR)/%.c, $(EMBED_DEBUG_BUILD_DIR)/%.o, $(SRCS))

embed_debug: CC = tcc
embed_debug: EMBED_CFLAGS += -Og -DDEBUG
embed_debug: clean_embed_debug $(EMBED_DEBUG_EXEC)

$(EMBED_DEBUG_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(INC) $(EMBED_CFLAGS) -c $< -o $@

$(EMBED_DEBUG_EXEC): $(EMBED_DEBUG_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(EMBED_CFLAGS) $^ -o $@

clean_embed_debug:
	rm -rf $(EMBED_DEBUG_BUILD_DIR)
