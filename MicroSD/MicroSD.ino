//Programa: Teste modulo cartao micro SD Arduino
//Autor: Arduino e Cia

#include <SPI.h>
#include <SD.h>

//Pino de conexão do pino CS do modulo
const int chipSelect = 4;

void setup()
{
  Serial.begin(9600);

  Serial.println("Arduino e Cia - Teste de Modulo cartao micro SD");
  Serial.println("Inicializando cartao SD...");

  //Inicia a comunicacao com o modulo SD
  if (!SD.begin(chipSelect))
  {
    Serial.println("Falha ao acessar o cartao !");
    Serial.println("Verifique o cartao/conexoes e reinicie o Arduino...");
    return;
  }
  Serial.println("Cartao iniciado corretamente !");
  Serial.println();
}

void loop()
{
  unsigned long currentMillis = millis();
  
  //Mostra os dados no Serial Monitor
  Serial.print("Este arduino esta ligado a ");
  Serial.print(currentMillis / 1000);
  Serial.println(" segundos");

  //Abre o arquivo datalog.txt
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  //Grava as informacoes no arquivo
  if (dataFile)
  {
    dataFile.print("Este arduino esta ligado a ");
    dataFile.print(currentMillis / 1000);
    dataFile.println(" segundos");

    //Fecha o arquivo
    dataFile.close();
  }

  //Exibe um erro se nao conseguir abrir o arquivo
  else
  {
    Serial.println("Erro ao abrir o arquivo datalog.txt");
  }
  delay(2000);
}
