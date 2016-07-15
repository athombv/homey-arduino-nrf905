#include <HomeyRadio.h>

Homey::Radio radio(0x58);

void setup() {
	// initialize module
	radio.initialize();
	Serial.begin(9600);

	radio.listeningMode();
	Serial.println("---- print received data ----");
}

void loop() {
	char data[8];
	uint8_t srcAddress;

	while( !radio.getData(data, sizeof(data), &srcAddress);
	Serial.println("received:");
	uint8_t i;
	for(i = 0; i < 8; i++) {
		Serial.println(data[i] & 0xFF, HEX);	
	}
}
