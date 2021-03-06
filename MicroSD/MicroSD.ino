#include <SPI.h>
#include <SD.h>

//Pino de conexão do pino CS do modulo
const int chipSelect = 4;

void setup()
{
  Serial.begin(9600);
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
  //Mostra os dados no Serial Monitor
  Serial.println("Leitura do dia: ");

  //Abre o arquivo datalog.txt
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  //Grava as informacoes no arquivo
  if (dataFile)
  {
    dataFile.print("Leitura do dia: " );
    dataFile.print("Umidade (%): " );
    dataFile.print("Temperatura (ºC): " );
    dataFile.print("Pressao (Pa):" );
    dataFile.print("Altitude: " );
    dataFile.println("Horario: " );
    
    //Fecha o arquivo
    dataFile.close();
  }

  //Exibe um erro se nao conseguir abrir o arquivo
  else
  {
    Serial.println("Erro ao abrir o arquivo datalog.txt");
  }
  //delay(2000);
}
