# InternetRelayChat
Projeto de chat usando sockets para disciplina de Redes

**INSTRUÇÕES DE COMPILAÇÃO:**

  **Para compilar e executar o servidor: make run_servidor**
  
  -> O servidor retornará pouco antes de iniciar, um número de endereço de ip **(I)** que será usado para conexão entre redes diferentes. Para conexão entre computadores fora da mesma rede, é necessario abrir uma porta no roteador; A padrão do programa é a porta 31010. Caso deseje rodar localmente, não há necessidade de abrir uma porta.
      
      
  **Para compilar e executar o cliente : make run_cliente**
  
  -> Basta entrar com o endereço de ip 0.0.0.0(para local); caso deseje rodar na mesma LAN em um computador diferente, use o ip do computador na LAN, 192.168.1.35, por exemplo. Se deseja rodar de outra rede, use o IP retornado em **(I)**
