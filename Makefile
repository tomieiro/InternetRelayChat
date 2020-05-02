all: run_servidor
	echo "Starting..."

run_servidor:
	gcc	-o	bin/servidor	src/lista_clientes.c	src/servidor.c	-lpthread
	./bin/servidor

run_cliente:
	g++	-o	bin/cliente	src/cliente.cpp	-lpthread
	./bin/cliente
	
clean:
	rm bin/*