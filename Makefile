CC = gcc
CFLAGS = -Wall -pg -g -Iinclude
ENDFLAGS = -lcunit -lm -o
HEADER_FILES := $(wildcard include/*.h)
SRC_FILES := $(wildcard *.c)

TEST_DIR := tests

BIN_DIR := bin

LIB_DIR := lib

LIBRARY := $(LIB_DIR)/libhash.a

OBJ_FILES := $(patsubst %.c, $(BIN_DIR)/%.o, $(SRC_FILES))

all: $(LIBRARY)

test: $(BIN_DIR)/hash

$(BIN_DIR)/hash: $(TEST_DIR)/tests.c $(LIBRARY)
	${CC} ${CFLAGS} $<  -lhash -Llib ${ENDFLAGS} $@

$(LIBRARY): $(OBJ_FILES)
	ar rcs $(LIBRARY) $^

$(BIN_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< ${ENDFLAGS} $@


clean:
	rm -f $(OBJ_FILES) lib/libhash.a bin/hash
