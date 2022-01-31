
#include <Wire.h>  
#include "SSD1306Wire.h"

//Define Portas
#define SCL 4//12 
#define SDA 5//13

SSD1306Wire  display(0x3c, SCL, SDA);

void setup() 
{
  Serial.begin(9600);
  display.init(); //Inicia o display
  display.flipScreenVertically(); //Inverte a tela
}

void draw() 
{
  display.setFont(ArialMT_Plain_10); //Define fonte e tamanho
  
  //Temperatura 1-Superior
  display.drawString( 2, 1, "23");
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
  display.drawString( 2, 20, "23");
  display.drawString( 20, 20, "C");

  //Outro 2-Inferior
  display.drawString( 2, 30, "23");
  display.drawString( 20, 30, "C");

  //Outro 3-Inferior
  display.drawString( 2, 40, "23");
  display.drawString( 20, 40, "C");
  
  //Hora
  display.setFont(ArialMT_Plain_24);
  display.drawString( 53, 20, "09:35");
  //moldura relogio
  display.drawRect(48, 20, 70, 30);
}

void loop() 
{
  //Atualização do display
  draw();
  display.display(); //
  delay(10);
}
