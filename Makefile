all:	irc.o
	g++	-o	bin/irc.o	src/irc.cpp

test:
	#g++	src/cliente.cpp	-o	bin/cliente
	g++	-Wall	src/server.cpp	-o	bin/server

clean:
	rm bin/*
