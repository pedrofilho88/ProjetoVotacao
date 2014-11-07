#define tls // se for teclado normal apenas comentar essa linha
#include <Keypad.h>
#include <LiquidCrystal.h>
//*************************************************************************************
void zeraVetor();
int cont=0;
const byte ROWS = 4; //quatro linhas
const byte COLS = 3; //tres colunas
#ifdef tls //se for o tls
    LiquidCrystal lcd(3, 4, 5, 6, 7, 8);
    byte rowPins[ROWS] = {A0, A1, A2, A3}; //conectar pino digital ao pino do teclado(linha)
    byte colPins[COLS] = {A4, A5, 2}; //conectar pino digital ao pino do teclado(coluna)
#else // teclado normal
    LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);// usando pinos analogicos como pinos do display
    byte rowPins[ROWS] = {2, 3, 4, 5};
    byte colPins[COLS] = {6, 7, 8};
#endif // tls

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
  lcd.setCursor(0, 1);
	cont=0;//declara contador de caracters digitados
	//char *pont;// declara ponteiro que retornará inforção
	do{
		tecla=0;
		tecla = keypad.getKey(); //atribui se alguma tecla for pressionada
		if(tecla){//se alguma tecla for pressionada
      if(tecla != '#') lcd.print(tecla);
			vetor[cont]=tecla;//atribui tecla pessionada ao vetor
			cont++;//incrementa o contador de teclas
			lcd.setCursor(cont, 1);
		}
	} while(tecla != '#');//enquanto a tecla # não for pressionada não sai daqui
	lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("AGUARDE...");
	cont--;
	vetor[cont] = ' ';

}//fim_recebe_teclado*****************************************************************

void zeraVetor(){
  for (int x = 0; x < 20; x++){
      vetor[x]=0;
  }
}


