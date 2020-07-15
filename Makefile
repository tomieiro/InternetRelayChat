all: run_servidor
	echo "Starting..."

run_servidor:
	@printf "IP Externo: "
	@wget -qO- http://ipecho.net/plain
	@printf "\n"
	@gcc	-o	bin/servidor	src/lista_canal.c	src/lista_clientes.c	src/servidor.c	-lpthread
	./bin/servidor

run_cliente:
	g++	-o	bin/cliente	src/cliente_gui.cpp	-lpthread -lfltk
	./bin/cliente

install_libs:
	sudo	apt	install	libfltk1.3-dev

clean:
	rm bin/*
