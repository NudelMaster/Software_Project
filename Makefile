symnmf: symnmf.o matrix.o symnmf.h matrix.h
	gcc -ansi -Wall -Wextra -Werror -pedantic-errors -lm -o symnmf symnmf.o matrix.o

symnmf.o: symnmf.c
	gcc -ansi -Wall -Wextra -Werror -pedantic-errors -lm -c symnmf.c

matrix.o: matrix.c
	gcc -ansi -Wall -Wextra -Werror -pedantic-errors -lm -c matrix.c

