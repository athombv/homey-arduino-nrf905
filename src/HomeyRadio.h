#ifndef HOMEY_RADIO_H
#define HOMEY_RADIO_H

#ifdef ARDUINO
#include <Arduino.h>
#else 
#include <stdint.h>
#endif 

namespace Homey {
	class Radio {
	public:
		Radio(uint8_t address);
		~Radio(void);

		void initialize(void);	

		bool send(uint8_t destAddress, void* data, uint8_t size);
		void listeningMode(void);

		bool getData(uint8_t* srcAddress, void* data, uint8_t size);
		void onData(uint8_t* data, uint8_t size);

		void enable(void);
		void disable(void);
	private:
		uint8_t mAddress;
	};
};
#endif // HOMEY_RADIO_H
