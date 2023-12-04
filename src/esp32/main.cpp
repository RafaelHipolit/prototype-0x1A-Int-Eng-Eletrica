#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <string.h>

#define WEB_SOCKET_SERVER "ws://192.168.243.115:8080/"
#define SSID "n" //nome wife
#define PASSWORD "n" //senha wifi

//#define RXp2 16
//#define TXp2 17

//conectar no pino RX_2 (pin 16) pra receber input de Serial2

websockets::WebsocketsClient socket;
//fonte: https://www.youtube.com/watch?v=eFhOlZTFyr4
bool connectedServer = false;

char charRead = -1;
int numPassengers = 0;
int percentPowerFan = 0;


void connectWiFi() {
    WiFi.mode(WIFI_OFF);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID,PASSWORD);
    Serial.print("Connecting to WiFi ");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.print("Connected to :");
    Serial.println(SSID);
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

void handleMessage(websockets::WebsocketsMessage msg){
    Serial.println(msg.data());
}

void handleEvent(websockets::WebsocketsEvent e, websockets::WSInterfaceString data){

}

void setup()
{	

	Serial.begin(115200);

	//Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
	Serial2.begin(9600);

	//conecta ao wifi e web socket
	connectWiFi();
    connectWebSocket();
    socket.onMessage(handleMessage);
    socket.onEvent(handleEvent);
}

void loop()
{	

	charRead = Serial2.read();
	Serial.print("char = ");
	Serial.print(charRead);
	Serial.print(" = ");
	Serial.println((int)charRead);
	if (charRead == 'l')
	{
		std::string stringAux = "";

		charRead = Serial2.read();		
		while (charRead != '#')
		{
			stringAux += charRead;
			charRead = Serial2.read();
		}
		numPassengers = std::stoi(stringAux);
		stringAux = "";

		charRead = Serial2.read();
		while (charRead != '#')
		{
			stringAux += charRead;
			charRead = Serial2.read();
		}
		percentPowerFan = std::stoi(stringAux);

		Serial.print("Pessoas=");
		Serial.print(numPassengers);
		Serial.print(";Ventoinha(%)=");
		Serial.println(percentPowerFan);


		socket.send(std::to_string(numPassengers).c_str());
	}
	else if (charRead == 'd')
	{
		socket.send("CONTADOR DESLIGADO");
	}


	if(!connectedServer){
        Serial.println("connecting to web socket server...");
        connectWebSocket();
    }

    socket.poll(); // chama os handleMessage e handleEvent

    delay(2000);
}