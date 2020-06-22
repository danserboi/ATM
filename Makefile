build:
	gcc -g main.c utils.c -o atm -Wall -Wextra

run:
	./atm

clean:
	rm -rf atm
	rm -rf *.out
