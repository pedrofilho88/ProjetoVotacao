/*
   proj_votacao
   Date: 17 out 2014
   Autor: Pedro Filho
   e-mail : pedrofilhooliveirasantos@gmail.com
   Versão: 1.0.0
   Desc:  Este código é responsável por coletar os dados vindo do radio ligado ao PC
   e enviar via rádio de volta o valor digitado no teclado.
 */
#include "teclado.h"
#include "radio.h"
#include <Keypad.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <LiquidCrystal.h>
// *************************************************************************************
char *strTmp=NULL;//define um ponteiro temporario pra armazenar str
char senha[20]="";
char str[20]="";
char votar[20] = {"votar"};
char presenca[20] = {"presenca"};
char confirmacao[20] = {"confirmacao"};
int tam = 0;
//********************* função setup ***************************************************
void setup(){
    delay(100);//aguarda um segundo
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("CAMARA");
    lcd.setCursor(3,1);
    lcd.print("MUNICIPAL");
    inicializaRadio();//chama função de radio.h para setar parametros do transmissor
    delay(1000);//espera um tempo pra o radio terminar de inicializar

}
// ******************** função loop ****************************************************
void loop(){
  tam = 0;
  int len = 0;
  if(str[0])  zera(); // se tiver alguma coisa na str zera o vetor
  if (radio.available()){
        bool done = false;
        while(!done){
          len = radio.getDynamicPayloadSize();
          tam = tam + len;
          done = radio.read(&str, len);//faz a leitura do radio
          delay(5);
        }
        tam++;
        str[tam] = '\0';
  } //fim_while
  if (tam) {
        compara();
  }
  delay(40);// aguarda 80 ms
//*************   fim radio.available()     ********************************************
}//fim loop******************************************************************************

bool comparaString(char *rec1, char *rec2 ){ // compara uma String se iguais dev. true
    int contador=0;
    bool sinalizador = false;
    for(int v = 0; v < tam; v++){
        if(rec1[v]==rec2[v]){
           contador++;
        }
    }
    if(contador == tam){
          sinalizador = true;
    }
    return sinalizador;
}

void zera(){  // zera o vetor str
   for(int a = 0; a < 20; a++){
      str[a]=0;
   }
}

void zeraSenha(){
    for(int a = 0; a < 20; a++){
      senha[a]=0;
   }
}

void compara(){ // faz uma comparacao para saber o que fazer
       if( comparaString(str, presenca) ){
          if(!strTmp){
            delay(80);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("DIGITE SUA SENHA");
            recebeTeclado();
            strcpy(senha, vetor); // copia vetor para senha
            strTmp = presenca; // atribui presenca ao ponteiro
            mudaEstado(false);//muda o estado para transmissao
            bool codigo = radio.write(&vetor, strlen(vetor)); //envia o a senha via radio
            mudaEstado(true); // muda o estado para recepção
          }
      // fim presenca ******************************************************************
      } else if( comparaString(str, votar)){
          if(strTmp && senha[0]){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("VOTE:1-SIM|2-NAO");
            recebeTeclado();
            strcat(senha, vetor);//anexa voto digitado para o final da senha
            delay(80);
            strTmp = votar; // atribui votar ao ponteiro
            mudaEstado(false); // muda o estado para transmissao
            delay(20); // aguarda 20 ms
            bool codigo = radio.write(&senha, strlen(senha));
            mudaEstado(true); // muda o estado pra recepção
            zeraSenha();
          }
      // fim votar ********************************************************************
      } else if( comparaString(str, confirmacao) ){
          if(comparaString(strTmp, presenca)){
              lcd.clear();
              lcd.setCursor(4, 0);
              lcd.print("Presenca");
              lcd.setCursor(3, 1);
              lcd.print("Confirmada!");
          }
          if (comparaString(strTmp, votar)){
              lcd.clear();
              lcd.setCursor(6, 0);
              lcd.print("Voto");
              lcd.setCursor(3, 1);
              lcd.print("Computado!");
              strTmp = NULL;
          }
      }
      //fim do if confirmação *********************************************************

}
