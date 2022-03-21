#include <Wire.h> 
#include <SPI.h>
#include <DHT.h>  
#include <SD.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_BMP280.h> 
#include "RTClib.h"
#include "SSD1306Wire.h"

#define DHTPIN 0          //Define o pino do sensor de umidade

String apiKey = "EXMS5KK94852P64T";     // Codigo gerado pelo thingspeak para acessar o projeto
const char *ssid =  "GABRIEL";     // Nome da rede e senha para conectar o ESP
const char *pass =  "32730444";
const char* server = "api.thingspeak.com"; // Site que vai ser acessado

const int chipSelect = 15;

char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado" };

DHT dht(DHTPIN, DHT11);

Adafruit_BMP280 bmp; 
RTC_DS3231 rtc; 

WiFiClient client;

void setup() 
{
  Serial.begin(115200);                          //Inicializa a comunicacao serial

  //Rotina de inicio do sensor BMP 280
  if(!bmp.begin(0x76))                         
  { 
    Serial.println(F("Sensor BMP280 não foi identificado! Verifique as conexões.")); 
    while(1);                                    //Trava o programa ate resolver o problema
  }
  
  //Rotina de inicio do Clock
  if (! rtc.begin())                           
  {                         
    Serial.println("RTC NAO INICIALIZADO");    
    while (1);                                 
  }

  if (!SD.begin(chipSelect))
  {
    Serial.println("Falha ao acessar o cartao !");
    Serial.println("Verifique o cartao/conexoes e reinicie o Arduino...");
    return;
  }
  Serial.println("Cartao iniciado corretamente !");
  Serial.println();

  dht.begin();
  delay(100);                                  

  Serial.println("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");

  recover();
}

void write()
{
  DateTime agora = rtc.now();                               
  Serial.print(F("Temperatura: "));                         
  Serial.print(bmp.readTemperature());                      
  Serial.println(" *C (Grau Celsius)");                     

  Serial.print(F("Pressão: "));                             
  Serial.print(bmp.readPressure());                         
  Serial.println(" Pa (Pascal)");                           

  Serial.print(F("Altitude aprox.: "));                     
  Serial.print(bmp.readAltitude(1013.25),0);                
  Serial.println(" m (Metros)");                            
    
  Serial.print("Data: ");
  Serial.print(agora.day(),DEC);                           
  Serial.print('/');                                       
  Serial.print(agora.month(), DEC);                         
  Serial.print('/');                                        
  Serial.print(agora.year(), DEC);                         
  
  Serial.print(" / Dia da semana: ");                       
  Serial.print(diasDaSemana[agora.dayOfTheWeek()]);         
  
  Serial.print(" / Horas: ");                               
  Serial.print(agora.hour(), DEC);                         
  Serial.print(':');                                       
  Serial.print(agora.minute(), DEC);                        
  Serial.print(':');                                       
  Serial.println(agora.second(), DEC);                      
 
  Serial.println("-----------------------------------"); 
  delay(1000);                                            
}

void send_thingspeak()
{
  DateTime agora = rtc.now();
  if (client.connect(server))                            
  {                              
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(dht.readTemperature());
    postStr +="&field2=";
    postStr += String(dht.readHumidity());
    postStr +="&field3=";
    postStr += String(bmp.readTemperature());
    postStr +="&field4=";
    postStr += String(bmp.readAltitude(1013.25),0);       //(1013.25) Pressão atmosférica no nivel do mar
    postStr +="&field5=";
    postStr += String(bmp.readPressure());
    postStr += "\r\n\r\n";

  }
  client.stop();                                           
  
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
  if (dataFile)                                             //Se abrir o leitor micro SD
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
    dataFile.print("Pressao: ");
    dataFile.print(";");
    dataFile.print(bmp.readPressure());
    dataFile.print(";");
    dataFile.print("Altitude: ");
    dataFile.print(";");
    dataFile.print(bmp.readAltitude(1013.25),0);              //(1013.25) Pressão atmosférica no nivel do mar 
    dataFile.print(";");

    dataFile.print("Temperatura externa: ");
    dataFile.print(";");
    dataFile.print(dht.readTemperature());
    dataFile.print(";");
    dataFile.print("Umidade: ");
    dataFile.print(";");
    dataFile.print(dht.readHumidity());
    dataFile.println();
    dataFile.close();                                         //Fecha o arquivo
  }
 
  else
  {
    Serial.println("Erro ao abrir o arquivo datalog.csv");    //Exibe um erro se nao conseguir abrir o arquivo
  }
}

void loop() 
{
  write();
  send_thingspeak();
}
