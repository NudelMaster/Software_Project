symnmf: symnmf.o matrix.o symnmf.h matrix.h
	gcc -ansi -Wall -Wextra -Werror -pedantic-errors -o symnmf symnmf.o matrix.o

symnmf.o: symnmf.c
	gcc -ansi -Wall -Wextra -Werror -pedantic-errors -c symnmf.c

matrix.o: matrix.c
	gcc -ansi -Wall -Wextra -Werror -pedantic-errors -c matrix.c

