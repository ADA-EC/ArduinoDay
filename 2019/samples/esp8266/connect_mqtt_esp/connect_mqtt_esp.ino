#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <PubSubClient.h>

//WiFi
#define WIFI_SSID           "Forfs"
#define WIFI_SENHA          "toiao123"
#define MAX_WIFI_INIT_RETRY 10
#define WIFI_RETRY_DELAY    500

//Serial
#define SERIAL_SPEED        115200

//MQTT
#define MQTT_CLIENT_ID      "ADA"
#define MQTT_SERVER         "m11.cloudmqtt.com" //MQTT broker sever. I use https://www.cloudmqtt.com/
#define MQTT_UNAME          "zwrbsgco"     //MQTT broker user name - I use this broker https://www.cloudmqtt.com/
#define MQTT_PASSW          "Gu19Jq6j-ruy" //MQTT broker password
#define MQTT_BROKER_PORT    18002          //MQTT BROKER listening port
#define MQTT_TOPIC          "ADA/ArduinoDay"

#define ESP_NAME            "ADA-ESP8266"
//#define ESP_PUB_ROLE
#define ESP_SUB_ROLE

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client, MQTT_SERVER, MQTT_BROKER_PORT);
bool mqtt_status;

//Wifi Initialization function
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

//MQTT callback function invoked for every MQTT received message on a subscribed topic
void mqtt_callback(const MQTT::Publish& pub){
      Serial.println("MQTT receiving a message:");
      Serial.println(pub.payload_string());
      Serial.println();
}

int MQTT_init(boolean topic_subscribe){
        Serial.println("Initializing MQTT communication.........");
               
        mqtt_client.set_callback(mqtt_callback); //set callback on received messages
        mqtt_client.set_max_retries(255);
        
        //here we connect to MQTT broker and we increase the keepalive for more reliability
        if (mqtt_client.connect(MQTT::Connect(MQTT_CLIENT_ID).set_keepalive(120000).set_auth(String(MQTT_UNAME), String(MQTT_PASSW)))) {
            Serial.println("Connection to MQTT broker SUCCESS..........");
            
            //if role is SUB subscribe to topic
            if (topic_subscribe) {
                if (mqtt_client.subscribe(MQTT_TOPIC)) {
                        Serial.println("Subscription to MQTT topic [" + String(MQTT_TOPIC) + "] SUCCESS.........");
                } else {
                        Serial.println("MQTT unable to subscribe to [" + String(MQTT_TOPIC) + "] ERROR.........");
                        mqtt_client.disconnect();
                        return false;
                }
            }
        } else {
            Serial.println("Connection to MQTT broker ERROR..........");
        }
        
        return mqtt_client.connected();
}

void setup() {
    Serial.begin(SERIAL_SPEED);
    delay(100);

    Serial.println();
    Serial.println("MQTT_basic starting....");

    int i;
    if (WiFi_init() != WL_CONNECTED) {
        Serial.print("WiFi connection ERROR to ");
        Serial.print(WIFI_SSID);
        Serial.println("....\n");
    } else {
        Serial.println("WiFi connection OK....");
        #ifdef ESP_PUB_ROLE
            mqtt_status = MQTT_init(false); //if ESP is Publisher do not subscribe to topic
        #endif
            
        #ifdef ESP_SUB_ROLE
            mqtt_status = MQTT_init(true); //if ESP is Subscriber do subscribe to topic
        #endif

        if (!mqtt_status){
            Serial.println("MQTT connection ERROR....");
        }
        else{
            Serial.println("MQTT connection OK....");
        }     
    }
}

void loop() {
    if (mqtt_status) {
         #ifdef ESP_PUB_ROLE 
             mqtt_client.publish(MQTT_TOPIC, "Hello I am ESP-" + String(ESP_NAME));
             Serial.println("MQTT message sent....");
             delay(3000);
         #endif
         
         #ifdef ESP_SUB_ROLE
             mqtt_client.loop();
             delay(100);
         #endif
     }

}
