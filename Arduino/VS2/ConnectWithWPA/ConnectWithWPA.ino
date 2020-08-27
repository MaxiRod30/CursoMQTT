
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//********************VARIABLES***************************

char* ssid = "Maxi10";
char* password = "1234567.";

const char* mqtt_server = "iotdarkmaxvs.ga";
const int mqtt_port = 1883;
const char* mqtt_user = "web_client";
const char* mqtt_pass = "Q1w2e3r4t5";

WiFiClient espClient;
PubSubClient client (espClient);

long lastMsg = 0;
char msg[50];

int temp1 = 0;
int temp2 = 1;
int volt = 2;

//*******
//********************FUNCIONES***************************
void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Intentando conexion MQTT.... ");

    String clientId = "esp32_";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass))
    {
      Serial.println("Conectado!");
      client.subscribe("led1");
      client.subscribe("led2");
    }
    else
    {
      Serial.print("fallo: ( con error -> ");
      Serial.print(client.state());
      Serial.println(" Intentamos de nuevo en 5 segundos");
      delay(5000);
    }
    
  }
}
void callback (char* topic, byte* payload, unsigned int length)
{
  String incoming = "";
  Serial.print("Mensaje recibido desde -> ");
  Serial.println(topic);
  Serial.println("");
  for (int i = 0; i < length; i++)
  {
    incoming += (char)payload[i];
  }
  incoming.trim(); // Para limpiar el string, elimina los caracteres vacios

  Serial.println("Mensaje -> " + incoming);
  
  // Control de salida
  if (incoming == "on")
  {
    digitalWrite(BUILTIN_LED,HIGH);
  }
  else
  {
    digitalWrite(BUILTIN_LED,LOW);
  }

}

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");  
  Serial.println("Conectado a red Wifi!");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());

}
//*******
//********************SETUP***************************
void setup() 
{
  pinMode(BUILTIN_LED,OUTPUT);
  Serial.begin(115200);
  randomSeed(micros());
  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}
//*******
//********************MAIN***************************
void loop() 
{
  if (!client.connected())
  {
    reconnect();
  }
  
  client.loop();

  long now = millis();

  if (now - lastMsg > 500)
  {
    lastMsg = now;
    temp1++;
    temp2++;
    volt++;

    String to_send = String(temp1) + "," + String(temp2) + "," + String(volt);
    to_send.toCharArray(msg , 50);
    Serial.print("Publicamos mensaje -> ");
    Serial.println(msg);
    client.publish("values",msg);
  }
  

}
//*******
