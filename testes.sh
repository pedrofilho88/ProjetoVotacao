#!/bin/bash


while (true) do	
	
	#status="abcABC123ABCabc" # zera variavel status antes de começar
	sleep 1	
	#status=$(curl http://localhost:3000/votacao/estagio)
    echo
	echo
	read status
	echo "valor lido foi: " $status
	echo "tamanho foi:"  ${#status}
	echo "o último caracter foi:  " ${status:0:${#status}-2}
	echo $status
	echo ${status: -1}
	echo	


done



#"${codigo}" | awk '{gsub(/^ +| +$/,"")} {print "=" $0 "="}'