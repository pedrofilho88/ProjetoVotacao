#!/bin/bash

stty 57600 -ixon icanon </dev/ttyUSB0 #abre serial e ajusta baud rate
sleep 1 #aguarda 1 segundo
stty -a -F /dev/ttyUSB0 #desabilita reinicializacao
sleep 1 #aguarda 1 segundo
exec 3<> /dev/ttyUSB0 #abre serial novamente
sleep 1 #aguarda 1 segundo

resposta=" "
while (true) do	

	status=" " # zera variavel status antes de começar
	sleep 1	   # espera um segundo a cada iteração
	status=$(curl http://localhost:3000/votacao/estagio)
	echo
	echo
	echo $status
	echo
	echo
        
	if [[ $status != "presenca" ]]; then
		echo "digite senha vereador: "
		#read codigo
		read -t 0.5 leitura < /dev/ttyUSB0 #define meio segundo de espera
		echo "lido da serial: "$leitura
		leitura=" " 
		#resposta=$(curl -d "codigo="${codigo}"" http://localhost:3000/votacao/presenca)
		#echo "enviou o codigo pra o site, RESPOSTA: "$resposta
		#if [[ $resposta == "ok" ]]; then
		#	echo "enviando OK para serial"
		#else
			#echo "DEU BOSTA"
		#fi
		#sleep 0.2 # aguarda 200 ms para estabilidade		
		#echo confirmacao > /dev/ttyUSB0  #ENVIA CONFIRMACAO P/ MODULO
		codigo=" "  # zera variavel codigo para proxima leitura
		resposta=" " #zera variavel resposta
		#sleep 0.5  # aguarda 500 ms		
	elif [[ $status == "votar" ]]; then
		echo "Digite o voto [1-sim/2-não] "
		read voto
		echo "numero de caracteres:"${#voto}
		#sleep 0.5 # aguarda 500 ms	
		codigo=${voto:0:${#voto}-1}	 # voto menos os dois ultimos digitos
		#echo "codigo depois do recorte: " $codigo
		#sleep 5
		curl -d "codigo="${codigo}"&voto="${voto: (-1)}"" http://localhost:3000/votacao
		echo "voto: " ${voto: (-1)}   
		#sleep 0.2
		#echo confirmacao > /dev/ttyUSB0 # ENVIA CONFIRMACAO P/MODULO
		codigo=" "  # zera variavel codigo para proxima leitura
		voto=""     # zera variavel  voto  para proxima leitura
		#sleep 0.5    # aguarda 500 ms
	else
		echo "sem acao"
		echo 		
	fi

done
