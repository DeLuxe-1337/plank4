CC = gcc

LLVM_CFLAGS := $(shell llvm-config --cflags)
LLVM_LDFLAGS := $(shell llvm-config --ldflags --libs core)

CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g $(LLVM_CFLAGS)
CPPFLAGS = -Iinclude

TARGET = bin/app

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) $(LLVM_LDFLAGS) -o $@

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf obj bin