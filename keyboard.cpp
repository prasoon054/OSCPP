#include "keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
: InterruptHandler(manager, 0x21), // Initialize the base class `InterruptHandler` with interrupt number 0x21.
dataport(0x60), // Initialize `dataport` to I/O port 0x60 (keyboard data port).
commandport(0x64) // Initialize `commandport` to I/O port 0x64 (keyboard command port).
{
    while(commandport.Read() & 0x1)
        dataport.Read();
    // Clear any existing data in the keyboard buffer.
    // The condition `commandport.Read() & 0x1` checks if the output buffer is full.

    commandport.Write(0xae); // Activate keyboard interrupts.
    commandport.Write(0x20); // Send command 0x20 to read the controller command byte.

    unsigned char status = (dataport.Read() | 1) & ~0x10;
    // Read the controller command byte and modify it:
    // - Set the lowest bit (bit 0) to 1 to enable interrupts.
    // - Clear bit 4 (~0x10) to disable translation.

    commandport.Write(0x60); // Send command 0x60 to set the controller command byte.
    dataport.Write(status); // Write the modified command byte back to the controller.

    dataport.Write(0xf4); // Send command 0xF4 to the keyboard to enable scanning.
}

KeyboardDriver::~KeyboardDriver()
{}

void printf(char*);
unsigned int KeyboardDriver::HandleInterrupt(unsigned int esp)
{
    unsigned char key = dataport.Read();
    // Read the key code from the keyboard's data port.

    if(key < 0x80)
    {
        // Check if the key code is a "make" code (indicates key press).
        switch(key)
        {
            case 0x02: printf("1"); break; // Print "1" for key code 0x02.
            case 0x03: printf("2"); break; // Print "2" for key code 0x03.
            case 0x04: printf("3"); break; // Print "3" for key code 0x04.
            case 0x05: printf("4"); break; // Print "4" for key code 0x05.
            case 0x06: printf("5"); break; // Print "5" for key code 0x06.
            case 0x07: printf("6"); break; // Print "6" for key code 0x07.
            case 0x08: printf("7"); break; // Print "7" for key code 0x08.
            case 0x09: printf("8"); break; // Print "8" for key code 0x09.
            case 0x0A: printf("9"); break; // Print "9" for key code 0x0A.
            case 0x0B: printf("0"); break; // Print "0" for key code 0x0B.

            case 0x10: printf("q"); break; // Print "q" for key code 0x10.
            case 0x11: printf("w"); break; // Print "w" for key code 0x11.
            case 0x12: printf("e"); break; // Print "e" for key code 0x12.
            case 0x13: printf("r"); break; // Print "r" for key code 0x13.
            case 0x14: printf("t"); break; // Print "t" for key code 0x14.
            case 0x15: printf("z"); break; // Print "z" for key code 0x15.
            case 0x16: printf("u"); break; // Print "u" for key code 0x16.
            case 0x17: printf("i"); break; // Print "i" for key code 0x17.
            case 0x18: printf("o"); break; // Print "o" for key code 0x18.
            case 0x19: printf("p"); break; // Print "p" for key code 0x19.

            case 0x1E: printf("a"); break; // Print "a" for key code 0x1E.
            case 0x1F: printf("s"); break; // Print "s" for key code 0x1F.
            case 0x20: printf("d"); break; // Print "d" for key code 0x20.
            case 0x21: printf("f"); break; // Print "f" for key code 0x21.
            case 0x22: printf("g"); break; // Print "g" for key code 0x22.
            case 0x23: printf("h"); break; // Print "h" for key code 0x23.
            case 0x24: printf("j"); break; // Print "j" for key code 0x24.
            case 0x25: printf("k"); break; // Print "k" for key code 0x25.
            case 0x26: printf("l"); break; // Print "l" for key code 0x26.

            case 0x2C: printf("y"); break; // Print "y" for key code 0x2C.
            case 0x2D: printf("x"); break; // Print "x" for key code 0x2D.
            case 0x2E: printf("c"); break; // Print "c" for key code 0x2E.
            case 0x2F: printf("v"); break; // Print "v" for key code 0x2F.
            case 0x30: printf("b"); break; // Print "b" for key code 0x30.
            case 0x31: printf("n"); break; // Print "n" for key code 0x31.
            case 0x32: printf("m"); break; // Print "m" for key code 0x32.
            case 0x33: printf(","); break; // Print "," for key code 0x33.
            case 0x34: printf("."); break; // Print "." for key code 0x34.
            case 0x35: printf("-"); break; // Print "-" for key code 0x35.

            case 0x1C: printf("\n"); break; // Print a newline for key code 0x1C.
            case 0x39: printf(" "); break; // Print a space for key code 0x39.

            default:
            {
                // Handle unknown key codes.
                char* foo = "KEYBOARD 0x00 ";
                char* hex = "0123456789ABCDEF";
                foo[11] = hex[(key >> 4) & 0xF]; // Get the high nibble of the key code as a hex character.
                foo[12] = hex[key & 0xF];       // Get the low nibble of the key code as a hex character.
                printf(foo);                    // Print the unknown key code in hexadecimal.
                break;
            }
        }
    }
    return esp; // Return the stack pointer.
}
