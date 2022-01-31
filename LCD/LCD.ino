#include "U8glib.h"
 
U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE);
 
void draw() 
{
  u8g.setFont(u8g_font_8x13B);
  
  //Temperatura 1-Superior
  u8g.drawStr( 5, 12, "23");
  u8g.drawCircle(25,4,3);
  u8g.drawStr( 30, 12, "C");
  u8g.drawRFrame(0,0, 40, 14, 4);
  
  //Umidade 2-Superior 
  u8g.drawStr(50,12,"100");
  u8g.drawStr( 75, 12, "%");
  u8g.drawRFrame(45,0, 40, 14, 4);

  //Pressão 3-Superior
  u8g.drawStr(92,12,"100");
  u8g.drawStr( 117, 12, "%");
  u8g.drawRFrame(90,0, 37, 14, 4);

  //Altura 1-Inferor
  u8g.drawStr( 5, 32, "23");
  u8g.drawStr( 30, 32, "C");
  //u8g.drawRFrame(0,20, 40, 14, 4);

  //Outro 2-Inferior
  u8g.drawStr( 5, 52, "23");
  u8g.drawStr( 30, 52, "C");
  //u8g.drawRFrame(0,40, 40, 14, 4);
  
  //Hora
  u8g.setFont(u8g_font_fub20);
  u8g.drawStr( 55, 45, "09:35");
  //moldura relogio
  u8g.drawRFrame(52,20, 75, 30, 4);
}
 
void setup(void) 
{
  Serial.begin(9600);
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}
 
void loop(void) 
{
  u8g.firstPage();  
  do
  {
    draw();
  } while( u8g.nextPage() );
   
  delay(50);
}
