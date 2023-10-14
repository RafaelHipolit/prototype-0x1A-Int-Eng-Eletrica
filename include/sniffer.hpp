#pragma once

#include <Arduino.h>
#include "esp_wifi.h"
#include "sys/_stdint.h"

typedef struct {
  unsigned frame_ctrl:16;
  unsigned duration_id:16;
  uint8_t addr1[6]; /* receiver address */
  uint8_t addr2[6]; /* sender address */
  uint8_t addr3[6]; /* filtering address */
  unsigned sequence_ctrl:16;
  uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

class Sniffer{
	public:
		Sniffer();
		Sniffer(int listMax);
		~Sniffer();
		void startSniffer();
		uint8_t **getMacList();
		void changeListChannel();
		void resetList();
		void printMacList();

	private:
		void insertMacList(wifi_ieee80211_mac_hdr_t *mac);
		bool isInList(uint8_t *mac);
		static void handleSniffer(void* buff, wifi_promiscuous_pkt_type_t type);
		int maxMACList = 100;
		uint8_t **macList = nullptr;
		int actualChannel = 0;
		int macListIndex = 0;

};

extern Sniffer sniffer;
