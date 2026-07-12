CC = gcc

LLVM_CFLAGS := $(shell llvm-config --cflags)
LLVM_LDFLAGS := $(shell llvm-config --ldflags --libs core)

CFLAGS = $(LLVM_CFLAGS) -std=c99 -Wall -Wextra -Wpedantic -g -MMD -MP
LDFLAGS = $(LLVM_LDFLAGS)

ifeq ($(SANITIZE),1)
	CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
	LDFLAGS += -fsanitize=address,undefined
endif

TARGET = bin/app

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)
DEP = $(OBJ:.o=.d)

.PHONY: all clean run sanitize

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) $(LDFLAGS) -o $@

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEP)

run: $(TARGET)
	./$(TARGET)

sanitize:
	$(MAKE) SANITIZE=1

clean:
	rm -rf obj bin