#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define WIFI_SSID           "Forfs"
#define WIFI_SENHA          "toiao123"
#define MAX_WIFI_INIT_RETRY 10
#define WIFI_RETRY_DELAY    500
#define SERIAL_SPEED        115200

int WiFi_init(){  
    int retries = 0;
  
    Serial.print("Conectando ao WiFi ");
    Serial.print(WIFI_SSID);
    Serial.println("..........");
  
    WiFi.mode(WIFI_STA); //set wifi station mode
    
    WiFi.begin(WIFI_SSID, WIFI_SENHA); //start connecting to WiFi AP
    
    //check the status of WiFi connection to be WL_CONNECTED
    while ((WiFi.status() != WL_CONNECTED) && (retries < MAX_WIFI_INIT_RETRY)) {
           retries++;
           delay(WIFI_RETRY_DELAY);
           Serial.println("#");
    }
    Serial.println(String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "." + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3]));
  
    return WiFi.status(); //return the WiFi connection status
}

void setup() {
  Serial.begin(SERIAL_SPEED);

  if(WiFi_init() != WL_CONNECTED){
    Serial.print("Erro de conexão ao wifi ");
    Serial.print(WIFI_SSID);
    Serial.println("....\n");
  }
  else{
    Serial.println("Conexão WiFi ok....");
  } 
}

void loop() {
  //Não é necessário nada
}
