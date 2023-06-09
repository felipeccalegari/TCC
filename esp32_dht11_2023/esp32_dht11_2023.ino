//Bibliotecas

#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

//Pino 4 - DHT11 <-> ESP32
#define DHTPIN 4

//Especificando qual sensor DHT é utilizado
#define DHTTYPE DHT22

//Credenciais Wifi
#define ssid "" // Wifi ssid
#define password "" //wifi password

//Credenciais MQTT
const char *mqtt_server = "broker.hivemq.com";
const char *mqtt_user = ""; //mqtt user
const char *mqtt_password = ""; //mqtt password
const int mqtt_port = 1883;

//Credenciais MQTT - Tópicos
const char *tempC = "dht/tempC";
const char *alarmeA = "dht/alarmeA";
const char *alarmeB = "dht/alarmeB";

//Instanciando objetos para wifi, mqtt e dht
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

//Função callback mqtt
void callback(char* topic, byte* payload, unsigned int length) {
  int i = 0;
  Serial.println("Message arrived:topic: " + String(topic));

}

void setup() {

  //Velocidade utilizada de baud rate para ESP32
  Serial.begin(115200);
  //Inicializa a conexão wifi
  WiFi.begin(ssid, password);

  //Verifica se foi conectado
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Conectando...");
  }
  Serial.println("Conectado a rede!");

  //Inicializa a conexão com MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  //Verifica se foi conectado
  while (!client.connected()) //Enquanto não estiver conectado ao broker MQTT
  {
    String client_id = "ESP32"; //client it mqtt
    client_id += String(WiFi.macAddress());
    Serial.printf("Client %s se conecta ao broker.\n", client_id.c_str());

    //Se for conectado com sucesso
    if (client.connect(client_id.c_str(), mqtt_user, mqtt_password))
    {
      Serial.println("Conectado ao broker!");
    }
    else //Se houver falha ao se conectar ao broker MQTT
    {
      Serial.print("Falha ao conectar com estado ");
      Serial.println(client.state());
      delay(1000);
    }
  }
  //Inicializa o sensor DHT11
  dht.begin();

}

//Declaração de variáveis

//Variáveis de alarme. Alta - 1, Baixa - 0
int alarmeAlta;
int alarmeBaixa;

//Variáveis para considerar qual é a temperatura alta e o que é considerado baixa
float tempAlarmeAlto = 23;
float tempAlarmeBaixo = 21;

void loop() {

  //Espera 5 segundos
  delay(5000);

  //Variável para armazenar a temperatura em Celsius
  float t = dht.readTemperature();
  
  //Verifica se há erro na leitura do sensor
  if (isnan(t)){
    Serial.println(F("Falha ao ler o sensor!"));
    return;
  }
  
  //Publica no tópico "dht/tempC" a temperatura atual
  client.publish(tempC, String(t).c_str());

  //Imprime no serial monitor a temperatura em Celsius
  Serial.print("Temperatura em Celsius: ");
  Serial.println(t);

  //Verificação para alarmes de temperatura Alta e Baixa

  //Alarme de temperatura alta
  if (t >= tempAlarmeAlto){

    Serial.println("Alarme de temperatura ALTA");
    //Altera os estados dos alarmes
    alarmeAlta = 1;
    alarmeBaixa = 0;
    //Publica nos tópicos "dht/alarmeA" e "dht/alarmeB" os estados dos alarmes alto e baixo, respectivamente
    client.publish(alarmeA, String(alarmeAlta).c_str());
    client.publish(alarmeB, String(alarmeBaixa).c_str());

  } else if (t <= tempAlarmeBaixo){ //Alarme de temperatura baixa

    Serial.println("Alarme de temperatura BAIXA");
    //Altera os estados dos alarmes
    alarmeAlta = 0;
    alarmeBaixa = 1;
    //Publica nos tópicos "dht/alarmeA" e "dht/alarmeB" os estados dos alarmes alto e baixo, respectivamente
    client.publish(alarmeA, String(alarmeAlta).c_str());
    client.publish(alarmeB, String(alarmeBaixa).c_str());

  } else { //Caso não for nem temperatura alta nem baixa, ou seja, o valor medido estará no intervalo do que é considerado nem alto e nem baixo
    //Altera os estados dos alarmes
    alarmeAlta = 0;
    alarmeBaixa = 0;
    //Publica nos tópicos "dht/alarmeA" e "dht/alarmeB" os estados dos alarmes alto e baixo, respectivamente
    client.publish(alarmeA, String(alarmeAlta).c_str());
    client.publish(alarmeB, String(alarmeBaixa).c_str());
  }
}
