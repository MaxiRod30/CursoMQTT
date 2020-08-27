#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//********************VARIABLES***************************
const String serial_number = "12345678";

char* ssid = "Maxi10";
char* password = "1234567.";

const char* mqtt_server = "iotdarkmaxvs.ga";
const int mqtt_port = 1883;
const char* mqtt_user = "web_client";
const char* mqtt_pass = "Q1w2e3r4t5..";

WiFiClient espClient;
PubSubClient client (espClient);

long lastMsg = 0;
char msg[50];

float temp1 = 0;


//*******
//********************FUNCIONES***************************

//***********************FUNCION PARA ESCALAR***********************

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//***********************FUNCION RECONECTAR***********************
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

      char topic[25];
     	String topic_aux = serial_number + "/command";
			topic_aux.toCharArray(topic,25);

      client.subscribe(topic);
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

//***********************FUNCION CALLBACK***********************

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
  if (incoming == "ON_OUT1")
  {
    digitalWrite(D5,HIGH);
  }
  else if(incoming == "OFF_OUT1")
  {
    digitalWrite(D5,LOW);
  }
  // Control de salida
  if (incoming == "ON_OUT2")
  {
    digitalWrite(D6,HIGH);
  }
  else if(incoming == "OFF_OUT2")
  {
    digitalWrite(D6,LOW);
  }
  if (incoming == "ON_OUT3")
  {
    digitalWrite(D7,HIGH);
  }
  else if(incoming == "OFF_OUT3")
  {
    digitalWrite(D7,LOW);
  }

}
//***********************SETUP WIFI***********************
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
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);
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
    
    temp1 = fmap(analogRead(A0), 0, 1024, 0, 100);

    String to_send = String(temp1);
    to_send.toCharArray(msg , 50);
    Serial.print("Publicamos mensaje -> ");
    Serial.println(msg);
    char topic2[25];
    String topic2_aux = serial_number + "/temps";
		topic2_aux.toCharArray(topic2,25);
    
    client.publish(topic2,msg);
  }
  

}
//*******
