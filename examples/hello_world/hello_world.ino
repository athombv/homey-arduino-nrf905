#include <HomeyRadio.h>

Homey::Radio radio(0x58);

void setup() {
	// initialize module
	radio.initialize();
	Serial.begin(9600);

	radio.listeningMode();
	Serial.println("Hello world transmitter");
}

void loop() {
	char data[] = "Hello \n";
	// send ping to homey
	radio.send(0xf0, (void*) &data, sizeof(data));	

	delay(1000);
}
