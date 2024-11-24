#ifndef PORT_H
#define PORT_H
// Header guard to prevent multiple inclusions of this header file.

class IOPort {
protected:
    IOPort(unsigned short int portnumber);
    // Constructor for the base class `IOPort`. It initializes the `portnumber`.

    ~IOPort();
    // Destructor for the base class `IOPort`.

    unsigned short int portnumber;
    // Stores the port number for hardware I/O operations.
};

class Port8Bit : public IOPort {
public:
    Port8Bit(unsigned short int portnumber);
    // Constructor for the `Port8Bit` class, passes the port number to the base class `IOPort`.

    ~Port8Bit();
    // Destructor for the `Port8Bit` class.

    virtual unsigned char Read();
    // Reads an 8-bit value from the I/O port.

    virtual void Write(unsigned char data);
    // Writes an 8-bit value to the I/O port.

protected:
    static inline unsigned char Read8(unsigned short int _port) {
        unsigned char result;
        __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));
        // Inline assembly to perform an 8-bit input from an I/O port:
        // - `"inb %1, %0"`: The `inb` instruction reads a byte from the port specified in `%1` (_port)
        //   and stores it in `%0` (result).
        // - `: "=a" (result)`: The output operand. `=a` means store the result in the `AL` register.
        // - `: "Nd" (_port)`: The input operand. `Nd` allows using a constant or `DX` register for the port number.
        return result;
    }

    static inline void Write8(unsigned short int _port, unsigned char _data) {
        __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
        // Inline assembly to perform an 8-bit output to an I/O port:
        // - `"outb %0, %1"`: The `outb` instruction writes the byte in `%0` (_data)
        //   to the port specified in `%1` (_port).
        // - `: "a" (_data)`: The input operand. The value to write is in the `AL` register.
        // - `: "Nd" (_port)`: The input operand. The port number is provided as a constant or `DX` register.
    }
};

class Port8BitSlow : public Port8Bit {
public:
    Port8BitSlow(unsigned short int portnumber);
    // Constructor for the `Port8BitSlow` class, passes the port number to the base class `Port8Bit`.

    ~Port8BitSlow();
    // Destructor for the `Port8BitSlow` class.

    virtual void Write(unsigned char data);
    // Writes an 8-bit value to the I/O port with additional delay.

protected:
    static inline void Write8Slow(unsigned short int _port, unsigned char _data) {
        __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
        // Inline assembly to perform an 8-bit output with added delay:
        // - `"outb %0, %1"`: Write the byte in `%0` (_data) to the port specified in `%1` (_port).
        // - `\njmp 1f\n1: jmp 1f\n1:`: Inserts two dummy jump instructions to create a short delay.
        //   - `1:`: A label used to mark a location in the assembly code.
        //   - `jmp 1f`: Jumps to the label `1`.
        //   - The sequence of jumps introduces a small time delay.
        // - `: "a" (_data)`: The value to write is in the `AL` register.
        // - `: "Nd" (_port)`: The port number is provided as a constant or `DX` register.
    }
};

#endif
// End of the header guard.
