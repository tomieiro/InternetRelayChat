all:	irc.o
	gcc	-o	bin/irc.o	src/irc.c	-lsocket

clean:
	rm bin/*
