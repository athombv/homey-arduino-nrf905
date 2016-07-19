# Homey arduino nRF905
This library can be used communicate between a Homey and a arduino using the nRF905 (433MHz) module. The library automatically acknowledges every incoming message so that the sender knows that de message has been received successfully. 

Homey currently only supports polling mode, which means that only Homey can initiate  the communication between Homey and the Arduino. The reason for this is that Homey cannot receive data from GFSK and ASK modulated devices at the same time.

## Requirements
- nRF905 module on 433MHz
- Arduino (avr) 
- Homey running verison 0.9.0 or later

## Installation
### Hardware
The Homey Arduino library uses the nRF905 library developed by Zak Kemble to communicate with the nRF905. 
Information about how to wire the nRF905 to your arduino can be found on his [website](http://blog.zakkemble.co.uk/nrf905-avrarduino-librarydriver/).

### Software
The library can be imported by downloading the library as a .zip file and then add it to the Arduino IDE by using the library manager. 

## Usage
Examples are provided to give you a quick overview of how to use the library. 
include the library into your project by adding the header file
`#include <HomeyRadio.h>`.

After that you need to create a Homey-Radio instance with a desired receiver address
and call the initialize method to initialize the nRF905 module, as shown in the example below.
```
#include <HomeyRadio.h>

Homey::Radio radio(0xFF);

void setup() {
    radio.initialize();
    radio.listeningMode();
}
```
At this point, the nRF905 is set into listening mode and ready to receive data from Homey.
Data can be received by calling the `getData(data, size, srcAddress)` method or transmitted by using the `send(destAddress, data, size)`.


### Methods

- `void Radio::initialize(void)`
    * ***returning:***      -
    * ***Description:***  Initializing nRF905 module
    * ***Parameters:*** -
    

- `bool Radio::send(byte destAddress, void* data, byte size)`
    * ***returning:***  True when Homey has succesfully received the data
    * ***Description:***  Method to send data to a Homey
    * ***Parameters:***
        * ***address:*** address of the Homey device
        * ***data:*** pointer to a byte-array
        * ***size:*** size of byte-array
    
- `bool Radio::getData(byte* srcAddress, void* data, byte size)`
    * ***returning:***  True when nRF905 has successfully received data from Homey
    * ***Description:***  Method to send data to a Homey
    * ***Parameters:***
        * ***address:*** address of the Homey device
        * ***data:*** pointer to a byte-array
        * ***size:*** size of byte-array

- `void Radio::listeningMode(void)`
    * ***returning:***  -
    * ***Description:***  enable nRF905 module to receive data
    * ***Parameters:*** -

- `void Radio::enable(void)`
    * ***returning:***  -
    * ***Description:***  power up nRF905 module
    * ***Parameters:*** -

- `void Radio::disable(void)`
    * ***returning:***  -
    * ***Description:***  power down nRF905 module 
    * ***Parameters:*** -
## Examples
The library includes several examples which can be found in the examples folder. Three examples are provided which explain respectively sending, receiving and requests,

- examples/Hello_world
- examples/Print_data
- examples/Request_handler
## Homey App driver
A Nodejs Homey driver is written which can be included in your Homey app project. The driver includes a signal-definition and message acking.  The driver can be found [here](http://github.com...).
## Contributing
Contribute to this library by sending me pull-requests.
## Credits
Special credit to Zak Kemble for his nRF905 driver
## Licence
