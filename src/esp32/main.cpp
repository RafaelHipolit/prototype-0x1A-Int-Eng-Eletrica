#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <string.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WEB_SOCKET_SERVER "ws://[IPV4]:8080/"
#define SSID "nome" //nome wife
#define PASSWORD "senha" //senha wifi

#define LCD_ON 1 //ativa codigo do lcd = 1; se nao 0
#define OLED_ON 1 //ativa codigo do telinha oled = 1; se nao 0

#define OLED_SCREEN_WIDTH 128 // OLED display width, in pixels
#define OLED_SCREEN_HEIGHT 64 // OLED display height, in pixels

//#define RXp2 16
//#define TXp2 17

//conectar no pino RX_2 (pin 16) pra receber input de Serial2

Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, -1);
//fonte: https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/


// Create An LCD Object. Signals: 
//              [ RS, E , D4, D5, D6, D7 ]
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);
//Fonte: https://deepbluembedded.com/esp32-lcd-display-16x2-without-i2c-arduino/

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

void LCDsetup(){
	lcd.begin(16, 2);		   // Colunas, linhas. Use 16,2 para display LCD 16x2
	lcd.clear();			   // Inicializa o display com uma tela vazia
	lcd.setCursor(0, 0);	   // Posiciona o cursor na coluna 0, linha 0 (primeira linha)
	lcd.print("Iniciando..."); // Escreve o texto desejado

	lcd.setCursor(0, 1); // Posiciona o cursor para a coluna 0, linha 1 (segunda linha)
	lcd.print("----");
}

void OLEDsetup(){
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    //for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("iniciando...");
  display.display(); //inicia a exibicao na tela
}

void setup()
{
	//conecta ao wifi e web socket
	connectWiFi();
    connectWebSocket();
    socket.onMessage(handleMessage);
    socket.onEvent(handleEvent);

	if (LCD_ON)
	{
		LCDsetup();
	}

	if (OLED_ON)
	{
		OLEDsetup();
	}	

	Serial.begin(115200);

	//Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
	Serial2.begin(9600);

	//pinMode(15,INPUT);
	//pinMode(2,OUTPUT);
	//digitalWrite(2,LOW);
}

int cont = 0;

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
			stringAux += std::to_string(charRead);
			charRead = Serial2.read();
		}
		numPassengers = std::stoi(stringAux);
		stringAux = "";

		charRead = Serial2.read();
		while (charRead != '#')
		{
			stringAux += std::to_string(charRead);
			charRead = Serial2.read();
		}
		percentPowerFan = std::stoi(stringAux);

		if (LCD_ON)
		{
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Pessoas=");
			lcd.setCursor(8, 0);
			lcd.print(numPassengers, 10);
			lcd.setCursor(0, 1);
			lcd.print("Tensao%=");
			lcd.setCursor(8, 0);
			lcd.print(percentPowerFan, 10);
		}

		if (OLED_ON)
		{
			display.clearDisplay();
			display.setCursor(0, 0);
			display.println("Passageiros=");
			display.setCursor(0, 1);
			display.println(numPassengers, 10);
			display.setCursor(0, 2);
			display.println("Tensao=");
			display.setCursor(0, 3);
			display.println(percentPowerFan, 10);
		}

	}
	else if (charRead == 'd')
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Desligado");

		display.clearDisplay();
		display.setCursor(0, 0);
  		display.println("Desligado");
	}

	/*
	if (digitalRead(15))
	{
		Serial.println("PIN ON");
		cont++;
	}
	display.clearDisplay();
	display.setCursor(0, 0);
	display.print("0123456789"); //10 char
	display.setCursor(0, 16);
	display.println(cont,10);
	display.display();
	*/


	if(!connectedServer){
        Serial.println("connecting to web socket server...");
        connectWebSocket();
    }

    socket.poll(); // chama os handleMessage e handleEvent

    socket.send(std::to_string(numPassengers).c_str());

    delay(2000);
}