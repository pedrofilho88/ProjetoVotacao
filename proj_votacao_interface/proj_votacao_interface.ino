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
int  tamSerial = 0;
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
  while(Serial.available()){
      strSerial[tamSerial] = Serial.read();//le e arazena na string
      tamSerial++;  //incrementa tamanho da string
  }

  if(tamSerial){
      if (!disp_num && !strcmp(strSerial,"ok")){
          enviaTodos(strSerial);
      }
      zeraStrSerial(); // zera conteudo da string StrSerial
  }

  if (radio.available(&disp_num)){
      bool done = false;
      while(!done){
        len = radio.getDynamicPayloadSize();
        tam = tam + len;
        done = radio.read(&strRadio, len);//faz a leitura do radio
        delay(5);
      }
      if(disp_num == 1){
        Serial.println();
      }
      tam++;
      strRadio[tam] = '\0';
  } //fim_while
  if (tam) {
        Serial.println(strRadio);
        zeraStrRadio();
  }
  delay(40); // Aguarda 80 ms
//*************   fim radio.available()     ********************************************
}//fim loop******************************************************************************

void zeraStrSerial(){
    for(int a = 0; a < 20; a++){
      strSerial[a]=0;
   }
}

void zeraStrRadio(){  // zera o vetor str
   for(int a = 0; a < 20; a++){
      strRadio[a]=0;
   }
}

