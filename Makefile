all: run_servidor
	echo "Starting..."

run_servidor:
	gcc	-o	bin/servidor	src/lista_clientes.c	src/servidor.c	-lpthread
	./bin/servidor

run_cliente:
	g++	-o	bin/cliente	src/gui.cxx	src/cliente.cpp	-lpthread -lfltk
	./bin/cliente
	
install_libs:
	sudo	apt	install	libfltk1.3-dev

clean:
	rm bin/*