#include <Keypad.h>
//*************************************************************************************
void zeraVetor();
int cont=0;
const byte ROWS = 4; //quatro linhas
const byte COLS = 3; //tres colunas
byte rowPins[ROWS] = {2, 3, 4, 5}; //conectar pino digital ao pino do teclado(linha)
byte colPins[COLS] = {6, 7, 8}; //conectar pino digital ao pino do teclado(coluna)
char vetor[20];//declara vetor que armazenará teclas digitadas
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );//instancia
//**************************************************************************************

void recebeTeclado(){
  zeraVetor();
	char tecla=0;//declara variavel que recebe a tecla digitada no momento

	cont=0;//declara contador de caracters digitados
	//char *pont;// declara ponteiro que retornará inforção
	do{
		tecla=0;
		tecla = keypad.getKey(); //atribui se alguma tecla for pressionada
		if(tecla){//se alguma tecla for pressionada
			vetor[cont]=tecla;//atribui tecla pessionada ao vetor
			cont++;//incrementa o contador de teclas
		}
	} while(tecla != '#');//enquanto a tecla # não for pressionada não sai daqui
	cont--;
	vetor[cont] = ' ';
	//pont=&vetor[0];//atribui o vetor ao ponteiro
	//return vetor;//retorna teclas digitadas no teclado e envia atraves do ponteiro
}//fim_recebe_teclado*****************************************************************


void zeraVetor(){
  for (int x = 0; x < 20; x++){
      vetor[x]=0;
  }

}



