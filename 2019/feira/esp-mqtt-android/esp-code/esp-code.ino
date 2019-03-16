#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <PubSubClient.h>

//----SERIAL CONFIG ----
#define SERIAL_SPEED        115200

//----WIFI CONFIG ----
#define WIFI_SSID           "tessarro" //your Wifi SSID
#define WIFI_PASSWD         "64812536" //your wifi password
#define MAX_WIFI_INIT_RETRY 10
#define WIFI_RETRY_DELAY    500

/* SSIDs
 *  
 * Forfs              toiao123 
 * iPhone de Samira   123456
 * tessarro           64812536
 * Marilene Garcia    mari123456
 * 
 */

char ssids[][50] = {
      {"Noel"}, {"ada12345"},
      {"Forfs"}, {"toiao123"},
      {"iPhone de Samira"}, {"123456"},
      {"tessarro"}, {"64812536"},
      {"Marilene Garcia"}, {"mari123456"},
      {"Moto"}, {"apotena2"},
};

int numssid = 12;

//----MQTT CONFIG ----
#define MQTT_CLIENT_ID      "ADA-GSR"
#define MQTT_SERVER         "m11.cloudmqtt.com" //MQTT broker sever. I use https://www.cloudmqtt.com/
#define MQTT_UNAME          "zwrbsgco"     //MQTT broker user name - I use this broker https://www.cloudmqtt.com/
#define MQTT_PASSW          "Gu19Jq6j-ruy" //MQTT broker password
#define MQTT_BROKER_PORT    18002          //MQTT BROKER listening port
#define MQTT_TOPIC          "ADA/SEnC"

#define ESP_NAME            "ADA-ESP8266"
//#define ESP_PUB_ROLE
#define ESP_SUB_ROLE

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client, MQTT_SERVER, MQTT_BROKER_PORT);
bool mqtt_status;

//Setando as possiveis entradas para o 7seg
byte seven_seg_digits[23][7] = { { 0,0,0,0,0,0,1 },  // = Digito 0
                                 { 1,0,0,1,1,1,1 },  // = Digito 1
                                 { 0,0,1,0,0,1,0 },  // = Digito 2
                                 { 0,0,0,0,1,1,0 },  // = Digito 3
                                 { 1,0,0,1,1,0,0 },  // = Digito 4
                                 { 0,1,0,0,1,0,0 },  // = Digito 5
                                 { 0,1,0,0,0,0,0 },  // = Digito 6
                                 { 0,0,0,1,1,1,1 },  // = Digito 7
                                 { 0,0,0,0,0,0,0 },  // = Digito 8
                                 { 0,0,0,0,1,0,0 },  // = Digito 9
                                 { 0,0,0,1,0,0,0 },  // = Digito A
                                 { 1,1,0,0,0,0,0 },  // = Digito B
                                 { 0,1,1,0,0,0,1 },  // = Digito C
                                 { 1,0,0,0,0,1,0 },  // = Digito D
                                 { 0,1,1,0,0,0,0 },  // = Digito E
                                 { 0,1,1,1,0,0,0 },  // = Digito F
                                 //{ 1,1,1,1,1,0,1 },  // = Digito G
                                 //{ 0,1,1,0,1,1,1 }, // = Digito H
                                 //i
                                 //j
                                 //k
                                 //{ 0,0,0,1,1,1,0 }, // = Digito L
                                 //m
                                 //n
                                 //{ 1,1,1,1,1,1,0 }, // = Digito O
                                 //{ 1,1,0,0,1,1,1 }, // = Digito P
                                 //q
                                 //r
                                 //{ 1,0,1,1,0,1,1 }, // = Digito S
                                 //t
                                 //{ 0,1,1,1,1,1,0 }, // = Digito U
};

int pins[8] = {13, 15, 4, 5, 16, 12, 14, 2};
int dpon = 0;

//Funcao que efetivamente escreve
void sevenSegWrite(byte digit)  //Funcao que aciona o display
{
  int i = 0;
  //Percorre o array ligando os segmentos correspondentes ao digito
  for (byte segCount = 0; segCount < 7; ++segCount){ 
    digitalWrite(pins[i], seven_seg_digits[digit][segCount]);
    i++;
  }
  if (dpon == 0){
    digitalWrite(2, HIGH);
    dpon = 1;
  }
  else{
    digitalWrite(2, LOW);
    dpon = 0;
  }
}

