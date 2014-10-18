#include "teclado.h"
#include "radio.h"
#include <Keypad.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
// *************************************************************************************
char *strTmp;//define um ponteiro temporario pra armazenar str
char *senha = NULL;
char str[20]="";
char votacao[20] = {"votacao"};
char presenca[20] = {"presenca"};
char confirmacao[20] = {"presenca"};
int tam = 0;
//********************* função setup ***************************************************
void setup(){
    Serial.begin(57600);//inicializa serial baud rate a 115200
    delay(100);//aguarda um segundo
    inicializaRadio();//chama função de radio.h para setar parametros do transmissor
    delay(1000);//espera um tempo pra o radio terminar de inicializar
}
// ******************** função loop ****************************************************
void loop(){
  tam = 0;
  int len = 0;
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

void compara(){ // faz uma comparacao para saber o que fazer
       if( comparaString(str, presenca) ){
          delete senha; // apaga a senha anterior
          delay(80);
          recebeTeclado();
          senha = vetor; // armazena senha digitada no ponteiro
          strTmp = presenca; // atribui presenca ao ponteiro
          zera();
          mudaEstado(false);//muda o estado para transmissao
          bool codigo = radio.write(&vetor, strlen(vetor)); //envia o a senha via radio
          mudaEstado(true); // muda o estado para recepção
      // fim presenca ******************************************************************
      } else if( comparaString(str, votacao)){
          recebeTeclado();
          delay(80);
          strTmp = votacao; // atribui votacao ao ponteiro
          zera();
          mudaEstado(false); // muda o estado para transmissao
          bool  voto  = radio.write(&vetor, strlen(vetor)); // envia o voto
          delay(50); // aguarda 50 ms até a primeira mensagem ser enviada
          bool codigo = radio.write(senha, strlen(senha));
          mudaEstado(true); // muda o estado pra recepção
      // fim votacao ******************************************************************
      } else if( comparaString(str, confirmacao) ){
          if(comparaString(strTmp, presenca)){
              Serial.println("Presenca confirmada!");
              zera();
              delete strTmp;
          }
          if (comparaString(strTmp, votacao)){
              Serial.println("Voto Computado");
              zera();
              delete strTmp;
          }
      }
      //fim do if confirmação *********************************************************

}

/*
Este codigo ainda é experimental ainda sao necessarios testes e implementações
adicionais que ainda não estao aqui
*/
