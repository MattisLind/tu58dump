//
// RX8E/RX28/RX11/RX211/RXV11/RXV21 to TU58/serial or SDcard Interface
//
// Copyright (c) 2015-2016, Donald N North
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// DONALD N NORTH BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Except as contained in this notice, the name of Donald N North shall not be
// used in advertising or otherwise to promote the sale, use or other dealings
// in this Software without prior written authorization from Donald N North.
//
// 22-Aug-2015 donorth Initial version
//

// Stolen and modified to be a test program for TU58 - Mattis Lind 2020-06-12

//
// definitions
//

// baud rate for USB serial debug port
//
//   250Kb is highest that AVR Mega2560/16U2 combo can run (ArduinoIDE 1.8.1 or earlier)
//   2000Kb is the highest that AVR Mega2560/16U2 combo can run (ArduinoIDE 1.8.2 or later)
//
//   All 'genuine' Arduino Mega2560s use this combo, so should work ok.
//
//   Mega2560 'clone' boards NOT using the 16U2 as a USB/Serial interface
//   may have issues running at this rate. Boards will have to be tested.
//   On some of these boards 230.4K or 115.2K may be the highest possible
//   rate possible. This will have an impact if using any 'debug' mode.
//
//   Depending upon the Mega2560 board, 2Mb or 1Mb may not even work 100%
//   reliably, so if enabling those modes you must test your Arduino board
//   to see if it performs as you expect (the L command is a good test).
//
#if ARDUINO >= 10802
// IDE 1.8.2 and later enable 500Kb, 1Mb, and 2Mb baud rates.
//#define SERIAL_BAUD_RATE 2000000L
//#define SERIAL_BAUD_RATE 1000000L
//#define SERIAL_BAUD_RATE  500000L
#define SERIAL_BAUD_RATE  250000L
#else
// IDE 1.8.1 and earlier 250Kb is the max baud rate.
#define SERIAL_BAUD_RATE  250000L
#endif

#define DEBUG_TU58 1

//
// includes
//
#include "tu58_driver.h"



//
// global variables
//
HardwareSerial *tty = &Serial; // optionally &Serial1 or &Serial2 for other UART connectors


uint8_t c_run;
uint8_t c_nop;
uint8_t c_init;
uint8_t c_diag;
uint8_t c_seek;
uint8_t c_read;
uint8_t c_write;
//
uint16_t number;
uint16_t block;
uint16_t value;
//
uint8_t tmp_buffer[128];


//
// process user input
//
void run_user (char c)
{
    static char cmd[40];
    static uint8_t i = 0;

    if (isDigit(c))    { value = 10*value + (c - '0'); }
    else if (c == 'B') { tty->printf("BLOCK %u\n", value);  block = value;  value = 0; }
    else if (c == 'N') { tty->printf("NUMBER %u\n", value); number = value; value = 0; }
    else if (c == 'I') { tty->printf("INIT\n");  c_init = 1; }
    else if (c == 'R') { tty->printf("READ\n");  c_read = 1; }
    else if (c == 'W') { tty->printf("WRITE\n"); c_write = 1; }
    else if (c == 'C') { tty->printf("COPY\n");  c_read = c_write = 1; }
    else if (c == 'D') { tty->printf("DIAG\n");  c_diag = 1; }
    else if (c == 'P') { tty->printf("NOP\n");   c_nop = 1; }
    else if (c == 'S') { tty->printf("SEEK\n");  c_seek = 1; }
    else if (c == 'H') { tty->printf("HALT\n");  c_read = c_write = 0; }

    return;
}



//
// run tu58 commands
//
void run_tu58 (void)
{
    int8_t sts;

    if (true) {

        if (c_init) {
            sts = tu_init();
            if (DEBUG_TU58) tty->printf("init: status=%d\n", sts);
        }

        if (c_diag) {
            sts = tu_diag();
            if (DEBUG_TU58) tty->printf("diag: status=%d\n", sts);
        }

        if (c_nop) {
            sts = tu_nop();
            if (DEBUG_TU58) tty->printf("nop: status=%d\n", sts);
        }

        if (c_seek) {
            sts = tu_seek(0, block);
            if (DEBUG_TU58) tty->printf("seek:  unit=%d block=0x%04X status=%d\n", 0, block, sts);
        }

        if (c_read) {
            sts = tu_read(0, block, sizeof(tmp_buffer), tmp_buffer);
            if (DEBUG_TU58) {
                uint16_t i;
                tty->printf("read:  unit=%d block=0x%04X count=0x%04X status=%d\n", 0, block, sizeof(tmp_buffer), sts);
                for (i = 0; i < sizeof(tmp_buffer); ++i) {
                    if (i % 32 == 0) tty->printf("  ");
                    tty->printf(" %02X", tmp_buffer[i]);
                    if (i % 32 == 31) tty->printf("\n");
                }
            }
        }

        if (c_write) {
            sts = tu_write(1, block, sizeof(tmp_buffer), tmp_buffer);
            if (DEBUG_TU58) {
                uint16_t i;
                tty->printf("write: unit=%d block=0x%04X count=0x%04X status=%d\n", 1, block, sizeof(tmp_buffer), sts);
                for (i = 0; i < sizeof(tmp_buffer); ++i) {
                    if (i % 32 == 0) tty->printf("  ");
                    tty->printf(" %02X", tmp_buffer[i]);
                    if (i % 32 == 31) tty->printf("\n");
                }
            }
        }

        if (c_read || c_write || c_seek || c_init || c_diag || c_nop) {
            number -= 1;
            block += 1;
            if (number == 0) {
                c_read = c_write = c_seek = c_init = c_diag = c_nop = 0;
                number = 1;
            }
        }

    }

    return;
}






//
// setup routine runs once at power up
//
void setup (void)
{
    // control/status input/output
    tty->begin(SERIAL_BAUD_RATE, SERIAL_8N2);


    // init globals
    number = 1;
    block = 0;
    value = 0;
    c_write = 0;
    c_read = 0;
    c_seek = 0;
    c_diag = 0;
    c_init = 0;
    c_nop = 0;

    // init tu58 interface
    if (DEBUG_TU58) tu_debug(tty, DEBUG_TU58);
    tu_initialize(1000000L, &Serial1);

    // done
    return;
}



//
// loop routine runs continuously
//
void loop (void)
{
    // check if user typed a character
    if (tty->available()) run_user(tty->read());

    // tu58 commands
    run_tu58();

    return;
}



// the end
