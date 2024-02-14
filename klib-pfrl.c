/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        Library Name:           Klib-pfrl (for RealDigital Blackboard peripherals)
        
        Version:                1.0.0 (Source file)

        Contributor(s):         Koby Miller

        Date last modified:     February 13th, 2024

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

#include "klib-pfrl.h"
#include "klib-io.h"
#include <sys/_intsup.h>
#include <sys/unistd.h>

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        getKeypadRaw();

        Returns a 16-bit number corresponding to which buttons on the keypad are pressed 

        Written by Koby Miller
        Last modified: February 9th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
unsigned int getKeypadRaw() {

    unsigned int data = 0; // data to be returned
    byte c; // iterator

    for(c = 0; c < 4; c++) {
        pmodWrite(0, ~(0b1000 >> c));
        data <<= 4;        
        data |= pmodRead(1) & 0xF;
        //usleep(1);
    }

    data = (~data) & 0xFFFF;

    return data;
}//end getKeypad()

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        getKeypad();

        Returns the currently pressed value on the keypad (in hexadecimal), setup as:

            1   2   3   A               *   *   *   *   R3
            4   5   6   B               *   *   *   *   R2
            7   8   9   C               *   *   *   *   R1
            0   F   E   D               *   *   *   *   R0
                                        C3  C2  C1  C0

        Written by Koby Miller
        Last modified: February 9th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
int getKeypad() {
    
    unsigned int pressed = getKeypadRaw();
    byte place = 0;
    
    int keys[17] = {        0xD, 0xC, 0xB, 0xA,     // Array of keypad values with their respective keys
                            0xE, 9,   6,   3,       // However, notice that the array is mirrored and rotated
                            0xF, 8,   5,   2,
                            0,   7,   4,   1,
                        
                            -1 };   

    while(bitIndex(pressed, place) == 0 && place < 16) { // scan by bitIndex which buttons are pressed, incrementing until pressed button is found
        place += 1;
    }

    return keys[place];
}


