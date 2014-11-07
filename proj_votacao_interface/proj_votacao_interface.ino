/*
   proj_votacao
   Date: 17 out 2014
   Autor: Pedro Filho
   e-mail: pedrofilhooliveirasantos@gmail.com
   Versão: 1.0.0
   Desc: Este código é responsável por coletar os dados vindo do radio ligado ao PC
   e enviar via rádio de volta o valor digitado no teclado.
 */
#include "radio.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
// *************************************************************************************
uint8_t disp_num = NULL;// Declara uma varivel que dira o dispositivo que recebeu
char senha[20]="";
char strRadio[20]="";
char strSerial[20]="";
int tamSerial = 0;
int tam;
//********************* função setup ***************************************************
void setup(){
    Serial.begin(57600);//inicializa serial baud rate a 57600
    delay(100);//aguarda um segundo
    inicializaRadio();//chama função de radio.h para setar parametros do transmissor
    delay(1000);//espera um tempo pra o radio terminar de inicializar

}
// ******************** função loop ****************************************************
void loop(){
  tamSerial = 0;
  tam = 0;
  int len = 0;
  recebeSerial();// recebe dados da serial e armazena em strSerial[]
  if(tamSerial){
      if (!disp_num ){
          enviaTodos(strSerial);//envia qualquer coisa que nao for confirmacao
      }
      zeraStrSerial();
  }
  if (radio.available(&disp_num)){
      bool done = false;
      while(!done){
            len = radio.getDynamicPayloadSize();
            tam = tam + len;
            done = radio.read(&strRadio, len);//faz a leitura do radio
            delay(5);
      }
      tam++;
      strRadio[tam] = '\0';
      Serial.println(strRadio);//envia pra serial o que foi recebido por radio
      while(!comparaString(strSerial, "confirmacao" )){
           recebeSerial(); //verifica se alguma coisa veio pela serial
      }
      enviaEspecifico(disp_num, "confirmacao");
      disp_num = NULL; // torna o numero do dispositivo inexistente denovo
      zeraStrRadio();
      zeraStrSerial();
  } //fim_if_radio.available()**
//*************   fim radio.available()     ********************************************
}//fim loop******************************************************************************



void recebeSerial(){
    while(Serial.available()){
      delay(5);
      strSerial[tamSerial] = Serial.read();//le e arazena na string
      tamSerial++;  //incrementa tamanho da string
    }

}

void zeraStrRadio(){  // zera o vetor str
   for(int a = 0; a < 20; a++){
      strRadio[a]=0;
   }
}

void zeraStrSerial(){  // zera o vetor str
   for(int a = 0; a < 20; a++){
      strSerial[a]=0;
   }
}

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

