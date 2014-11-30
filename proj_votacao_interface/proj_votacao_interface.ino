/*
   proj_votacao
   Date: 17 out 2014
   Autor: Pedro Filho
   e-mail: pedrofilhooliveirasantos@gmail.com
   Versão: 2.0.0
   Desc: Este código é responsável por coletar os dados vindo do radio ligado ao PC
   e enviar via rádio de volta o valor digitado no teclado.
 */
#include "radio.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#define numDispositivos 4
// *************************************************************************************
uint8_t disp = NULL; // Declara uma varivel que dira o disp
uint8_t disp_atual = NULL; // disp que esta trabalhando no momento
int disp_num[numDispositivos] = {NULL,NULL,NULL,NULL};
char strSerial[20]="";
int tamSerial = 0;
int contaDisp = 0; //variavel que conta os dispositivos armazenados
char strArmazenada[numDispositivos][20];
//********************* função setup ***************************************************
void setup(){
    Serial.begin(115200);//inicializa serial baud rate a 57600
    delay(100);//aguarda um segundo
    inicializaRadio();//chama função de radio.h para setar parametros do transmissor
    delay(1000);//espera um tempo pra o radio terminar de inicializar
    zeraStrSerial();
}
// ******************** função loop ****************************************************
void loop(){
        verificaRadio(); //chama função pra ver se chegou alguma coisa' pelo radio
        if(contaDisp && !disp_atual) {
              Serial.println(strArmazenada[verificaDispositivo()]);//manda string da fila pela serial
        }
        if(recebeSerial()){
              if (!disp_atual){
                  enviaTodos(strSerial);//envia qualquer coisa que nao for confirmacao
                  zeraStrSerial();
              } else if (disp_atual){
                   if(comparaString(strSerial, "confirmacao")){
                        enviaEspecifico(disp_atual, "confirmacao");
                        disp_atual = NULL; // zera o disp atual
                        contaDisp--;
                        zeraStrSerial();
                   }
              }
        }
}//fim loop******************************************************************************
bool recebeSerial(){
    tamSerial = 0;
    bool statusSerial = false;
    while(Serial.available()){
      delay(5);
      strSerial[tamSerial] = Serial.read();//le e arazena na string
      tamSerial++;  //incrementa tamanho da string

    }
    if(tamSerial){
        strSerial[tamSerial] = '\0';
        statusSerial = true;
    }
    return statusSerial;
}
void zeraStrSerial(){  // zera o vetor str
    tamSerial = 0;
    for(int a = 0; a < 20; a++){
        strSerial[a]=0;
    }
}
bool comparaString(char *rec1, char *rec2 ){ // compara uma String se iguais dev. true
    int contador=0;
    int y = strlen(rec2)-1;
    bool sinalizador = false;
    for(int v = 0; v < y; v++){
        if(rec1[v] == rec2[v])  contador++;
    }
    if(contador == y)  sinalizador = true;
    return sinalizador;
}
void verificaRadio(){
    int len = 0;
    int tam = 0;
    if (radio.available(&disp)){
        bool done = false;
        while(!done){
              len = radio.getDynamicPayloadSize();
              tam = tam + len;
              done = radio.read(strArmazenada[contaDisp], len);//faz a leitura do radio
              delayMicroseconds(260);
        }
        disp_num[contaDisp] = disp;
        strArmazenada[contaDisp][tam] = '\0';
        contaDisp++;
    }
}

int verificaDispositivo(){
    for(int x=0; x < numDispositivos; x++){
        uint8_t tmp = disp_num[x];
        if(tmp){
            disp_atual = tmp;
            disp_num[x] = NULL;
            return x;
            break;
        }
    }
    return -1;
}
