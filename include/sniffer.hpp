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
		void insertMacList(wifi_ieee80211_mac_hdr_t *mac);
		bool isInList(uint8_t *mac);
		int maxMACList = 100;
		uint8_t **macList = nullptr;
		int macListIndex = 0;

};

extern Sniffer sniffer;
