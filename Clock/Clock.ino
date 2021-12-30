/*******************************************************************************
* Real Time Clock : Primeiros Passos (v1.0)
* 
* Exibicao das informacoes data, hora e dia da semana com Modulo RTC DS1307.
* 
* Copyright 2019 RoboCore.
* Escrito por Matheus Cassioli (29/04/2019).
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version (<https://www.gnu.org/licenses/>).
*******************************************************************************/
  
//Inclui as bibliotecas
#include <Wire.h>
#include "RTClib.h"

//As linhas de codigo a seguir devem ser comentadas, ou descomentadas, de acordo com o modelo de RTC utilizado (DS1307 ou DS3132)
RTC_DS1307 rtc; //Objeto rtc da classe DS1307
//RTC_DS3231 rtc; //Objeto rtc da classe DS3132
  
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"}; //Dias da semana

void setup () {
  Serial.begin(9600);                          //Inicializa a comunicacao serial
  if (! rtc.begin()) {                         //Se o RTC nao for inicializado, faz
    Serial.println("RTC NAO INICIALIZADO");    //Imprime o texto
    while (1);                                 //Trava o programa
  }
  //rtc.adjust(DateTime(2021, 3, 6, 15, 24, 00));//Ajusta o tempo do RTC para a data e hora definida pelo usuario.
  delay(100);                                  // 100 Milissegundos
}

void loop () {
  DateTime agora = rtc.now();                             // Faz a leitura de dados de data e hora
  //Serial.print("Data: ");
  //Serial.print(agora.day(),DEC);                         //Imprime dia
  //Serial.print('/');                                      //Imprime barra
  //Serial.print(agora.month(), DEC);                       //Imprime mes
  Serial.print('/');                                      //Imprime barra
  Serial.print(agora.year(), DEC);                        //Imprime ano
  /*Serial.print(" / Dia da semana: ");                     //Imprime texto
  Serial.print(diasDaSemana[agora.dayOfTheWeek()]);       //Imprime dia da semana
  Serial.print(" / Horas: ");                             //Imprime texto
  Serial.print(agora.hour(), DEC);                        //Imprime hora
  Serial.print(':');                                      //Imprime dois pontos
  Serial.print(agora.minute(), DEC);                      //Imprime os minutos
  Serial.print(':');                                      //Imprime dois pontos
  Serial.print(agora.second(), DEC);*/                      //Imprime os segundos
  Serial.println();                                      //Quebra de linha
  delay(1000);                                            // 1 Segundo
}
