symnmf: symnmf.o matrix.o symnmf.h matrix.h
	gcc -ansi -Wall -Wextra -Werror -pedantic-errors -g -o symnmf symnmf.o matrix.o -lm

symnmf.o: symnmf.c
	gcc -ansi -Wall -Wextra -Werror -pedantic-errors -c symnmf.c

matrix.o: matrix.c
	gcc -ansi -Wall -Wextra -Werror -pedantic-errors -c matrix.c

clean:
	@echo "Cleaning up"
	@rm -f *.o symnmf
