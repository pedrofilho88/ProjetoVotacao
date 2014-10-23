COMO USAR O GIT SIMPLIFICADO

#--> ADICIONAR UM ARQUIVO NO GIT

$ git add .

#--> CRIAR UM NOVO BRANCH

$ git checkout -b <novobranch>

#--> APAGAR UM BRANCH

$ git branch -d <nome_do_branch>

#--> MOSTRAR EM QUAL BRANCH VC ESTA

$ git branch -a

#--> SABER O QUE FOI ALTERADO

$ git status

#--> COMO SUBIR O CODIGO PARA O SERVIDOR DO GIT 

$ git push origin <nome do branch> 

#--> COMMITAR UMA MODIFICAÇÃO ANTES DE MUDAR DE BRANCH

$ git commit -am "comentario do que foi feito no codigo"git

#--> COMO FAZER UM MERGE[juntar] ENTRE OS CÓDIGOS

$ git merge <nome do branch>

#--> como reverter um estado anterior por exemplo para antes de um merge

$ git reset --hard 89aeb848b4d010b02d760650f16162837eb3daae 

//SENDO O VALOR APOS O --hard <endereco_do_commit_por_exemplo>

#--> COMO CRIAR UM BRANCH E COPIAR OS ARQUIVOS PARA OUTRO BRANCH

$ git checkout <master_por_exemplo> 
$ git branch <novo_branch>
$ git checkout -b <novo_branch> <master_por_exemplo>

// desta forma o conteúdo de um branch -
// mais um testes de credencial
#--> COMO CONFIGURAR O GIT PARA FICAR UM MES SEM PRECISAR DIGITAR A SENHA E USUARIO

$ git config --global credential.helper 'cache -- 2592000'
