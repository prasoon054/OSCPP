#include "interrupts.h" 
#include "gdt.h"        
#include "keyboard.h"  

void printf(char* message) {
    static unsigned short int* screenBuffer = (unsigned short int*)0xb8000; 
    static unsigned short int col = 0, row = 0; 
    for (int index = 0; message[index] != '\0'; ++index) { 
        // Loop through each character in the input string
        screenBuffer[80 * row + col] = (screenBuffer[80 * row + col] & 0xFF00) | message[index]; 
        // Set the character at the current position in video memory
        // The upper byte is preserved for text attributes; the lower byte stores the character

        if (++col == 80) { 
            // If the end of the row is reached (80 columns), move to the next line
            col = 0;
            row++;
        }
        if (row == 25) { 
            // If the screen's height limit is reached (25 rows), clear the screen
            for (unsigned short int clearRow = 0; clearRow < 25; clearRow++) { 
                // Loop through all rows
                for (unsigned short int clearCol = 0; clearCol < 80; clearCol++) { 
                    // Loop through all columns
                    screenBuffer[80 * clearRow + clearCol] = (screenBuffer[80 * clearRow + clearCol] & 0xFF00) | ' '; 
                    // Clear each character while preserving text attributes
                }
            }
            col = 0; // Reset cursor to the top-left corner
            row = 0;
        }
    }
}

// Kernel entry point function
extern "C" void kernelMain(const void* multiboot_structure, unsigned int* multiboot_magic) {
    printf("Hello World!"); 
    // Print "Hello World!" to the screen

    GDT gdt; 
    // Instantiate the Global Descriptor Table (GDT), which manages memory segments

    InterruptManager interrupts(0x20, &gdt); 
    // Instantiate the interrupt manager and set its base interrupt vector (0x20) 
    // and associate it with the GDT

    KeyboardDriver keyboard(&interrupts); 
    // Instantiate the keyboard driver and link it to the interrupt manager

    interrupts.Activate(); 
    // Activate the interrupt manager to enable hardware interrupts

    while(1); 
    // Enter an infinite loop to prevent the kernel from exiting
}
