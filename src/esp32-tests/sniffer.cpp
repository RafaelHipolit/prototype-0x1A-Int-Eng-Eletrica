#include "sniffer.hpp"
#include "esp_wifi.h"
#include "sys/_stdint.h"
#include <cstdlib>

Sniffer sniffer;

Sniffer::Sniffer() {
	this->macList = (uint8_t **)malloc(sizeof(uint8_t) * this->maxMACList);
}

Sniffer::Sniffer(int listMax) {
	this->maxMACList = listMax;
	this->macList = (uint8_t **)malloc(sizeof(uint8_t) * this->maxMACList);
}

Sniffer::~Sniffer() {
	for (int i = 0; i < this->macListIndex; i++) {
		delete [] this->macList[i];
	}
	free(this->macList);
}
