#!/bin/bash
gcc -I ../linux/include/uapi -o ecp3_eeprom tbsecp3-eep.c re_wr_file.c 
