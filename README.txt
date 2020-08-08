provided crt0.s in lib/src/z80 is compiled like
sdasz80 -plosgff crt0.s
(see https://sourceforge.net/p/sdcc/support-requests/104/ )

make a binary file suitable for burning to EEPROM with
objcopy -Iihex -Obinary input.ihx output.bin

Trans330 IO map

    Port $00 – PIO parallel port A.
        Bits 3-0: output, keypad columns. Bit 3 is right-most column.
        Bit 4: output, display controller reset
        Bit 5: output, display controller data
        Bit 6: output, display controller clock
        Bit 7: input, card reader data
    Port $01 – PIO control register for port A.
    Port $02 – PIO parallel port B.
        Bits 3-0: input, keypad rows. Bit 3 is bottom row.
        Bits 7-4: not connected
    Port $03 – PIO control register for port B.
    Port $10 – CTC channel 0
    Port $11 – CTC channel 1
    Port $12 – CTC channel 2. Chan 2 output is connected to speaker driver.
    Port $13 – CTC channel 3. Chan 3 input trigger is connected to chan 2 output.
    Port $20 – DART serial port A. 8-pin DIN RS-232 port.
    Port $21 – DART control register for port A.
    Port $22 – DART serial port B. 6-pin DIN accessory port.
    Port $23 – DART control register for port B.
    Ports $30 – $3F – Real-time clock registers $0-$F
    Ports $40,$50,$60,$70 – Modem?

PIO setup
port, value
0x01, 0xCF; control each port bit individually
0x01, 0x80, bit 7 is input, the rest outputs
0x01, 0x18, use interrupt vector 18
0x01, 0x97, generate input if any masked bit is low
0x01, 0x7F; interrupt mask - bit 7
0x00, 0x3F; set initial output values for port A

see https://www.bigmessowires.com/2011/05/10/mapping-the-tranz-330/ for more details
