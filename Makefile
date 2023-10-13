CC = gcc
CFLAGS = -Wall -g -Iinclude
ENDFLAGS = -lcunit -o
HEADER_FILES := $(wildcard *.h)
SRC_FILES := $(wildcard *.c)
BIN_DIR := bin

LIB_DIR := lib

LIBRARY := $(LIB_DIR)/libhash.a

OBJ_FILES := $(patsubst %.c, $(BIN_DIR)/%.o, $(SRC_FILES))

all: $(LIBRARY)

tests: $(BIN_DIR)/tests

$(BIN_DIR)/tests: $(OBJ_FILES)
	${CC} ${CFLAGS} $^ ${ENDFLAGS} $@

$(LIBRARY): $(OBJ_FILES)
	ar rcs $(LIBRARY) $^

$(BIN_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ_FILES)
