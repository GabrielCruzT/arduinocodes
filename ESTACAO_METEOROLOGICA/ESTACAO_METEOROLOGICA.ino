#include <Wire.h> 
#include <SPI.h>
#include <DHT.h>  
#include <SD.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_BMP280.h> 
#include "RTClib.h"
#include "SSD1306Wire.h"
#define DHTPIN 0          //pin where the dht11 is connected

String apiKey = "EXMS5KK94852P64T";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "GABRIEL";     // replace with your wifi ssid and wpa2 key
const char *pass =  "32730444";
const char* server = "api.thingspeak.com";

const int chipSelect = 15;

char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado" }; //Dias da semana

DHT dht(DHTPIN, DHT11);

Adafruit_BMP280 bmp; //OBJETO DO TIPO Adafruit_BMP280 (I2C)
RTC_DS3231 rtc; //Objeto rtc da classe DS3132

/*//Define Portas
#define SCL 4 
#define SDA 5
SSD1306Wire  display(0x3c, SCL, SDA);*/

WiFiClient client;

void setup() 
{
  Serial.begin(115200);                          //Inicializa a comunicacao serial

  //Rotina de inicio do sensor BMP 280
  if(!bmp.begin(0x76))                         //SE O SENSOR NÃO FOR INICIALIZADO NO ENDEREÇO I2C 0x76, FAZ
  { 
    Serial.println(F("Sensor BMP280 não foi identificado! Verifique as conexões.")); 
    while(1); 
  }
  
  //Rotina de inicio do Clock
  if (! rtc.begin())                           //Se o RTC nao for inicializado, faz
  {                         
    Serial.println("RTC NAO INICIALIZADO");    //Imprime o texto
    while (1);                                 //Trava o programa
  }

  if (!SD.begin(chipSelect))
  {
    Serial.println("Falha ao acessar o cartao !");
    Serial.println("Verifique o cartao/conexoes e reinicie o Arduino...");
    return;
  }
  Serial.println("Cartao iniciado corretamente !");
  Serial.println();
  
  //rtc.adjust(DateTime(2022, 1, 11, 17, 29, 00)); //Ajuste de data

  //display.init(); //Inicia o display
  //display.flipScreenVertically(); //Inverte a tela

  dht.begin();
  delay(100);                                  // 100 Milissegundos

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  recover();
}

void escrita()
{
  DateTime agora = rtc.now();                               // Faz a leitura de dados de data e hora
  Serial.print(F("Temperatura: ")); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(bmp.readTemperature()); //IMPRIME NO MONITOR SERIAL A TEMPERATURA
  Serial.println(" *C (Grau Celsius)"); //IMPRIME O TEXTO NO MONITOR SERIAL

  Serial.print(F("Pressão: ")); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(bmp.readPressure()); //IMPRIME NO MONITOR SERIAL A PRESSÃO
  Serial.println(" Pa (Pascal)"); //IMPRIME O TEXTO NO MONITOR SERIAL

  Serial.print(F("Altitude aprox.: ")); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(bmp.readAltitude(1013.25),0); //IMPRIME NO MONITOR SERIAL A ALTITUDE APROXIMADA (Valor da pressão ao nível do mar)
  Serial.println(" m (Metros)"); //IMPRIME O TEXTO NO MONITOR SERIAL
    
  Serial.print("Data: ");
  Serial.print(agora.day(),DEC);                         //Imprime dia
  Serial.print('/');                                      //Imprime barra
  Serial.print(agora.month(), DEC);                       //Imprime mes
  Serial.print('/');                                      //Imprime barra
  Serial.print(agora.year(), DEC);                        //Imprime ano
  
  Serial.print(" / Dia da semana: ");                     //Imprime texto
  Serial.print(diasDaSemana[agora.dayOfTheWeek()]);       //Imprime dia da semana
  
  Serial.print(" / Horas: ");                             //Imprime texto
  Serial.print(agora.hour(), DEC);                        //Imprime hora
  Serial.print(':');                                      //Imprime dois pontos
  Serial.print(agora.minute(), DEC);                      //Imprime os minutos
  Serial.print(':');                                      //Imprime dois pontos
  Serial.println(agora.second(), DEC);                      //Imprime os segundos
 
  Serial.println("-----------------------------------"); //IMPRIME UMA LINHA NO MONITOR SERIAL
  delay(1000);                                            // 1 Segundo
  
}

