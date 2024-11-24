/*
IOPort Class:
    Represents a generic I/O port for hardware communication.
    The portnumber variable stores the I/O port address.
    Acts as the base class for other specific port classes.

Port8Bit Class:
    Inherits from IOPort and provides functionality to read from and write to an 8-bit port.
    Write and Read functions utilize low-level methods (Write8 and Read8) for I/O operations.

Port8BitSlow Class:
    Inherits from Port8Bit and introduces a slower variant of writing to an 8-bit port.
    Uses Write8Slow for the write operation, which presumably adds delay or ensures compatibility with slower hardware.

Inheritance Hierarchy:
    IOPort is the base class, handling basic port number initialization.
    Port8Bit extends IOPort to add read/write methods for 8-bit ports.
    Port8BitSlow further extends Port8Bit for slower write operations.
*/


#include "port.h"

IOPort::IOPort(unsigned short int portnumber)
{
    this->portnumber = portnumber;
}

IOPort::~IOPort() {}

Port8Bit::Port8Bit(unsigned short int portnumber): IOPort(portnumber) {}

Port8Bit::~Port8Bit() {}

void Port8Bit::Write(unsigned char data)
{
    Write8(portnumber, data);
    // Writes an 8-bit value (`data`) to the specified port number.
    // The `Write8` function (likely implemented elsewhere) handles the actual low-level port write.
}

unsigned char Port8Bit::Read()
{
    return Read8(portnumber);
    // Reads an 8-bit value from the specified port number.
    // The `Read8` function (likely implemented elsewhere) performs the actual low-level port read.
}

Port8BitSlow::Port8BitSlow(unsigned short int portnumber): Port8Bit(portnumber) {}
// Constructor for the `Port8BitSlow` class, which inherits from `Port8Bit`.
// Uses initializer list to pass the port number to the base class `Port8Bit`.

Port8BitSlow::~Port8BitSlow() {}
// Destructor for the `Port8BitSlow` class.
// No specific cleanup is performed.

void Port8BitSlow::Write(unsigned char data)
{
    Write8Slow(portnumber, data);
    // Writes an 8-bit value (`data`) to the port with additional delay or slow processing.
    // The `Write8Slow` function (likely implemented elsewhere) manages the actual write operation with the slowdown.
}
