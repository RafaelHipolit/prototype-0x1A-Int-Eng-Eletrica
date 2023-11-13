// Codigo do prototipo do grupo 0xA1 de IntEngEle
// Versao arduno

// includes:
#include <Ultrasonic.h>


// CONSTANTES:

// pinos sensor ultrasonico 1 - entrada de pessoas
#define PIN_TRIG_1 13
#define PIN_ECHO_1 12

// pinos sensor ultrasonico 2 - saida de pessoas
#define PIN_TRIG_2 11
#define PIN_ECHO_2 10

//pinos dos led que acendem quando o sensor esta detectando algo
#define PIN_LED_1 9 //led do sensor ultrasonico 1 - entrada de pessoas
#define PIN_LED_2 8 //led do sensor ultrasonico 2 - saida de pessoas

#define PIN_PWM 6 //pino do pwm para a ponte h

#define PIN_H_BRIDGE_DIRECTION_ROTATION 5 //pino conectado a ponte h para determinar seu sentido de rotacao 

// distancia maxima que os sensores ultrasonicos detectam algo
#define DIST_MAX_DETECTAR 20

#define MAX_QUANT_PEOPLE 10
#define MIN_QUANT_PEOPLE 0


// variaveis globais:
int percentVoltage = 0;
float dist1 = 0, dist2 = 0;
bool detectou1 = false, detectou2 = false;
int quantPessoas = 0;
bool alguemSaiu = false;


// declaracao das funcoes:
float readUltrasonicDistanceCm(int triggerPin, int echoPin);
void calcPercentVoltage();


// implementacao das funcoes:

// funcao que faz o sensor ultrasonico medir a distancia e retorna o valor medido - retirada de https://www.makerhero.com/blog/sensor-ultrassonico-hc-sr04-ao-arduino/
float readUltrasonicDistanceCm(int triggerPin, int echoPin)
{
	Ultrasonic ultrasonic(triggerPin, echoPin);
	long microsec = ultrasonic.timing();
	return ultrasonic.convert(microsec, Ultrasonic::CM);
}

// funcao de calcula e retorna a porcentagem de tensao de acordo com quantidade de pessoas
void calcPercentVoltage()
{
	if (quantPessoas > MAX_QUANT_PEOPLE)
	{
		percentVoltage = 100;
	}
	else if (quantPessoas < MIN_QUANT_PEOPLE)
	{
		percentVoltage = 0;
	}
	else
	{
		percentVoltage = ((float)(quantPessoas - MIN_QUANT_PEOPLE) / (MAX_QUANT_PEOPLE - MIN_QUANT_PEOPLE)) * 100;
	}
	Serial.print("percentVoltage = ");
	Serial.println(percentVoltage);
}




void setup()
{
	//configura dos pinos dos led como output
	pinMode(PIN_LED_1, OUTPUT);
	pinMode(PIN_LED_2, OUTPUT);

	//configura o pino do sentido de rotacao da ponte h e o ativa
	pinMode(PIN_H_BRIDGE_DIRECTION_ROTATION, OUTPUT);
	digitalWrite(PIN_H_BRIDGE_DIRECTION_ROTATION, HIGH);

	//configura o pino do pwm e deixa ele inicialmente desligado
	pinMode(PIN_PWM, OUTPUT);
	analogWrite(PIN_PWM, 0);

	Serial.begin(9600);
}

void loop()
{
	//LOGICA:
	//dist recebe a distancia que o sensor detecta
	//Se a disatancia estiver dentro do intervalo que eu quero, entao a var detectou recebe true
	//ou seja, o sensor detectou algo
	//separadamente faco outra verificacao: Se a var detectou ta true porem a distancia nao ta mais dentro do intervalo que eu quero,
	//entao o que o sensor estava detectando sumiu
	//portanto eu aumento ou diminuo a quant de pessoas e coloco false em detectou

	dist1 = readUltrasonicDistanceCm(PIN_TRIG_1, PIN_ECHO_1);
	if (dist1 < DIST_MAX_DETECTAR)
	{
		if (!detectou1)
		{
			Serial.print("Sensor 1 detectou algo - Distancia(cm) = ");
			Serial.println(dist1);
			digitalWrite(PIN_LED_1, HIGH);
		}
		detectou1 = true;
	}

	dist2 = readUltrasonicDistanceCm(PIN_TRIG_2, PIN_ECHO_2);
	if (dist2 < DIST_MAX_DETECTAR)
	{
		if (!detectou2)
		{
			Serial.print("Sensor 2 detectou algo - Distancia(cm) = ");
			Serial.println(dist2);
			digitalWrite(PIN_LED_2, HIGH);
		}
		detectou2 = true;
	}

	if (detectou1 && dist1 >= DIST_MAX_DETECTAR)
	{
		detectou1 = false;
		Serial.println("O que o sensor 1 detectou sumiu");
		quantPessoas++;
		alguemSaiu = false;
		Serial.print("Quantidade de pessoas = ");
		Serial.println(quantPessoas);
		calcPercentVoltage();
		digitalWrite(PIN_LED_1, LOW);
	}

	if (detectou2 && dist2 >= DIST_MAX_DETECTAR)
	{
		detectou2 = false;
		Serial.println("O que o sensor 2 detectou sumiu");
		if (quantPessoas != 0)
		{
			quantPessoas--;
			alguemSaiu = true;
		}
		Serial.print("Quantidade de pessoas = ");
		Serial.println(quantPessoas);
		calcPercentVoltage();
		digitalWrite(PIN_LED_2, LOW);
	}

	//aplico a tensao calculada no pino do PWM
	if (percentVoltage >= 40)
	{	
		//para ativar os pinos de PWM deve-se usa a funcao analogWrite que recebe o numero do pino e a quantidade de tensao por um valor entre 0 e 255
		//aqui pego o valor da porcentagem entre 0 e 100, divido por 100 pra ficar um valor entre 0 e 1, e depois multiplico por 255
		analogWrite(PIN_PWM, (int)(255.0 * (percentVoltage / 100.0)));
	}
	if (percentVoltage < 40 && alguemSaiu)
	{
		analogWrite(PIN_PWM, (int)(255.0 * (percentVoltage / 100.0)));
	}

	delay(100);
}








// codigo do thinker cad - to deixando aqui so pra ficar salvo
/*
float readUltrasonicDistanceCm(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return 0.01723 * pulseIn(echoPin, HIGH);
}
*/
