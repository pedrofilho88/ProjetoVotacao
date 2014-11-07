
#esta instucao roda porem da o erro "[: ==: unary operator expected"
if [ $status == "presenca" ]; then
#esta instucao roda normalmente sem apresentar erro
if [[ $status == "presenca" ]]; then
#esta instucao tambem funciona normalmente
if [ "$status" == "presenca"]; then # tambem funciona só com um sinal de "="