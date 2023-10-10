CC = gcc
FLAGS = -Wall -g
ENDFLAGS = -lcunit -o

test.o: common.o hash.o iterator.o linked_list.o extended.o tests.o
	${CC} ${FLAGS} $^ ${ENDFLAGS} $@

hash_table.o: common.o hash.o iterator.o linked_list.o extended.o
	${CC} ${FLAGS} -c $^ ${ENDFLAGS} $@

common.o: common.c
	${CC} ${FLAGS} -c $^ -o $@
hash.o: hash.c
	${CC} ${FLAGS} -c $^ -o $@
iterator.o: iterator.c
	${CC} ${FLAGS} -c $^ -o $@
linked_list.o: linked_list.c
	${CC} ${FLAGS} -c $^ -o $@
extended.o: extended.c
	${CC} ${FLAGS} -c $^ -o $@
tests.o: tests.c
	${CC} ${FLAGS} -c $^ -o $@
