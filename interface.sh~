#!/bin/bash

stty 57600 -ixon icanon </dev/ttyUSB0 #abre serial e ajusta baud rate
sleep 1 #aguarda 1 segundo
stty -a -F /dev/ttyUSB0 #desabilita reinicializacao
sleep 1 #aguarda 1 segundo
exec 3<> /dev/ttyUSB0 #abre serial novamente
sleep 1 #aguarda 1 segundo

while (true) do
	
	status=" " # zera variavel status antes de começar
	sleep 1	    # espera um segundo a cada iteração
	status=$(curl http://localhost:3000/votacao/estagio)
        
	if [ $status = "presenca" ]; then
		echo "enviando valor"
		echo $status > /dev/ttyUSB0
		echo "agora aguardando resposta do módulo..."
		read codigo < /dev/ttyUSB0
		echo "leu: " $codigo		
		curl -d "codigo="${codigo}"" http://localhost:3000/votacao/presenca
		echo "enviou o codigo pra o site..."
		sleep 0.2 # aguarda 200 ms para estabilidade		
		echo "confirmacao" > /dev/ttyUSB0  #ENVIA CONFIRMACAO P/ MODULO		 
        codigo=" "  # zera variavel codigo para proxima leitura
		sleep 0.5  # aguarda 500 ms		
	elif [ $status = "votar" ]; then
		echo $status > /dev/ttyUSB0
		echo "agora aguardando resposta do módulo..."
		read -n 1 voto < /dev/ttyUSB0 # Lê o voto
		sleep 0.1 # aguarda 100 ms
		read codigo < /dev/ttyUSB0 # Lê o código/senha do módulo
		curl -d "codigo="${codigo}"&voto="${voto}"" http://localhost:3000/votacao
		echo "enviou o codigo e o voto para o site"
		sleep 0.2
		echo "confirmacao" > /dev/ttyUSB0 # ENVIA CONFIRMACAO P/MODULO
		codigo=" "  # zera variavel codigo para proxima leitura
		voto=""     # zera variavel  voto  para proxima leitura
		sleep 0.5    # aguarda 500 ms
	else
		echo "sem acao"		
	fi

done
