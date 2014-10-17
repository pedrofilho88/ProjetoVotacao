#!/bin/bash

stty 57600 -ixon icanon </dev/ttyUSB0 #abre serial e ajusta baud rate
sleep 1 #aguarda 1 segundo
stty -a -F /dev/ttyUSB0 #desabilita reinicializacao
sleep 1 #aguarda 1 segundo
exec 3<> /dev/ttyUSB0 #abre serial novamente
sleep 1 #aguarda 1 segundo


while (true) do	
	
	variavel=$(curl http://localhost:3000/votacao/estagio)
        sleep 1
	if [ $variavel = "presenca" ]; then
		echo "enviando valor"
		echo $variavel > /dev/ttyUSB0
		echo "agora aguardando leitura..."
		read lido < /dev/ttyUSB0
		echo "leu " $lido		
 
		#"${lido}" | awk '{gsub(/^ +| +$/,"")} {print "=" $0 "="}'
		echo $lido
		curl -d "codigo="${lido}"" http://localhost:3000/votacao/presenca
				
		echo "enviou o codigo pra o site..." 
                lido= " "
		sleep 1		
	elif [ $variavel = "votar" ]; then
		echo $variavel > /dev/ttyUSB0
		echo $variavel
	else
		echo "sem acao"		
	fi
done



