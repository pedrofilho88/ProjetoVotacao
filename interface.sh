#!/bin/bash
stty 115200 -ixon icanon </dev/ttyUSB0 #abre serial e ajusta baud rate
sleep 1 #aguarda 1 segundo
stty -a -F /dev/ttyUSB0 #desabilita reinicializacao
sleep 1 #aguarda 1 segundo
exec 3<> /dev/ttyUSB0 #abre serial novamente
sleep 1 #aguarda 1 segundo
status=" " # zera variavel status antes de começar
resposta=" "
#********************************  FUNÇÃO PRINCIPAL  **********************************************
principal(){
	lido_site=" "	
	lido_site=$(curl http://localhost:3000/votacao/estagio) #atribui a lido_site retorno do site
	sleep 0.1  # Agurda 1 décimo de segundo antes da comparação	
	if [[  $status == $lido_site ]]; then
		decide
	else
		#echo $lido_site
		echo $lido_site > /dev/ttyUSB0 #envia comando para serial
		status=$lido_site #atribui o valor lido pelo para status
		decide
	fi
}
#*******************************  FUNÇÃO DECIDE **************************************************
decide() {
	case "$status" in
		"presenca")	funcaoPresenca;;	#chama função presença		
			
		"votar") funcaoVotar;;		
	esac
	principal	
}
#********************************  FUNÇÃO VOTAR   **********************************************
funcaoVotar() {	
	codigo=" "
	resposta=" "
	voto=0
	read -t 0.2 voto < /dev/ttyUSB0 # tenta ler alguma coisa por 400 ms
	#echo "voto lido"
	#echo $voto
	#echo "tamanho"
	echo ${#voto}
	if [[ $voto ]]; then
	    #echo "entrou"
	    #echo "voto"${voto: (-1)}		
		codigo=${voto:0:${#voto}-2}	 # voto menos os dois ultimos digitos
		voto="${voto: (-1)}"
		#echo "voto cortado"$voto		
		#echo $codigo"codigo"
		resposta=$(curl -d "codigo=$codigo&voto="${voto}"" http://localhost:3000/votacao)		
		sleep 0.3
		#echo "resposta foi: " $resposta
		if [[ $resposta == "ok" ]]; then

			echo "confirmacao" > /dev/ttyUSB0 # envia resposta para serial
		fi
		if [[ $resposta == "error" ]]; then
			echo "error" > /dev/ttyUSB0 # envia resposta para serial
		fi
	fi
	principal	
}
#*********************************  FUNÇAO PRESENÇA  *******************************************
funcaoPresenca() {	
	resposta=" " #zera variavel resposta denovo
	codigo=0
	read -t 0.2 codigo < /dev/ttyUSB0 # tenta ler alguma coisa por 400 ms
	#echo
	#echo "codigo lido|"$codigo
	#echo		
	if [[ $codigo ]]; then			
		resposta=$(curl -d "codigo="${codigo}"" http://localhost:3000/votacao/presenca)
		sleep 0.2
		#echo "resposta foi: " $resposta
		if [[ $resposta == "ok" ]]; then
			#echo "resposta ok"
			echo "confirmacao" > /dev/ttyUSB0 #envia resposta de volta para serial
		fi
		if [[ $resposta == "error" ]]; then
			echo "error" > /dev/ttyUSB0 # envia resposta para serial
		fi				
	fi

	principal	
}
#***********************************  FIM  *********************************************************
principal