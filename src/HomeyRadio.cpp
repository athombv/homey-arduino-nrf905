#include "lib/nRF905/nRF905.h"
#include "HomeyRadio.h"

#include <util/delay.h>
#include <Arduino.h>

using namespace Homey;

#define RETRANS_CYCLES 	5
#define SEQUENCENR_MAX 	128
#define ACK_TIMEOUT		50 // ms

#define MESSAGE_ACK

static Radio* radio; 

Radio::Radio(uint8_t address) : mAddress(address) {
	radio = this;	
}
		
Radio::~Radio(void) {
}


void Radio::initialize(void) {
  nRF905_init();
  nRF905_setRXAddress(&mAddress);
}

bool Radio::send(uint8_t destAddress, void* data, uint8_t size) {
	if(size > NRF905_PAYLOAD_SIZE-2) return false;

	static uint8_t seqNr = 0;
	uint8_t txBuffer[NRF905_PAYLOAD_SIZE];

	// add sequencenr and type to packet
	uint8_t idx = 0;
	seqNr = (seqNr + 1) & SEQUENCENR_MAX-1;

	txBuffer[idx++] = mAddress; 
	txBuffer[idx++] = seqNr;

	memcpy((void*) (txBuffer+idx),(void*) data, size);
	// set destination address
	nRF905_setTXAddress((void*) &destAddress);
	nRF905_setData(txBuffer, sizeof(txBuffer));

	// retransmit message 
	uint8_t i;
	for(i = 0; i < RETRANS_CYCLES; i++)
		while(!nRF905_send()); // block until message has been send

	// set nrf into receive mode
	nRF905_receive();

	#ifdef MESSAGE_ACK
	// block until data has been received
	uint8_t rxBuffer[NRF905_PAYLOAD_SIZE];
	uint16_t timeout = ACK_TIMEOUT * 2;
	while( !nRF905_getData(rxBuffer, sizeof(rxBuffer)) && timeout--) _delay_us(500);

	// timeout occured?
	if(timeout <= 0)
		return false;
	// received ack message?
	if(!(rxBuffer[1] & 0x80) || seqNr != (rxBuffer[1] & 0x7F) )
		return false;
	// is ack coming from the sender?
	if(rxBuffer[0] != destAddress) 
		return false;
	#endif

	return true;
}

void Radio::listeningMode(void) {
	nRF905_receive();
}

bool Radio::getData(uint8_t* srcAddress, void* data, uint8_t size) {
	if(size > NRF905_PAYLOAD_SIZE-2) return false;

	uint8_t buffer[NRF905_PAYLOAD_SIZE];
	static uint8_t prevSeqNr = 0xFF;
	if( nRF905_getData(buffer, sizeof(buffer)) ) {
		// received a 'data' message?	
		uint8_t msgType = buffer[1] & 0x80;
		uint8_t msgNr = buffer[1] & 0x7F;
		uint8_t msgSrcAddr = buffer[0];
		// if msgtype is 'data' and message is different then the previous one
		if( msgType == 0 && msgNr != prevSeqNr) {
			prevSeqNr = msgNr; 
			// set src address
			*srcAddress = msgSrcAddr;
			// copy payload 
			memcpy((void*) data,(void*) (buffer+2), size);
			// change dest address
			nRF905_setTXAddress((void*) &msgSrcAddr); // set address to send ack to
			// set header	
			buffer[0] = mAddress; // set source address
			buffer[1] |= 0x80; // set ack message 

			nRF905_setData(buffer, sizeof(buffer));
			// wait for homey to switch to rx mode
			_delay_ms(3);
			// send message 
			uint8_t i;
			for(i = 0; i < RETRANS_CYCLES; i++) {
				while(!nRF905_send());
			}
			// turn back into receive mode
			nRF905_receive();	

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

