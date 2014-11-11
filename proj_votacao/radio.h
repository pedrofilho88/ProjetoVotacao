#include <SPI.h>
#include <RF24.h>
#include "printf.h"
/*------------------------------------------------------------------------------*/
#define CE_PIN   9 //pino 9 do microcontrolador
#define CSN_PIN 10 //pino 10 do microcontrolador
// NOTA: define o endereço mac do dispositivo a ser transmitido
const uint64_t recep = 0xE8E8F0F0E1LL; // define o mac do receptor
const uint64_t trans = 0xE8E8F0F0E1LL;// define o mac do transmissor
RF24 radio(CE_PIN, CSN_PIN); // cria uma instancia de "RF24" chamada "radio"
void mudaEstado(bool flag);
//**********************************************************************************
void inicializaRadio(){
      printf_begin();
      radio.begin();//inicializa o rádio
      radio.setDataRate(RF24_250KBPS);//define a velocidade de transmissao
      radio.setPALevel(RF24_PA_MAX);//ajusta a pontencia de transmissao
      radio.setChannel(125);//define o canal de transmissao
      radio.enableDynamicPayloads();// (tenho que estudar isso ainda!)
      radio.setRetries(15,15);// ajusta o valor máximo de tentativas
      radio.setCRCLength(RF24_CRC_16);//define tamanho do dado transmitido
      mudaEstado(true); //inicializa o radio a prinicipio como receptor
}//fim_inicializaRadio()*************************************************************
void mudaEstado(bool flag){
    if(flag){
        radio.openWritingPipe(trans);
        radio.openReadingPipe(1,recep); //abre a comunicação como receptor
        radio.startListening(); // entra em listening
    }
    else if(!flag)	{
        radio.stopListening(); //desabilita a recepção de dados
        radio.openReadingPipe(1,recep);
        radio.openWritingPipe(trans); //abre a comunicacao como transmissor
    }
}// fim_mudaEstado()*****************************************************************
