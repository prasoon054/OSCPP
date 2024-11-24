#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupts.h"
#include "port.h"

class KeyboardDriver : public InterruptHandler
// Define the `KeyboardDriver` class, which inherits from the `InterruptHandler` class.
// This indicates that the `KeyboardDriver` will handle specific interrupt events.
{
    Port8Bit dataport;
    // Member variable `dataport` represents the data port for the keyboard (typically I/O port 0x60).
    // This is used to read/write data to/from the keyboard hardware.

    Port8Bit commandport;
    // Member variable `commandport` represents the command port for the keyboard (typically I/O port 0x64).
    // This is used to send commands to the keyboard controller.

public:
    KeyboardDriver(InterruptManager* manager);
    // Constructor for the `KeyboardDriver` class. 
    // It takes a pointer to an `InterruptManager` object to register the keyboard driver as an interrupt handler.

    ~KeyboardDriver();
    // Destructor for the `KeyboardDriver` class.
    // It performs any cleanup needed when the object is destroyed.

    virtual unsigned int HandleInterrupt(unsigned int esp);
    // Virtual method to handle keyboard interrupts.
    // `esp` is the stack pointer passed during the interrupt, which may be used to 
    // modify the state of the interrupted process. This method will be called when 
    // a keyboard interrupt occurs.
};

#endif