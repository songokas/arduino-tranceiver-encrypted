/*
  DSM2_tx implements the serial communication protocol used for operating
  the RF modules that can be found in many DSM2-compatible transmitters.

  Copyrigt (C) 2012  Erik Elmore <erik@ironsavior.net>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//#pragma once
#ifndef MOCK_SERIAL
#define MOCK_SERIAL
#include <iostream>
//#include <string>
//
class __FlashStringHelper;

class Stream
{
    public:
        void print(int i) {}
};

class HardwareSerial: public Stream {
public:
  void begin(unsigned long);
  void end();
  size_t write(const unsigned char*, size_t);
  void print(float) {};
  void print(int) {};
  void print(unsigned long) {};
  void print(long) {};
  void print(char&) {};
  void print(std::string) {};
  void print(const char*) {};

  void println(float) {};
  void println(int) {};
  void println(unsigned long) {};
  void println(long) {};
  void println(char*) {};
  void println(const char*) {};
  void println(std::string) {};
  void println(const __FlashStringHelper*) {};
  void println() {};
};

extern HardwareSerial Serial;
extern HardwareSerial Serial1;

#endif
