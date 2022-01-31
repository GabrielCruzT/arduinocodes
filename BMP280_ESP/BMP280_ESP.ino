#include <Wire.h> 
#include <SPI.h>
#include <Adafruit_Sensor.h> //INCLUSÃO DE BIBLIOTECA
#include <Adafruit_BMP280.h> //INCLUSÃO DE BIBLIOTECA
#include "SSD1306Wire.h"
Adafruit_BMP280 bmp; //OBJETO DO TIPO Adafruit_BMP280 (I2C)

//Define Portas
#define SCL 4 
#define SDA 5

SSD1306Wire  display(0x3c, SCL, SDA);

void setup(){
  Serial.begin(9600); //INICIALIZA A SERIAL
  if(!bmp.begin(0x76)){ //SE O SENSOR NÃO FOR INICIALIZADO NO ENDEREÇO I2C 0x76, FAZ
    Serial.println(F("Sensor BMP280 não foi identificado! Verifique as conexões.")); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
  display.init(); //Inicia o display
  display.flipScreenVertically(); //Inverte a tela
}
void draw() 
{
  display.setFont(ArialMT_Plain_10); //Define fonte e tamanho
  
  //Temperatura 1-Superior
  display.drawString( 2, 1, String(bmp.readTemperature()));
  display.drawCircle(15,4,1);
  display.drawString(20, 1, "C");
  display.drawRect(0,0, 30, 15);
  
  //Umidade 2-Superior 
  display.drawString(45,1,"100");
  display.drawString( 62, 1, "%");
  display.drawRect(43,0, 32, 15);

  //Pressão 3-Superior
  display.drawString(92,1,"100");
  display.drawString( 109, 1, "%");
  display.drawRect(90,0, 32, 15);

  //Altura 1-Inferor
  display.drawString( 2, 20, String(bmp.readAltitude(1013.25),0));
  display.drawString( 20, 20, "m");

  //Outro 2-Inferior
  display.drawString( 2, 30, String(bmp.readPressure()));
  display.drawString( 20, 30, "Pa");

  //Outro 3-Inferior
  display.drawString( 2, 40, "23");
  display.drawString( 20, 40, "C");
  
  //Hora
  display.setFont(ArialMT_Plain_24);
  display.drawString( 53, 20, "09:35");
  //moldura relogio
  display.drawRect(48, 20, 70, 30);

  Serial.print(F("Temperatura: ")); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(bmp.readTemperature()); //IMPRIME NO MONITOR SERIAL A TEMPERATURA
  Serial.println(" *C (Grau Celsius)"); //IMPRIME O TEXTO NO MONITOR SERIAL

  Serial.print(F("Pressão: ")); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(bmp.readPressure()); //IMPRIME NO MONITOR SERIAL A PRESSÃO
  Serial.println(" Pa (Pascal)"); //IMPRIME O TEXTO NO MONITOR SERIAL

  Serial.print(F("Altitude aprox.: ")); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(bmp.readAltitude(1013.25),0); //IMPRIME NO MONITOR SERIAL A ALTITUDE APROXIMADA (Valor da pressão ao nível do mar)
  Serial.println(" m (Metros)"); //IMPRIME O TEXTO NO MONITOR SERIAL
  
  Serial.println("-----------------------------------"); //IMPRIME UMA LINHA NO MONITOR SERIAL
  delay(2000); //INTERVALO DE 2 SEGUNDOS
}
void loop()
{
  draw();
  display.display(); //
  delay(10);
}
