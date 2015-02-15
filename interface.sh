#!/bin/bash
#************** inibe  reinicialização da serial ***********************************************
stty 115200 -ixon icanon </dev/ttyUSB0 #abre serial e ajusta baud rate
sleep 1 #aguarda 1 segundo
stty -a -F /dev/ttyUSB0 #desabilita reinicializacao
sleep 1 #aguarda 1 segundo
exec 3<> /dev/ttyUSB0 #abre serial novamente
status=" " # zera variavel status antes de começar
resposta=" "
sleep 8 # Aguarda 5 seg até que o node seja inicializado

cd /home/pedro/principal && ./sobeNode.sh &   #sobe banco de dados
sleep 8 # Aguarda 8 segundos até o banco subir
./chamaChrome.sh & #Inicializa o google chrome


#********************************  FUNÇÃO PRINCIPAL  **********************************************
principal(){
	lido_site=" "	
	lido_site=$(curl http://localhost:3000/votacao/estagio) #atribui a lido_site retorno do site
	sleep 0.1  # Agurda 1 décimo de segundo antes da comparação	
	if [[  $status == $lido_site ]]; then
		decide
	else
		echo $lido_site > /dev/ttyUSB0 #envia comando para serial
		status=$lido_site #atribui o valor lido pelo para status
		decide
	fi
}
#*******************************  FUNÇÃO DECIDE **************************************************
decide() {
	case "$status" in
		"presenca")	funcaoPresenca;;	#chama função presença			
		"votar") funcaoVotar;;		    #chama função votar
	esac	
}
#********************************  FUNÇÃO VOTAR   **********************************************
funcaoVotar() {	
	codigo=" "
	resposta=" "
	voto=0
	read -t 0.1 voto < /dev/ttyUSB0 # tenta ler alguma coisa por 200 ms		
	if [[ $voto ]]; then
		#echo "voto inteiro"$voto	    
	    #echo "voto"${voto: (-1)}		
		codigo=${voto:0:${#voto}-2}	 # voto menos os dois ultimos digitos
		voto="${voto: (-1)}"
		#echo $codigo"codigo"
		resposta=$(curl -d "codigo=$codigo&voto="${voto}"" http://localhost:3000/votacao)		
		sleep 0.3
		if [[ $resposta == "ok" ]]; then
			echo "confirmacao" > /dev/ttyUSB0 # envia resposta para serial
		fi
		if [[ $resposta == "error" ]]; then
			echo "error" > /dev/ttyUSB0 # envia resposta para serial
		fi
	fi		
}
#*********************************  FUNÇAO PRESENÇA  *******************************************
funcaoPresenca() {	
	resposta=" " #zera variavel resposta denovo
	codigo=0
	read -t 0.2 codigo < /dev/ttyUSB0 # tenta ler alguma coisa por 400 ms
	if [[ $codigo ]]; then			
		resposta=$(curl --keepalive-time 3 -d "codigo="${codigo}"" http://localhost:3000/votacao/presenca)
		sleep 0.2
		if [[ $resposta == "ok" ]]; then
			echo "confirmacao" > /dev/ttyUSB0 #envia resposta de volta para serial
		fi
		if [[ $resposta == "error" ]]; then
			echo "error" > /dev/ttyUSB0 # envia resposta para serial
		fi				
	fi		
}
#*********************************** Chama Função Principal Infinitamente***********************

while (true) do
	principal
done