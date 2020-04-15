all:
	g++	-o	bin/irc.o	src/irc.cpp

run_servidor:
	g++	src/servidor.cpp	-o	bin/servidor
	./bin/servidor

run_cliente:
	g++	src/cliente.cpp	-o	bin/cliente
	./bin/cliente

clean:
	rm bin/*