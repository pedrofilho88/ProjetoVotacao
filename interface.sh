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
	if [[  $status == $lido_site ]]; then
		decide
	else
		echo $lido_site > /dev/ttyUSB0 #envia comando para serial
		echo "entrou no primeiro if"
		status=$lido_site #atribui o valor lido pelo para status
		echo "status: "$status 
		echo "lido_site: "$lido_site
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
	codigo=" "
	resposta=" "
	read -t 0.4 voto < /dev/ttyUSB0 # tenta ler alguma coisa por 400 ms	
	if [[ $voto ]]; then		
		codigo=${voto:0:${#voto}-2}	 # voto menos os dois ultimos digitos			
		resposta=$(curl -d "codigo=$codigo&voto=${voto: (-1)}" http://localhost:3000/votacao)		
		sleep 0.3
		echo "resposta: " $resposta
		if [[ $resposta == "ok" ]]; then
			echo "confirmacao" > /dev/ttyUSB0 # envia resposta para serial
		fi
	fi
	principal	
}
${voto_lido:0:${#voto_lido}-4}
funcaoPresenca() {
	
	resposta=" " #zera variavel resposta denovo
	read -t 0.4 codigo < /dev/ttyUSB0 # tenta ler alguma coisa por 400 ms 
	if [[ $codigo ]]; then	
		resposta=$(curl -d "codigo="${codigo}"" http://localhost:3000/votacao/presenca)
		sleep 0.2
		if [[ $resposta == "ok" ]]; then
			echo "resposta ok"
			echo "confirmacao" > /dev/ttyUSB0 #envia resposta de volta para serial
		fi		
	fi
	principal	
}
principal