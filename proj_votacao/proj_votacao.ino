/*****************************************************************************************
   proj_votacao
   Date: 6 dec 2014
   Autor: Pedro Filho
   e-mail : pedrofilhooliveirasantos@gmail.com
   Versão: 1.2.0
   Desc:  Este código é responsável por coletar os dados vindo do radio ligado ao PC
   e enviar via rádio de volta o valor digitado no teclado.
 ****************************************************************************************/
#include "teclado.h"
#include "radio.h"
#include <Keypad.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <LiquidCrystal.h>
/***************************************************************************************/
char *strTmp=NULL;//define um ponteiro temporario pra armazenar str
char senha[20]="";
char str[20]="";

void setup(){
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("CAMARA");
    lcd.setCursor(3,1);
    lcd.print("MUNICIPAL");
    inicializaRadio();//chama função de radio.h para setar parametros do transmissor
    delay(1000);//espera um tempo pra o radio terminar de inicializar
}

void loop(){
    if(recebeRadio())  compara(str[0]); //compara passando primeiro caracter da string
}

/** função compara duas strings e devolve o primeiro caracter se verdadeiro*/
char comparaString(char *rec1, char *rec2 ){ // compara String se iguais retorna true
    int contador=0;
    for(int v=0; v<strlen(rec2); v++) if(rec1[v]==rec2[v]) contador++;
    if(contador==strlen(rec2)) return rec1[0];
    else return 0;
}

/** função que recebe dados vindo do rádio e armazena no vetor str*/
bool recebeRadio(){
    int len = 0;
    bool done = false;
    if(str[0]) zera(); // se tiver alguma coisa na str zera o vetor
    if (radio.available()){
        while(!done){
            len = radio.getDynamicPayloadSize();
            done = radio.read(&str, len);//faz a leitura do radio
        }
    }
    return done; //retorna verdadeiro caso alguma coisa for recebida
}

/** zera o vetor str que recebe os dados da serial*/
void zera(){  // zera o vetor str
   for(int a = 0; a < 20; a++) str[a]=0;
}

/** zera senha digitada no teclado e armazenada na string*/
void zeraSenha(){
    for(int a = 0; a < 20; a++) senha[a]=0;
}

/** faz uma comparacao para saber o que fazer*/
void compara(char recb){
      switch(recb) {
          case 'p':
              if(!strTmp) funcaoPresenca();
              break;
          case 'v':
              if(strTmp && senha[0]) funcaoVotar();
              break;
          case 'c':
              funcaoConfirmacao();
              break;
          case 'e':
              funcaoError();
              break;
      }
}

/** funcao que recebe os dados da votação via teclado e envia via radio*/
void funcaoVotar(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VOTE:1-SIM/2-NAO");
    recebeTeclado(false);
    strcat(senha, vetor);//anexa voto digitado para o final da senha
    strTmp = "votar"; // atribui votar ao ponteiro
    mudaEstado(false); // muda o estado para transmissao
    delayMicroseconds(200); // aguarda 20 ms
    bool codigo = false;
    codigo = radio.write(&senha, strlen(senha));
    while(!codigo) codigo=radio.write(&vetor, strlen(vetor));
    mudaEstado(true); // muda o estado pra recepção
}

/** funcao recebe digitada no teclado armazena em senha[] e envia via radio*/
void funcaoPresenca(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIGITE SUA SENHA");
    recebeTeclado(true);
    strcpy(senha, vetor); // copia vetor para senha
    strTmp = "presenca"; // atribui presenca ao ponteiro
    mudaEstado(false);//muda o estado para transmissao
    bool codigo = false;
    codigo = radio.write(&vetor, strlen(vetor)); //envia o a senha via radio
    while(!codigo) codigo = radio.write(&vetor, strlen(vetor));
    mudaEstado(true); // muda o estado para recepção
}

/** funcao confirma o recebimento e o processamento do dado por parte da pagina*/
void funcaoConfirmacao(){
    if(comparaString(strTmp, "presenca")){
         lcd.clear();
         lcd.setCursor(4, 0);
         lcd.print("Presenca");
         lcd.setCursor(3, 1);
         lcd.print("Confirmada!");
    } else if (comparaString(strTmp, "votar")){
         lcd.clear();
         lcd.setCursor(6, 0);
         lcd.print("Voto");
         lcd.setCursor(3, 1);
         lcd.print("Computado!");
         zeraSenha(); // zera a string senha
         strTmp = NULL;
    }
}

/** função verifica que tipo de erro (presença ou votação) e trata o erro de acordo*/
void funcaoError(){
    if(comparaString(strTmp, "presenca")){
         lcd.clear();
         lcd.print("Senha Incorreta!");
         lcd.setCursor(0,1);
         lcd.print("Digite Novamente");
         delay(4000);
         strTmp = NULL; // aponta o ponteiro temporario para null
         zeraSenha(); ;// zera o vetor com a senha anterior
         funcaoPresenca(); // chama a funçao presença novamente
    } else if (comparaString(strTmp, "votar")){
         lcd.clear();
         lcd.setCursor(6,0);
         lcd.print("Erro!");
         lcd.setCursor(1,1);
         lcd.print("Reenviando...");
         mudaEstado(false); // muda o estado para transmissao
         bool codigo = false;
         codigo = radio.write(&senha, strlen(senha));
         while(!codigo) codigo = radio.write(&vetor, strlen(vetor));
         mudaEstado(true); // muda o estado pra recepção
         zeraSenha();
    }
}
