
void display_send_byte_asm(char x);

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

void setup_PIO() {
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

// void display_send_byte(char x) {
//     // shift out byte on bit 5, MSB first, toggling clock (bit 6)
//     char c = 0;
//     for( c=0; c<8; c++) {
//         if( x & 0x80) {
//             // TODO set data bit
//         } else {
//             // TODO clear data bit
//         }
//         // TODO toggle clock line
//         x = x << 1;
//     }
// }

// void setup_display() {
//     // TODO 
//     // "MSB of 1 is control byte, MSB of 0 is data byte"
//     char current = PIO_PORT_A;
//     PIO_PORT_A = 0xEF & current; // clear bit 4 (display reset)
//     // TODO delay 0x1C cycles
//     PIO_PORT_A = 0x10 | current; // set bit 4 (display reset)
//     display_send_byte(0xFF); // set display duty cycle to 31 (max brightness)
// }

void setup_display_asm() {
    __asm
    in a, (#0)  ; get current port state
    and #0xEF   ; clear bit 4 (display reset)
    out (#0), A
    ld b, #0x1c ; wait 0x1c cycles
00001$:
    djnz 00001$
    or #0x10    ; set bit 4 (display reset)
    out (#0), a
    __endasm;

    // set the display duty cycle to 31 (max brightness)
    display_send_byte_asm(0xff);
}

void display_send_byte_asm(char x) {
    // sdcc ASM notes:
    // see - https://gist.github.com/Konamiman/af5645b9998c802753023cf1be8a2970
    // Decimal numeric constants must be preceded with #
    // Hexadecimal numeric constants must be preceded with #0x
    // The syntax for offsets when using index registers is n(ix), where in other assemblers it's usually (ix+n)
    x; // nop to silence compiler usage complaint
    __asm
    ld iy, #2
    add iy, sp  ; bypass the return address of the function in the stack

    ld c, (iy)  ; C should contain param x

    ; (following lifted from BMOW, and translated to SDCCs inline asm)
    ld b, #8 ; 8 bits to send
00001$: 
    IN A, (#0)   ; get current port state
    RLA     ; rotate the port word until the data bit is in the carry flag
    RLA
    RLA
    RL C    ; shift the next output data bit into the carry flag
    RRA     ; rotate the port word until the data bit is in bit 5
    RRA
    RRA
    out (#0), A  ; setup the data output (bit 5)
    or #0x40        ; set clock high (bit 6)
    out (#0), A
    and #0xBF       ; set clock low (bit 6)
    out (#0), A
    DJNZ 00001$       ; continue with the next bit
    __endasm;
}

void display(char *msg) {
    while(*msg) {
        display_send_byte_asm(*msg);
        msg += 1;
    }
}

// #define NO_KEY 0xFF
// char get_keypress() {
//     // TODO scan the keyboard once and return 0x0 to 0xF
//     return NO_KEY;
// }

// const char zero[] = "0";
// char *itoa( int val) {
//     // TODO do the modulo thing to convert to a string
//     val; // silence compiler for now
//     return zero;
// }

// string constants we want in ROM
const char msg1[] = "HELLO WORLD";

void main() {
    // setup keypad and display
    setup_PIO();
    setup_display_asm();
    // display hello world
    display(msg1);
    
    // int c = 0;
    // do {
    //     c += 1;
    //     // wait for keypress
    //     while(get_keypress() == NO_KEY) {
    //         __asm__("nop");
    //     }
    //     // display a number
    //     display(itoa(c));
    // } while(1);
}
