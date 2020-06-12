
#ifdef LINUX
/*

  Implement wrapper for Arduino environment

  Compile on Linux: c++ main.cpp tu58_driver.cpp Serial.cpp -DLINUX


  Run as sudo to be able to open the serial port.

*/
#include "Serial.h"
#include <ctype.h>
#include "arduino.h"

#define isDigit isdigit



void setup();
void loop();


class Serial Serial ("/dev/ttyUSB0");

class Serial Serial1("/dev/ttyUSB0");

int main (int argc, char ** argv) {

  setup();
  do {
    loop();
  }
  while (1);
  return 0;
}


#include "tu58dump.ino"

#endif
