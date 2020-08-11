
// IO port definitions
__sfr __at 0x00 PIO_PORT_A; // kbd cols, disp, card reader
__sfr __at 0x01 PIO_CTRL_A;
__sfr __at 0x02 PIO_PORT_B; // kbd rows
__sfr __at 0x03 PIO_CTRL_B;

__sfr __at 0x10 CTC0;
__sfr __at 0x11 CTC1;
__sfr __at 0x12 CTC2; // speaker
__sfr __at 0x13 CTC3; // input tied to CTC2 output

__sfr __at 0x20 DART_PORT_A; // 8-pin rs232
__sfr __at 0x21 DART_CTRL_A;
__sfr __at 0x22 DART_PORT_B; // 6-pin acc. port
__sfr __at 0x23 DART_CTRL_B;

__sfr __at 0x30 RTC_BASE; // RTC is 0x30 to 0x3F

setup_PIO() {
    // setup PIO port A
    PIO_CTRL_A=0xCF; // control each port bit individually
    PIO_CTRL_A=0x80; // bit 7 is input, the rest outputs
    PIO_CTRL_A=0x18; // use interrupt vectr 18
    PIO_CTRL_A=0x97; // generate input if any masked bit is low
    PIO_CTRL_A=0x7F; // interrupt mask - bit 7
    PIO_PORT_A=0x3F; // set initial output values for port A

    // setup PIO port B
    PIO_CTRL_B=0xCF; // control each port bit individually
    PIO_CTRL_B=0x0F; // bottom nibble is input, top is output
    PIO_CTRL_B=0x28; // interrupt vector 28 (TODO check this!!)
    PIO_CTRL_B=0x97; // generate IRQ if anymasked bit is low (TODO look up how to make this IRQ mask for bit being high!!!)
    PIO_CTRL_B=0x0F; // IRQ mask - lower nibble
    PIO_PORT_B=0x00; // initial outputs all low (TODO check this!!!)
}

setup_display() {
    // TODO 
    // "MSB of 1 is control byte, MSB of 0 is data byte"
    byte current = PIO_PORT_A;
    PIO_PORT_A = 0xEF & current; // clear bit 4 (display reset)
    // TODO delay 0x1C cycles
    PIO_PORT_A = 0x10 | current; // set bit 4 (display reset)
    display_send_byte(0xFF); // set display duty cycle to 31 (max brightness)
}

display_send_byte(byte x) {
    // shift out byte on bit 5, MSB first, toggling clock (bit 6)
    byte c = 0;
    for( c=0; c<8; c++) {
        if( x & 0x80) {
            // TODO set data bit
        } else {
            // TODO clear data bit
        }
        // TODO toggle clock line
        x = x << 1;
    }
}

main() {
    // setup keypad and display
    // display hello world
    // wait for keypress
    // display a number
    // wait for keypress
    // increment number
    // loop up two

    return 0;
}



