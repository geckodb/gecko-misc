VERSION=0.1
CC = clang
CFLAGS = -march=native -Wall -std=c11 -Wextra -Wshadow -O3 -I ./include -D_GNU_SOURCE
LDFLAGS = -shared
OUT_DIR = ./build
SRC_DIR = ./source
EVAL_DIR = ./evaluation

.PHONY: all presentation test evaluation clean

all: directories $(OUT_DIR)/malloc.so

directories: $(OUT_DIR)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)
	
$(OUT_DIR)/malloc.so: $(OUT_DIR)/malloc.o
	$(CC) $(LDFLAGS) -o $@ $(OUT_DIR)/malloc.o

$(OUT_DIR)/malloc.o:
	$(CC) $(CFLAGS) -fPIC -c $(SRC_DIR)/malloc.c -o $(OUT_DIR)/malloc.o

clean:
	rm -rf build;
