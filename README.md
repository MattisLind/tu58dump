# tu58dump
Tool to dump TU58 tapes.

![TU58 drive](https://i.imgur.com/74cPEih.png)

A TU58 is a tape drive manufactured by DEC that takes DC100 style tapes and provide a simple but slow random access media. The tape has 2048 blocks, 128 bytes each. The tapes comes preformatted and cannot be formatted in the field. The small size of a block is because the Intel 8156 chip in the controller only has 256 bytes of RAM. 

![TU 58 tapes](https://i.imgur.com/q8ZxF0T.jpg)

This is the pile of tapes that I need to dump the contents of so that the software can be used in various emulators, like tu58em.

## Design

From Dan North I gotten the main pieces of software tu58_driver.[cpp|h] that provide a simple interface to the TU58 drive. Since the original code was written for Arduino I use a wrapper library that I made for another project to make it possible to run in a Linux environment. The main file would then read one block at a time. If successful it will write it down to file. If not it will retry a configurable number of times and then finally write an empty block to the file and continue with next block untill all blocks has been read.

