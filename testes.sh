#!/bin/bash


while (true) do	
	
	status=" " # zera variavel status antes de começar
	sleep 1	
	status=$(curl http://localhost:3000/votacao/estagio)
        echo
	echo
	echo
	echo "valor lido foi: " $status
	echo
	echo
	echo	


done



#"${codigo}" | awk '{gsub(/^ +| +$/,"")} {print "=" $0 "="}'