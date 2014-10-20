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
	echo
	echo
	echo $status
	echo
	echo
        
	if [ $status = "presenca" ]; then
		echo "enviando presenca pela seiral"
		echo $status > /dev/ttyUSB0
		echo "agora aguardando resposta do módulo..."
		read codigo < /dev/ttyUSB0
		echo "leu: " $codigo		
		curl -d "codigo="${codigo}"" http://localhost:3000/votacao/presenca
		echo "enviou o codigo pra o site..."
		sleep 0.2 # aguarda 200 ms para estabilidade		
		echo confirmacao > /dev/ttyUSB0  #ENVIA CONFIRMACAO P/ MODULO
		codigo=" "  # zera variavel codigo para proxima leitura
		sleep 0.5  # aguarda 500 ms		
	elif [ $status = "votar" ]; then
		echo "enviando votar pela serial"
		echo $status > /dev/ttyUSB0 # envia o status de votar para serial
		sleep 0.5
		echo "agora aguardando resposta do módulo..."
		read voto < /dev/ttyUSB0 # limpa o buffer da serial USB0
		voto=" " # limpa a variavel voto
		read  voto < /dev/ttyUSB0 # Lê a senha e o voto musturados
		echo "lido voto:"$voto
		echo "numero de caracteres:"${#voto}
		sleep 0.5 # aguarda 500 ms	
		codigo=${voto:0:${#voto}-4}	 # voto menos os dois ultimos digitos
		echo "codigo depois do recorte: " $codigo
		sleep 5
		curl -d "codigo="${codigo}"&voto="${voto: (-3)}"" http://localhost:3000/votacao
		echo "enviou o voto: " ${voto: (-3)}   
		sleep 0.2
		echo confirmacao > /dev/ttyUSB0 # ENVIA CONFIRMACAO P/MODULO
		codigo=" "  # zera variavel codigo para proxima leitura
		voto=""     # zera variavel  voto  para proxima leitura
		sleep 0.5    # aguarda 500 ms
	else
		echo "sem acao"		
	fi

done

#curl -d "codigo="${codigo}"" http://localhost:3000/votacao/presenca