#include <Arduino.h>
#include "../../include/sniffer.hpp"

unsigned long millisBefore = 0;

void setup() {
	Serial.begin(115200);
	sniffer.startSniffer();
	Serial.print("OKAY");
}

void loop() {
	for (int i = 0; i < 13; i++) {
		sniffer.changeListChannel();
		Serial.printf("Mudou para o canal = %d\n", sniffer.getActualChannel());
		delay(500);
	}
	delay(5000);
	sniffer.printMacList();
	sniffer.resetList();
}
