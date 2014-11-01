#!/bin/bash
stty 57600 -ixon icanon </dev/ttyUSB0 #abre serial e ajusta baud rate
sleep 1 #aguarda 1 segundo
stty -a -F /dev/ttyUSB0 #desabilita reinicializacao
sleep 1 #aguarda 1 segundo
exec 3<> /dev/ttyUSB0 #abre serial novamente
sleep 1 #aguarda 1 segundo
status=" " # zera variavel status antes de começar
#***********************************************************************************************
principal(){
while (true) do	
	lido_site=" "
	sleep 0.5  # espera um segundo a cada iteração
	lido_site=$(curl http://localhost:3000/votacao/estagio) #atribui a lido_site retorno do site	
	if [[ $status == " " || $status != lido_site ]]; then
		echo $lido_site > /dev/ttyUSB0 #envia comando para serial
		status=$lido_site #atribui o valor lido pelo para status
	    decide
	else
		decide 
	fi	    
done
}


decide() {
	case "$status" in
		"presenca")	funcaoPresenca;;	#chama função presença		
			
		"votar") funcaoVotar;;		
	esac
	principal	
}

funcaoVotar() {
	voto=0
	codigo=" "
	resposta=" "
	read -t 0.4 voto < /dev/ttyUSB0 # tenta ler alguma coisa por 400 ms
	echo $voto
	if [[ $voto != " " ]]; then
		codigo=${voto:0:${#voto}-4}	 # voto menos os dois ultimos digitos
		echo "entrou"
		resposta=$(curl -d "codigo="${codigo}"&voto="${voto: (-3)}"" http://localhost:3000/votacao)
		if [[ $resposta == "ok" ]]; then
			echo $resposta > /dev/ttyUSB0 # envia resposta para serial
		fi
	fi
	principal	
}


funcaoPresenca() {
	codigo=" "
	resposta=" " #zera variavel resposta denovo
	read -t 0.4 codigo < /dev/ttyUSB0 # tenta ler alguma coisa por 400 ms 
	if [[ $codigo != " " ]]; then	
		resposta=$(curl -d "codigo="${codigo}"" http://localhost:3000/votacao/presenca)
		if [[ $resposta == "ok" ]]; then
			echo $resposta > /dev/ttyUSB0 #envia resposta de volta para serial
		fi		
	fi
	principal	
}
principal