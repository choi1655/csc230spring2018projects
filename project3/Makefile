# Our main executable depends on main.o (implicit) and stuff.o
hangman.o: hangman.c display.c display.h wordlist.c wordlist.h
	gcc -Wall -std=c99 -g -c hangman.c 

display.o: display.c display.h
	gcc -Wall -std=c99 -g -c display.c

wordlist.o: wordlist.c wordlist.h
	gcc -Wall -std=c99 -g -c wordlist.c


clean:
	rm -f hangman.o display.o wordlist.o
	rm -f hangman
	rm -f output.txt
