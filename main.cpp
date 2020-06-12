
#ifdef LINUX
/*

  Implement wrapper for Arduino environment

  Compile on Linux: c++ main.cpp tu58_driver.cpp Serial.cpp -DLINUX  -o tu58dump


  Run as sudo to be able to open the serial port.

*/
#include "Serial.h"
#include <ctype.h>
#include "arduino.h"
#include <sys/time.h>

#define isDigit isdigit

unsigned long millis() {
  struct timeval  tv;
  gettimeofday(&tv, NULL);

long time_in_mill = 
         (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond
 return time_in_mill;
}

unsigned short word (uint8_t h, uint8_t l) {
  return ((h & 0xff) << 8) | (l & 0xff);
}



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
