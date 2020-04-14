all:	irc.o
	g++	-o	bin/irc.o	src/irc.cpp

test:
	g++	src/cliente.cpp	-o	bin/cliente
	g++	-Wall	src/servidor.cpp	-o	bin/servidor

clean:
	rm bin/*