//Wifi Initialization function
int WiFi_init(const char* wifi_ssid, const char* wifi_passwd)
{
        //const char* wifi_ssid   = WIFI_SSID;
        //const char* wifi_passwd = WIFI_PASSWD;

        int retries = 0;

        Serial.print("Connecting to WiFi ");
        //Serial.print(WIFI_SSID);
        Serial.print(wifi_ssid);
        Serial.println("..........");

        WiFi.mode(WIFI_STA); //set wifi station mode
        
        WiFi.begin(wifi_ssid, wifi_passwd); //start connecting to WiFi AP
        
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
void mqtt_callback(const MQTT::Publish& pub)
{
        Serial.println("MQTT receiving a message:");
        Serial.println(pub.payload_string());
        Serial.println();

        for(int i = 0; i < pub.payload_string().length(); i++){
          if(pub.payload_string()[i] == 'A' or pub.payload_string()[i] == 'a'){
            sevenSegWrite(10);
          }else if(pub.payload_string()[i] == 'B' or pub.payload_string()[i] == 'b'){
            sevenSegWrite(11);
          }else if(pub.payload_string()[i] == 'C' or pub.payload_string()[i] == 'c'){
            sevenSegWrite(12);
          }else if(pub.payload_string()[i] == 'D' or pub.payload_string()[i] == 'd'){
            sevenSegWrite(13);
          }else if(pub.payload_string()[i] == 'E' or pub.payload_string()[i] == 'e'){
            sevenSegWrite(14);
          }else if(pub.payload_string()[i] == 'F' or pub.payload_string()[i] == 'f'){
            sevenSegWrite(15);
          }else if(pub.payload_string()[i] == 'G' or pub.payload_string()[i] == 'g'){
            sevenSegWrite(16);
          }else if(pub.payload_string()[i] == 'H' or pub.payload_string()[i] == 'h'){
            sevenSegWrite(17);
          }else if(pub.payload_string()[i] == 'L' or pub.payload_string()[i] == 'l'){
            sevenSegWrite(18);
          }else if(pub.payload_string()[i] == 'O' or pub.payload_string()[i] == 'o'){
            sevenSegWrite(19);
          }else if(pub.payload_string()[i] == 'P' or pub.payload_string()[i] == 'p'){
            sevenSegWrite(20);
          }else if(pub.payload_string()[i] == 'S' or pub.payload_string()[i] == 's'){
            sevenSegWrite(21);
          }else if(pub.payload_string()[i] == 'U' or pub.payload_string()[i] == 'u'){
            sevenSegWrite(22);
          }else if(pub.payload_string()[i] == '0'){
            sevenSegWrite(0);
          }else if(pub.payload_string()[i] == '1'){
            sevenSegWrite(1);
          }else if(pub.payload_string()[i] == '2'){
            sevenSegWrite(2);
          }else if(pub.payload_string()[i] == '3'){
            sevenSegWrite(3);
          }else if(pub.payload_string()[i] == '4'){
            sevenSegWrite(4);
          }else if(pub.payload_string()[i] == '5'){
            sevenSegWrite(5);
          }else if(pub.payload_string()[i] == '6'){
            sevenSegWrite(6);
          }else if(pub.payload_string()[i] == '7'){
            sevenSegWrite(7);
          }else if(pub.payload_string()[i] == '8'){
            sevenSegWrite(8);
          }else if(pub.payload_string()[i] == '9'){
            sevenSegWrite(9);
          }
          delay(500);
      }
}

int MQTT_init(boolean topic_subscribe)
{
        Serial.println("Initializing MQTT communication.........");
               
        mqtt_client.set_callback(mqtt_callback); //set callback on received messages
        mqtt_client.set_max_retries(255);
        
        //here we connect to MQTT broker and we increase the keepalive for more reliability
        if (mqtt_client.connect(MQTT::Connect(MQTT_CLIENT_ID).set_keepalive(1200).set_auth(String(MQTT_UNAME), String(MQTT_PASSW)))) {
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
        //setando os pinos do 7seg
        pinMode(13, OUTPUT); //GPIO 13 do ESP ligado ao segmento A  
        pinMode(15, OUTPUT); //GPIO 15 do ESP ligado ao segmento B
        pinMode(4, OUTPUT);  //GPIO  4 do ESP ligado ao segmento C
        pinMode(5, OUTPUT);  //GPIO  5 do ESP ligado ao segmento D
        pinMode(16, OUTPUT); //GPIO 16 do ESP ligado ao segmento E
        pinMode(12, OUTPUT); //GPIO 12 do ESP ligado ao segmento F
        pinMode(14, OUTPUT); //GPIO 14 do ESP ligado ao segmento G
        pinMode(2, OUTPUT);  //GPIO  2 do ESP ligado ao dot point
        
        Serial.begin(SERIAL_SPEED);
        delay(100);

        Serial.println();
        Serial.println("MQTT_basic starting....");

        int i;
        for(i = 0; i < numssid; i=i+2){
            if (WiFi_init(ssids[i], ssids[i+1]) != WL_CONNECTED) {
                    Serial.print("WiFi connection ERROR to ");
                    Serial.print(ssids[i]);
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
                        break;
                }
                else{
                        Serial.println("MQTT connection OK....");   
                        break;
                }     
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
