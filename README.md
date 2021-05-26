# InternetRelayChat
Projeto de chat usando sockets utlizando C/C++

**INSTRUÇÕES DE COMPILAÇÃO:**

  **Para compilar e executar o servidor: make run_servidor**
  
  -> O servidor retornará pouco antes de iniciar, um número de endereço de ip **(I)** que será usado para conexão entre redes diferentes. Para conexão entre computadores fora da mesma rede, é necessario abrir uma porta no roteador; A padrão do programa é a porta 31010. Caso deseje rodar localmente, não há necessidade de abrir uma porta.
      
      
  **Para compilar e executar o cliente : make run_cliente**
  
  -> Basta entrar com o endereço de ip 0.0.0.0(para local); caso deseje rodar na mesma LAN em um computador diferente, use o ip do computador na LAN, 192.168.1.35, por exemplo. Se deseja rodar de outra rede, use o IP retornado em **(I)**
  
  -> Vale ressaltar que os comandos "ping", "kick", "nickname", "mute", "unmute" e "whois" sao chamados pela GUI igualmente a sua versao cli.
     
     Por exemplo, quando clicamos no botao %mute% e preenchemos o campo acima dele com "fulano", a GUI envia "/mute#fulano" para o servidor, respeitando as regras
     da especificacao. Alem disso, o comando /join é respeitado e enviado ao server como "/join#nome_canal" assim que o botao de iniciar eh clicado.

Nota: No projeto foi adotada a seguinte politica: uma pessoa pode entrar num mesmo canal com o nome de uma pessoa existente la, porem quando la dentro, nao pode
      alterar seu nome para o mesmo que o de outra pessoa da sala. Para evitar que pessoas com o mesmo nome entrem num mesmo canal, basta voltar a linha comentada
      no arquivos: (239 e 242 do servidor.c) e (427 e 428 no cliente_gui.cpp).
