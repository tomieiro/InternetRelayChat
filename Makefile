all:
	g++	-o	bin/irc.o	src/irc.cpp

run_servidor:
	g++	-o	bin/servidor	src/socket_utils.cpp	src/servidor.cpp	
	./bin/servidor

run_cliente:
	g++	-o	bin/cliente	src/socket_utils.cpp	src/cliente.cpp
	./bin/cliente

clean:
	rm bin/*
