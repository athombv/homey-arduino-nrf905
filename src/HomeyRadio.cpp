#include "lib/nRF905/nRF905.h"
#include "HomeyRadio.h"

using namespace Homey;

#define RETRANS_CYCLES 	10	
#define SEQUENCENR_MAX 	128

#define MESSAGE_ACK

static Radio* radio; 
/* nrf on data event */
void nRF905_on_data(uint8_t* data, uint8_t size) {
	radio->onData(data, size);
}

Radio::Radio(uint8_t address) : mAddress(address) {
	radio = this;	
}
		
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
	uint8_t idx = 0;
	seqNr = (seqNr + 1) & SEQUENCENR_MAX-1;

	txBuffer[idx++] = mAddress; 
	txBuffer[idx++] = seqNr;

	memcpy((void*) (txBuffer+idx),(void*) data, size);
	// set destination address
	nRF905_setTXAddress((void*) &address);
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
	while( !nRF905_getData(rxBuffer, sizeof(rxBuffer)) );

	// received ack message?
	if(!(rxBuffer[1] & 0x80) || seqNr != (rxBuffer[1] & 0x7F) )
		return false;
	// is ack coming from the sender?
	if(rxBuffer[0] != address) 
		return false;

	#endif

	return true;
}

void Radio::listeningMode(void) {
	nRF905_receive();
}

bool Radio::getData(void* data, uint16_t size, uint8_t* srcAddress) {
	if(size > NRF905_PAYLOAD_SIZE-2) return false;

	uint8_t buffer[NRF905_PAYLOAD_SIZE];
	if( nRF905_getData(buffer, sizeof(buffer)) ) {
		// received a 'data' message?	
		if( !(buffer[1] & 0x80)	) {
			// set address
			*srcAddress = buffer[0];			
			memcpy((void*) data,(void*) (buffer+2), size);

			return true;
		}
	}
	return false;
}

void Radio::onData(uint8_t* data, uint8_t size) {
	uint8_t buffer[NRF905_PAYLOAD_SIZE];	
	uint8_t idx = 0;

	buffer[idx++] = mAddress; 		// set source address
	buffer[idx++] = data[1] & 0x80; // set ack message 
	// copy data to buffer
	memcpy((void*) (buffer+idx),(void*) (data+idx), NRF905_PAYLOAD_SIZE -2);

	nRF905_setTXAddress((void*) &data[0]); // set address to send ack to
	nRF905_setData(buffer, sizeof(buffer));
	// send message, we do not have to wait 
	nRF905_send();
}

void Radio::enable(void) {
	nRF905_powerUp();
}

void Radio::disable(void) {
	nRF905_powerDown();
}

