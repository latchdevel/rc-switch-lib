# RCSwitch Common Library

Adaptation of ["RCSwitch"](https://github.com/sui77/rc-switch) Arduino library to be used on any system to encode and decode RC codes of supported protocols.

Works on any **libc++** compatible system, such as macOS, FreeBSD, Linux, and even Windows.

[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![Build tests](https://github.com/latchdevel/rc-switch-lib/actions/workflows/BuildTest.yml/badge.svg)](https://github.com/latchdevel/rc-switch-lib/actions/workflows/BuildTest.yml)

Originally developed for Arduino framework, it has been ported to Raspberry Pi using the WiringPi library.

Its main use is to capture and reproduce operation codes of radio control devices like the popular low cost power outlet sockets, which works on the 315Mhz or 433MHz frequency in AM ASK/OOK modulation.

It supports only a few very basic protocols (up to 64 pulses):
 - SC5262 / SC5272
 - HX2262 / HX2272
 - PT2262 / PT2272
 - EV1527 / RT1527 / FP1527 / HS1527 
 - Intertechno outlets
 - HT6P20X

There are other similar projects that support a large number of protocols, including some very complex ones, with a large number of pulses, such as [**PiCode**](https://github.com/latchdevel/PiCode). However, RCSwitch is very popular for its simplicity and ease of use.

## Usage
RCSwitch Common Library is a C++ library built statically and dynamically on any **libc++** compatible system.

It implements the original RCSwitch class, simulating the transmissions and receptions of RC codes, exchanging objects of the [**std::list** class](https://en.cppreference.com/w/cpp/container/list) that store the pulse trains.

```c++
typedef std::list<unsigned int> pulse_list_t;
```

### Encode
You can use any transmission RCSwitch class method to encode an RC code into a pulse train.

```c++
    pulse_list_t switchOn(int nGroupNumber, int nSwitchNumber);
    pulse_list_t switchOff(int nGroupNumber, int nSwitchNumber);
    pulse_list_t switchOn(const char* sGroup, int nSwitchNumber);
    pulse_list_t switchOff(const char* sGroup, int nSwitchNumber);
    pulse_list_t switchOn(char sFamily, int nGroup, int nDevice);
    pulse_list_t switchOff(char sFamily, int nGroup, int nDevice);
    pulse_list_t switchOn(const char* sGroup, const char* sDevice);
    pulse_list_t switchOff(const char* sGroup, const char* sDevice);
    pulse_list_t switchOn(char sGroup, int nDevice);
    pulse_list_t switchOff(char sGroup, int nDevice);
    pulse_list_t sendTriState(const char* sCodeWord);
    pulse_list_t send(unsigned long code, unsigned int length);
    pulse_list_t send(const char* sCodeWord);
```

All related TX methods are functional:
```c++
    void setProtocol(int nProtocol);
    void setPulseLength(uint16_t nPulseLength);
    void setRepeatTransmit(int nRepeatTransmit);
```

### Decode
To decode a pulse train, a specific method has been added to the RCSwitch class that takes a pulse train as a parameter, simulating its reception as an AM ASK/OOK radio frequency receiver.
```c++
    bool decodePulseTrain(pulse_list_t);
```
Returns a bool indicating whether it was successfully decoded, and any case all related RX methods are functional:
```c++
    bool available();
    void resetAvailable();
    unsigned long getReceivedValue();
    unsigned int getReceivedBitlength();
    unsigned int getReceivedDelay();
    unsigned int getReceivedProtocol();
    unsigned int* getReceivedRawdata();
    void setReceiveTolerance(int nPercent);
```

## BUILD
No external depends, can run on any libc++ compatible system, such as macOS, FreeBSD, Linux, and even Windows.

Requires a **C++** compiler, **CMake** and **git** optionally. Debian linux based example: `apt install build-essential cmake git`

 ```
    $ git clone https://github.com/latchdevel/rc-switch-lib (or download .zip)
    $ cd rc-switch-lib
    $ mkdir build
    $ cd build
    $ cmake .. (or "cmake -DCMAKE_BUILD_TYPE=debug .." for debug)
    $ make
    $ make install (optional)
    $ make uninstall (to uninstall)
```

### Output
Example of use: `$ ./example`
```
Encoding: switchOn("11111", "00010")
raw_pulses[50]={350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,1050,350,350,1050,1050,350,350,1050,1050,350,350,1050,350,1050,350,1050,1050,350,350,1050,350,1050,350,1050,1050,350,350,10850};
OK: decode value: 5393 (24 bits) Protocol: 1 Delay: 350
dec_pulses[50]={350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,350,1050,1050,350,350,1050,1050,350,350,1050,1050,350,350,1050,350,1050,350,1050,1050,350,350,1050,350,1050,350,1050,1050,350,350,10850};
OK: pulse lists match.
```

# License
Copyright (c) 2011 Suat Özgür. All right reserved.

Copyright (c) 2024 Jorge Rivera. All right reserved.

License GNU Lesser General Public License v3.0.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

See the [LICENSE](LICENSE.txt) file for license rights and limitations (lgpl-3.0).