#include "DHT.h"
#include <stdlib.h>

/***** Definições de variaveis *****/

#define DHT_PIN01 15

#define DHT_PIN02 18

#define DHTTYPE DHT11

DHT dht01(DHT_PIN01, DHTTYPE);

DHT dht02(DHT_PIN02, DHTTYPE);

void imprimeSerial(float temperatura01, float temperatura02, float humidade01, float humidade02, float variacaoTemperaturaSensores, float variacaoHumidadeSensores, float temperaturaMedia, float humidadeMedia, float hic01, float hic02, bool temperaturaDiscrepante, bool humidadeDiscrepante);

void setup()
{

  Serial.begin(9600);

  Serial.println(F("DHTxx test!"));

  dht01.begin();

  dht02.begin();
}

void loop()
{
  // Leitura da humidade dos sensores

  float humidade01 = dht01.readHumidity();

  float humidade02 = dht02.readHumidity();

  // Leitura da temperatura em Celcius

  float temperatura01 = dht01.readTemperature();

  float temperatura02 = dht02.readTemperature();

  // Calculo da media das netricas e a variação entre sensires
  float variacaoTemperaturaSensores = abs(temperatura01 - temperatura02);

  float variacaoHumidadeSensores = abs(humidade01 - humidade02);

  float temperaturaMedia = (temperatura01 + temperatura02) / 2;

  float humidadeMedia = (humidade01 + humidade02) / 2;

  // Checa se alguma das leituras do sensor 01 falhou e tenta novamente

  if (isnan(humidade01) || isnan(temperatura01))
  {
    Serial.println(F("Falha ao ler as informações do sensor 01"));
    return;
  }

  // Checa se alguma das leituras do sensor 02 falhou e tenta novamente

  if (isnan(humidade02) || isnan(temperatura02))
  {
    Serial.println(F("Falha ao ler as informações do sensor 02"));
    return;
  }

  // Capta a sensação termica

  float hic01 = dht01.computeHeatIndex(temperatura01, humidade01, false);

  float hic02 = dht01.computeHeatIndex(temperatura02, humidade02, false);

  // Checa se temperatura ou humidade estão discrepantes

  bool temperaturaDiscrepante = variacaoTemperaturaSensores > 5;

  bool humidadeDiscrepante = variacaoHumidadeSensores > 10;

  imprimeSerial(temperatura01, temperatura02, humidade01, humidade02, variacaoTemperaturaSensores, variacaoHumidadeSensores, temperaturaMedia, humidadeMedia, hic01, hic02, temperaturaDiscrepante, humidadeDiscrepante);

  delay(5000);
}

void imprimeSerial(float temperatura01, float temperatura02, float humidade01, float humidade02, float variacaoTemperaturaSensores, float variacaoHumidadeSensores, float temperaturaMedia, float humidadeMedia, float hic01, float hic02, bool temperaturaDiscrepante, bool humidadeDiscrepante)
{
  Serial.println("Sensor 01: ");
  Serial.print(F("Humidade: "));
  Serial.print(humidade01);

  Serial.print(F("%  Temperatura: "));
  Serial.print(temperatura01);
  Serial.print(F("°C"));

  Serial.print(F("Sensação termica: "));
  Serial.print(hic01);
  Serial.println(F("°C "));
  Serial.println("");

  Serial.println("Sensor 02: ");
  Serial.print(F("Humidade: "));
  Serial.print(humidade02);

  Serial.print(F("%  Temperatura: "));
  Serial.print(temperatura02);
  Serial.print(F("°C"));

  Serial.print(F("Sensação termica: "));
  Serial.print(hic02);
  Serial.println(F("°C "));
  Serial.println("");

  Serial.println("******* Variação das metricas ********");
  Serial.print(F("Variação de temperatura dos sensores: "));
  Serial.print(variacaoTemperaturaSensores);
  Serial.println(F("°C ,"));
  if (temperaturaDiscrepante)
    Serial.println(F("Sensores com temperaturas muito discrepantes!"));
  Serial.println("");

  Serial.print(F("Variação da humidade dos sensores: "));
  Serial.println(variacaoHumidadeSensores);
  if (humidadeDiscrepante)
    Serial.println(F("Sensores com humidades muito discrepantes!"));
  Serial.println("");

  Serial.println("******* Temperatura e humidade media ********");
  Serial.print(F("Temperatura media da sala: "));
  Serial.println(temperaturaMedia);
  Serial.println("");

  Serial.print(F("Humidade media da sala: "));
  Serial.println(humidadeMedia);
  Serial.println("------------------------------------------------------------");
}