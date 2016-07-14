#include "lib/nRF905/nRF905.h"
#include "HomeyRadio.h"

using namespace Homey;

#define RETRANS_CYCLES 	10	
#define SEQUENCENR_MAX 	128

//#define MESSAGE_ACK

Radio::Radio(uint8_t address) : mAddress(address) {}

Radio::~Radio(void) {
}


void Radio::initialize(void) {
  nRF905_init();
  nRF905_setRXAddress(&mAddress);
}

bool Radio::send(uint8_t address, void* data, uint16_t size) {
	if(size > NRF905_PAYLOAD_SIZE-1) return false;

	static uint8_t seqNr = 0;
	uint8_t txBuffer[NRF905_PAYLOAD_SIZE];

	// add sequencenr and type to packet
	seqNr = (seqNr + 1) & SEQUENCENR_MAX-1;
	txBuffer[0] = seqNr & 0x7F; 

	memcpy(txBuffer+1, data, size);

	nRF905_setTXAddress(&address);
	nRF905_setData(txBuffer, sizeof(txBuffer));

	uint8_t i;
	// retransmit message 
	for(i = 0; i < RETRANS_CYCLES; i++)
		while(!nRF905_send()); // block until message has been send

	// set nrf into receive mode
	nRF905_receive();

	#ifdef MESSAGE_ACK
	// block until data has been received
	uint8_t rxBuffer[NRF905_PAYLOAD_SIZE];
	while( !nRF905_getData(rxBuffer, sizeof(rxBuffer)) );
	// received ack message?
	if(rxBuffer[0] & 0x80) {
		rxBuffer[0] &= 0x7F; // reset ack flag
		// compare sent and received (ack) message
		if(memcmp(rxBuffer, txBuffer, sizeof(rxBuffer)) != 0)
			return false;
	}
	#endif

	return true;
}

void Radio::listeningMode(void) {
	nRF905_receive();
}

bool Radio::getData(void* data, uint16_t size) {
	if(size > NRF905_PAYLOAD_SIZE-1) return false;

	uint8_t buffer[NRF905_PAYLOAD_SIZE];
	if( nRF905_getData(buffer, sizeof(buffer)) ) {
		if( !(buffer[0] & 0x80)	) {
			// no ack, valid message, copy buffer to data
			memcpy(data, buffer+1, size);

			return true;
		}
	}
	return false;
}

void Radio::enable(void) {
	nRF905_powerUp();
}

void Radio::disable(void) {
	nRF905_powerDown();
}

