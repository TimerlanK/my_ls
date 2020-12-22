CC=gcc
CC_OPT=-g3 -fsanitize=address -Wall -Wextra -Werror
OUTPUT=my_ls

all:${OUTPUT}

${OUTPUT}: my_ls.o
	${CC} ${CC_OPT} my_ls.o -o ${OUTPUT}

main.o: main.c
	${CC} ${CC_OPT} -c my_ls.c

clean:
	rm *.o ${OUTPUT}