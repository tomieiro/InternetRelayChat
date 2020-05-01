all: run_servidor
	echo "Starting..."

run_servidor:
	gcc	-o	bin/servidor	src/lista_clientes.c	src/servidor.c	-lpthread
	./bin/servidor

run_cliente:
	gcc	-o	bin/cliente	src/cliente.c	-lpthread
	./bin/cliente
	
clean:
	rm bin/*