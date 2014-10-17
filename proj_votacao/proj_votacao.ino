#include "teclado.h"
#include "radio.h"
#include <Keypad.h>
#include <stdio.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

char strTmp[20]="";//define um ponteiro temporario pra armazenar str
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
    //zera();
}
// ******************** função loop ****************************************************
void loop(){
  tam = 0;
  int len = 0;
  char temp = str[0];
  if (temp){
      zera();
  }
  if (radio.available()){
        bool done = false;
        while(!done){
          len = radio.getDynamicPayloadSize();
          tam = tam + len;
          done = radio.read(&str, len);//faz a leitura do radio
          delay(5);
        }
        Serial.println(str);
        tam++;
        Serial.println(tam);
        str[tam] = '\0';
  } //fim_while
    if (tam) {
        compara();
    }
    delay(80);// aguarda 80 ms
//*************   fim radio.available()     ********************************************
}//fim loop******************************************************************************

bool comparaString(char *rec1, char *rec2 ){ //rec1 = str ||| rec2 = votacao etc ...
    Serial.print("comparaString = ");
    int contador=0;
    bool sinalizador = false;
    for(int v = 0; v < tam; v++){
        if(rec1[v]==rec2[v]){
           contador++;
        }
    }
    Serial.println(contador);
    if(contador == tam){
          sinalizador = true;
    }
    return sinalizador;
}

void zera(){
      for(int a = 0; a < 20; a++){
          //strTmp[a]=0;//define um ponteiro temporario pra armazenar str
          str[a]=0;
          //senha[a]=0;
      }
}

void compara(){
       if( comparaString(str, presenca) ){
          delay(80);
          Serial.println("Digite a Senha:");
          recebeTeclado();
          Serial.println(vetor);
          zera();
          mudaEstado(false);//muda o estado para transmissao
          //bool done = false;
          //int cont = 0;
          //while(!done && cont<3){
           bool done =  radio.write(&vetor, strlen(vetor));
           Serial.println(done);
          //    delay(80);
          //    cont++;
          //}
          mudaEstado(true);
      // fim presenca ******************************************************************
      } /*else if( comparaString(str, votacao)){
          //strTmp = str;
          Serial.println("VOTE:1>SIM 2>NAO");
          recebeTeclado();
          Serial.println(vetor);
          //mudaEstado(false);//muda o estado para transmissao
          //bool done = false;
          //bool done1 = false;
          //while(!done){
            //  done =  radio.write(voto, sizeof(voto));
          //} // envia voto
          //while (!done1){
          //    done1 = radio.write(senha, sizeof(senha));
          //} // envia senha
          //mudaEstado(true);
      // fim votacao ******************************************************************
      } else if( comparaString(str, confirmacao) ){
          if(strcmp(strTmp, "presenca")==0){
              Serial.println("Presenca confirmada!");
              //delete strTmp;
          }
          if (strcmp(strTmp, "votacao")==0){
              Serial.println("Voto Computado");
              //delete strTmp;
          }
      }*/
      //fim do if confirmação *********************************************************

}

/*
Este codigo ainda é experimental ainda sao necessarios testes e implementações
adicionais que ainda não estao aqui
*/