void draw()  //Escrita no Display
{
  /*display.setFont(ArialMT_Plain_10); //Define fonte e tamanho
  DateTime agora = rtc.now();                               // Faz a leitura de dados de data e hora
  
  //Temperatura 1-Superior
  display.drawString( 2, 1, String(bmp.readTemperature()));
  display.drawCircle(30,4,1);
  display.drawString(33, 1, "C");
  display.drawRect(0,0, 45, 15);
  
  //Umidade 2-Superior 
  display.drawString(60,1,String(bmp.readPressure()));
  display.drawString( 110, 1, "Pa");
  display.drawRect(57,0, 70, 15);

  //Altura 1-Inferor
  display.drawString( 2, 20, String(bmp.readAltitude(1013.25),0));
  display.drawString( 20, 20, "m");

  //Outro 2-Inferior
  display.drawString( 2, 30, "70");
  display.drawString( 20, 30, "%");

  //DATA
  display.drawString( 40, 50, String(agora.day()));
  display.drawString( 50, 50, "/");
  display.drawString( 60, 50, String(agora.month()));
  display.drawString( 70, 50, "/");
  display.drawString( 80, 50, String(agora.year()));
  
  
  //Hora
  display.setFont(ArialMT_Plain_24);
  display.drawString( 53, 20, String(agora.hour()));
  display.drawString( 57, 20, ":");
  display.drawString( 61, 20, String(agora.minute()));
  //moldura relogio
  display.drawRect(48, 20, 70, 30);*/
}

void send_thingspeak()
{
  DateTime agora = rtc.now();
  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
  {                              
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(dht.readTemperature());
    postStr +="&field2=";
    postStr += String(dht.readHumidity());
    postStr +="&field3=";
    postStr += String(bmp.readTemperature());
    postStr +="&field4=";
    postStr += String(bmp.readAltitude(1013.25),0);
    postStr +="&field5=";
    postStr += String(bmp.readPressure());
    postStr += "\r\n\r\n";
     
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

  }
  client.stop();                                           // thingspeak needs minimum 15 sec delay between updates
  Serial.println("Waiting...");
  
  delay(1000);
  if (agora.hour()== 0 ||agora.hour()== 4 ||agora.hour()== 8 || agora.hour()== 12 || agora.hour()== 16 || agora.hour()== 20)
  {
    if(agora.minute() == 50 && agora.second() < 10)
      recover();
  }
    
}

void recover()
{
  DateTime agora = rtc.now();                               // Faz a leitura de dados de data e hora
  
  File dataFile = SD.open("datalog.csv", FILE_WRITE);       //Abre o arquivo datalog.csv

  //Gravação das informacoes no arquivo
  if (dataFile)
  {
    dataFile.print("Data: ");
    dataFile.print(";");
    dataFile.print(agora.day(),DEC);
    dataFile.print("/");
    dataFile.print(agora.month(),DEC);
    dataFile.print("/");
    dataFile.print(agora.year(),DEC);
    dataFile.print(";");

    dataFile.print("Horario: ");
    dataFile.print(";");
    dataFile.print(agora.hour(), DEC);
    dataFile.print(":");
    dataFile.print(agora.minute(), DEC);
    dataFile.print(":");
    dataFile.print(agora.second(), DEC);
    dataFile.print(";");

    dataFile.print("Temperatura interna: ");
    dataFile.print(";");
    dataFile.print(bmp.readTemperature());
    dataFile.print(";");
    dataFile.print("Temperatura externa: ");
    dataFile.print(";");
    dataFile.print(dht.readTemperature());
    dataFile.print(";");
    dataFile.print("Umidade: ");
    dataFile.print(";");
    dataFile.print(dht.readHumidity());
    dataFile.print(";");
    dataFile.print("Pressao: ");
    dataFile.print(";");
    dataFile.print(bmp.readPressure());
    dataFile.print(";");
    dataFile.print("Altitude: ");
    dataFile.print(";");
    dataFile.print(bmp.readAltitude(1013.25),0);
    dataFile.println();
   
    dataFile.close();                                         //Fecha o arquivo
  }
 
  else
  {
    Serial.println("Erro ao abrir o arquivo datalog.txt");    //Exibe um erro se nao conseguir abrir o arquivo
  }
}

void loop() 
{
  escrita();
  send_thingspeak();
  
  /*//Rotina de dados no display
  display.clear();
  draw();
  display.display(); 
  delay(10);*/
}
