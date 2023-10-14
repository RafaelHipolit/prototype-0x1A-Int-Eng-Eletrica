#pragma once

#include <Arduino.h>
#include "esp_wifi.h"
#include "sys/_stdint.h"

class Sniffer{
	public:
		Sniffer();
		Sniffer(int listMax);
		~Sniffer();

	private:
		int maxMACList = 100;
		uint8_t **macList = nullptr;

};

extern Sniffer sniffer;
