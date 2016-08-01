# Homey arduino nRF905
This library can be used to communicate between a Homey and a arduino using the nRF905 (433MHz) module. The library automatically acknowledges every incoming message so that the sender knows that the message has been received successfully. 

Homey currently only supports polling mode, which means that only Homey can initiate the communication between Homey and the Arduino. The reason for this is that Homey cannot receive data from GFSK and ASK modulated devices at the same time. 
After Homey has transmitted to the arduino it switches to receive mode and waits 200ms for the arduino to respond back. Within the 200ms the arduino can send data to Homey.

## Requirements
- nRF905 module on 433MHz
- Arduino (avr) 
- Homey running verison 0.9.0 or later

## Installation
### Hardware
Despite the fact that the nRF905 module uses 3.3V instead of 5V, we haven't experienced any issues when wiring the nRF905 directly
to the arduino instead of using voltage dividers. 

**You still have to connect the power pin (vcc) of the nRF905 module to the 3.3v pin of the arduino.**

The fritzing scheme below shows how to correctly wire your nRF905 module to the arduino. 

![fritzing scheme](doc/arduino_nRF905_wiring.png "nRF905 wiring scheme")

The table below shows all the pin connections between the nRF905 and the arduino:

| nRF905 pins | Arduino pins | 
| ----------- | ------------ |
|     VCC     |     3.3V     |
|     GND     |     GND      |
|     CSN     |     10       |
|     SCK     |     13       |
|     SI      |     11       |
|     SO      |     12       |  
|     AM      |     -        |
|     DR      |     3        |
|     CD      |     2        |
|     CE      |     7        |
|     PWR     |     8        |
|     TXE     |     9        |

### Software
The library can be imported by downloading the library as a .zip file and then add it to the Arduino IDE by using the library manager. 

Alternatively, it is also possible to add the contents of the library into the same directory as your Arduino project.
## Usage
Examples are provided to give you a quick overview of how to use the library. 
include the library into your project by adding the header file

`#include <HomeyRadio.h>`

After that you need to create a Homey-Radio instance with a desired receiver address between 0x00 and 0xFE.
Then call the initialize method to initialize the nRF905 module. A example is shown below.
```
#include <HomeyRadio.h>

Homey::Radio radio(0x50);

void setup() {
    radio.initialize();
    radio.listeningMode();
}
```
At this point, the nRF905 is set into listening mode and ready to receive data from Homey.
Data can be received by calling the `getData(data, size, srcAddress)` method or transmitted by using the `send(destAddress, data, size)`.


### Methods

- `void Radio::initialize(void)`
    * **returning:**      -
    * **Description:**  Initializing nRF905 module
    * **Parameters:** -
    

- `bool Radio::send(byte destAddress, void* data, byte size)`
    * **returning:**  True if Homey has succesfully received the data
    * **Description:**  Method to send data to a Homey
    * **Parameters:**
        * ***address:** address of the Homey device between 0x00 and 0xFE
        * ***data:** pointer to a byte-array
        * ***size:** size of byte-array
    
- `bool Radio::getData(byte* srcAddress, void* data, byte size)`
    * **returning:**  True when nRF905 has successfully received data from Homey
    * **Description:**  Method to listen to data from a Homey
    * **Parameters:**
        * **address:** address of the Homey device
        * **data:** pointer to a byte-array
        * **size:** size of byte-array

- `void Radio::listeningMode(void)`
    * **returning:**  -
    * **Description:**  enable nRF905 module to receive data
    * **Parameters:** -

- `void Radio::enable(void)`
    * **returning:**  -
    * **Description:**  power up nRF905 module 
        (you do not have to call .initialize() and .listeningMode() again)
    * **Parameters:** -

- `void Radio::disable(void)`
    * **returning:**  -
    * **Description:**  power down nRF905 module 
    * **Parameters:** -
    
## Examples
The library includes several examples which can be found in the examples folder. Three examples are provided which explain respectively sending, receiving and requests,

- examples/hello_world
- examples/print_data

## Homey App driver
A Nodejs Homey driver is written which can be included in your Homey app project. The driver includes a signal-definition and message acking.  The driver can be found [here](https://github.com/athombv/node-homey-arduinodriver).

## Contributing
Contribute to this library by sending me pull-requests.

## Credits
Special credit to Zak Kemble for his nRF905 driver
