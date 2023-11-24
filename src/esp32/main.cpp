#include <Arduino.h>

//#define RXp2 16
//#define TXp2 17

//conecta no pino RX pra receber input de Serial2

#include <LiquidCrystal.h> //Inclusão da Biblioteca
// Create An LCD Object. Signals: 
//              [ RS, E, D4, D5, D6, D7 ]
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);
//Fonte: https://deepbluembedded.com/esp32-lcd-display-16x2-without-i2c-arduino/

//int backLight = 13; // pino 13 responsável pelo controle do LED

char charRead = -1;

void setup()
{
	//pinMode(backLight, OUTPUT);
	//digitalWrite(backLight, HIGH); // Liga backlight. Substituir 'HIGH' por 'LOW' para desligar a luz de fundo.

	lcd.begin(16, 2);		   // Colunas, linhas. Use 16,2 para display LCD 16x2
	lcd.clear();			   // Inicializa o display com uma tela vazia
	lcd.setCursor(0, 0);	   // Posiciona o cursor na coluna 0, linha 0 (primeira linha)
	lcd.print("Iniciando..."); // Escreve o texto desejado

	lcd.setCursor(0, 1); // Posiciona o cursor para a coluna 0, linha 1 (segunda linha)
	lcd.print("----");

	Serial.begin(115200);

	//Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
	Serial2.begin(9600);
}

void loop()
{
	
	int i = 0;
	int j = 0;

	charRead = Serial2.read();
	Serial.print("char = ");
	Serial.print(charRead);
	Serial.print(" = ");
	Serial.println((int)charRead);
	if (charRead == 'l')
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Pessoas=");
		lcd.setCursor(0, 1);
		lcd.print("Tensao%=");

		charRead = Serial2.read();
		while (charRead != '#')
		{
			lcd.setCursor(8 + i, 0);
			lcd.print(charRead);
			i++;
			charRead = Serial2.read();
		}

		charRead = Serial2.read();
		while (charRead != '#')
		{
			lcd.setCursor(8 + j, 1);
			lcd.print(charRead);
			j++;
			charRead = Serial2.read();
		}
	}
	else if (charRead == 'd')
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Desligado");
	}

	delay(1000);
}
