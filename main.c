
// define some IO port
__sfr __at 0x22 SOME_IO;

main() {
    // how to write to an IO port
    SOME_IO=0xf0;

    return 0;
}



