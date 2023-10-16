#include "../../include/sniffer.hpp"
#include "esp_wifi.h"
#include "sys/_stdint.h"
#include <cstdlib>

#define MAC_COLUMNS 6
#define MAX_CHANNEL 13

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

void Sniffer::startSniffer() {
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();
  esp_wifi_set_promiscuous(true);
  //esp_wifi_set_promiscuous_filter(&filt);
  esp_wifi_set_promiscuous_rx_cb(&this->handleSniffer);
}

void Sniffer::resetList() {
	for (int i = 0; i < this->macListIndex; i++) {
		delete [] this->macList[i];
	}
	this->macListIndex = 0;
}

void Sniffer::changeListChannel() {
  esp_wifi_set_channel((this->actualChannel < MAX_CHANNEL) ? this->actualChannel++ : this->actualChannel = 1, WIFI_SECOND_CHAN_NONE);
}

uint8_t **Sniffer::getMacList() {
	return this->macList;
}

void Sniffer::handleSniffer(void* buff, wifi_promiscuous_pkt_type_t type) {
	if (type != WIFI_PKT_MGMT)
    return;

  wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buff;
  wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
  wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

	sniffer.insertMacList(hdr);
}

void Sniffer::insertMacList(wifi_ieee80211_mac_hdr_t *mac) {
	if (this->macListIndex >= this->maxMACList || isInList(mac->addr2))
		return;

	this->macList[this->macListIndex] = new uint8_t[6];

	for (int i = 0; i < MAC_COLUMNS; i++) {
		this->macList[this->macListIndex][i] = mac->addr2[i];
	}

	this->macListIndex++;
}

bool Sniffer::isInList(uint8_t *mac) {
	for (int i = 0; i < this->macListIndex; i++) {
		for (int oct = 0; oct < MAC_COLUMNS; oct++) {
			if (mac[oct] != this->macList[i][oct])
				break;
			else if (oct >= 5) {
				return true;
			}
		}
	}
	return false;
}

void Sniffer::printMacList() {
	Serial.printf("Found %02d MAC address\n", this->macListIndex);
	for (int i = 0; i < this->macListIndex; i++) {
		printf("MAC(%02d) -> %02x:%02x:%02x:%02x:%02x:%02x\n", i,
				this->macList[i][0],this->macList[i][1],this->macList[i][2],
				this->macList[i][3],this->macList[i][4],this->macList[i][5]);
	}
	Serial.print("END\n");
}
