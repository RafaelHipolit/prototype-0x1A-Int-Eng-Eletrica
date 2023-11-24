#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <string.h>

#define WEB_SOCKET_SERVER "ws://192.168.1.11:81/"

using namespace websockets;

WebsocketsClient socket;
bool connectedServer = false;

const char* ssid = "nome";
const char* password = "senha";

void connectWiFi() {
    WiFi.mode(WIFI_OFF);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,password);
    Serial.print("Connecting to WiFi ");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.print("Connected to :");
    Serial.println(ssid);
    Serial.print("IP address : ");
    Serial.println(WiFi.localIP());
}

void connectWebSocket(){
    connectedServer = socket.connect(WEB_SOCKET_SERVER);
    if (connectedServer)
    {
        Serial.println("Connected to web socket");
    }
    else {
        Serial.println("Connection to web socket failed");
    }
}

void handleMessage(WebsocketsMessage msg){
    Serial.println(msg.data());
}

void handleEvent(WebsocketsEvent e, WSInterfaceString data){

}

void setup(){
    Serial.begin(115200);

    connectWiFi();

    connectWebSocket();

    socket.onMessage(handleMessage);
    socket.onEvent(handleEvent);
}

int i = 0;

void loop(){

    if(!connectedServer){
        Serial.println("connecting to web socket server...");
        connectWebSocket();
    }

    socket.poll(); // chama os handleMessage e handleEvent

    std::string s = std::to_string(i);
    char const *pchar = s.c_str();

    socket.send(pchar);

    i++;

    delay(2000);  
}