#include <SPI.h>
#include <RF24.h>
#include "printf.h"
/*------------------------------------------------------------------------------*/
#define CE_PIN   9 //pino 9 do microcontrolador
#define CSN_PIN 10 //pino 10 do microcontrolador
#define nro_dispositivos 3 // o numero de receptores
// NOTA: recep define os endereços MAC doS dispositivos a serem recebidos
const uint64_t recep[nro_dispositivos] = { 0xE8E8F0F0E2LL, 0xE8E8F0F0E3LL, 0xE8E8F0F0E4LL };
const uint64_t trans[nro_dispositivos] = { 0xE8E8F0F0E2LL, 0xE8E8F0F0E3LL, 0xE8E8F0F0E4LL };
RF24 radio(CE_PIN, CSN_PIN); // cria uma instancia de "RF24" chamada "radio"
//**********************************************************************************
void inicializaRadio(){
      printf_begin(); // inicializa a função printf
      radio.begin();//inicializa o rádio
      radio.setDataRate(RF24_250KBPS);//define a velocidade de transmissao
      radio.setPALevel(RF24_PA_MAX);//ajusta a pontencia de transmissao
      radio.setChannel(125);//define o canal de transmissao
      radio.enableDynamicPayloads();// (tenho que estudar isso ainda!)
      radio.setRetries(15,15);// ajusta o valor máximo de tentativas
      radio.setCRCLength(RF24_CRC_16);//define tamanho do dado transmitido
} //fim_inicializaRadio()*************************************************************

void modoRecepcao(){ // Entra em listening e espera algum dado via radio
      for(int y=0; y<nro_dispositivos; y++){
        radio.openReadingPipe(y,recep[y]); //abre a comunicação como receptor
      }
      radio.startListening();//começa a receber os dados de todos os dispositivos
}

void enviaTodos(char *mensagem){ //Envia mensagem para todos endereços
      radio.stopListening();
      for(int x=0; x<nro_dispositivos; x++){
          radio.openWritingPipe(trans[x]);//transmite para endereço especificado
          delay(5); // aguarda 5 ms
          radio.write(mensagem, strlen(mensagem)); //transmite mensagem
      }
      for(int y=0; y<nro_dispositivos; y++){
          radio.openReadingPipe(y, recep[y]);//transmite para endereço especificado
      }
      radio.startListening();// entra em listening
}

void enviaEspecifico(uint8_t disp_num, char *mensagem){
      radio.stopListening();
      radio.openWritingPipe(trans[disp_num]);
      delay(5);
      radio.write(mensagem, strlen(mensagem));
      modoRecepcao(); //coloca o módulo novamente em modo de recepção
}


/*void mudaEstado(bool flag){
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
*/
