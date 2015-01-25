//#define tls // se for teclado normal apenas comentar essa linha
#include <Keypad.h>
#include <LiquidCrystal.h>
//*************************************************************************************
void zeraVetor();
int cont = 0;
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
void recebeTeclado(bool stts){ // stts = true -> presenca | stts=false votacao
  zeraVetor();
	char tecla=0;//declara variavel que recebe a tecla digitada no momento
  lcd.setCursor(0, 1);
  bool stp = false; // true -> não deixa digitar mais | false -> normal
	cont=0;//declara contador de caracters digitados
	//char *pont;// declara ponteiro que retornará inforção
	do{
		tecla=0;
		tecla = keypad.getKey(); //atribui se alguma tecla for pressionada
		if(tecla == '*' && stts){
      zeraVetor(); // zera o vetor do teclado
      cont=0; //zera contador de string
      tecla=0;
      stp = false;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("DIGITE SUA SENHA");
      lcd.setCursor(0,1); // ajusta a posição do cursor pra primeira pos, 2 linha
		} else if (tecla == '*' && !stts){
      zeraVetor(); // zera o vetor do teclado
      cont=0; //zera contador de string
      tecla=0;
      stp = false;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("VOTE:1-SIM/2-NAO");
      lcd.setCursor(0,1); // ajusta a posição do cursor pra primeira pos, 2 linha
		}

		if(tecla && stts && !stp){//se alguma tecla for pressionada
        if(tecla != '#'){
            lcd.print('*');
            vetor[cont]=tecla;//atribui tecla pessionada ao vetor
            cont++;//incrementa o contador de teclas
            if(cont > 15) stp=true; /** impede que seja digitado mais que 16 caracteres*/
            lcd.setCursor(cont, 1);
        }
		} else if(tecla && !stts && !stp) {
        if(tecla != '#'){
            if(tecla == '1'){
                lcd.print("Sim");
                vetor[cont] = tecla;
                cont++;
                stp = true;
            } else if (tecla == '2'){
                lcd.print("Nao");
                vetor[cont] = tecla;
                cont++;
                stp = true;
            }
			}
		}

	} while(tecla != '#');//enquanto a tecla # não for pressionada não sai daqui
	vetor[cont] = ' ';
	lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("AGUARDE...");
  delay(200);
}//fim_recebe_teclado*****************************************************************

void zeraVetor(){
  for (int x = 0; x < 20; x++){
      vetor[x]=0;
  }
}


