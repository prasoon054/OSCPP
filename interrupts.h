#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "gdt.h" 
#include "port.h" 

class InterruptManager;

class InterruptHandler {
protected:
    unsigned char InterruptNumber; // The specific interrupt number this handler manages.
    InterruptManager* interruptManager; // Pointer to the InterruptManager managing this handler.

    // Constructor to initialize the interrupt number and the interrupt manager.
    InterruptHandler(InterruptManager* interruptManager, unsigned char InterruptNumber);

    // Destructor to clean up the handler.
    ~InterruptHandler();

public:
    // Virtual function to handle an interrupt, which subclasses must implement.
    virtual unsigned int HandleInterrupt(unsigned int esp);
};

// The InterruptManager class handles the routing and management of interrupts.
class InterruptManager {
    friend class InterruptHandler; // Allows InterruptHandler to access private members.

protected:
    static InterruptManager* ActiveInterruptManager; // Pointer to the currently active InterruptManager.

    InterruptHandler* handlers[256]; // Array of handlers for all 256 possible interrupts.

    // Structure defining an entry in the Interrupt Descriptor Table (IDT).
    struct GateDescriptor {
        unsigned short int handlerAddressLowBits; // Lower 16 bits of the handler address.
        unsigned short int gdt_codeSegmentSelector; // Code segment selector in the GDT.
        unsigned char reserved; // Reserved byte, unused.
        unsigned char access; // Access flags defining privilege level and type.
        unsigned short int handlerAddressHighBits; // Upper 16 bits of the handler address.
    } __attribute__((packed)); // Ensures the structure is packed without padding.

    static GateDescriptor interruptDescriptorTable[256]; // Array representing the IDT.

    // Structure to define a pointer to the IDT with its size and base address.
    struct InterruptDescriptorTablePointer {
        unsigned short int size; // Size of the IDT.
        unsigned int base; // Base address of the IDT.
    } __attribute__((packed)); // Ensures this structure is also packed without padding.

    unsigned short int hardwareInterruptOffset; // Base offset for hardware interrupts.

    // Sets an entry in the IDT with the given attributes.
    static void SetInterruptDescriptorTableEntry(unsigned char interrupt,
        unsigned short int codeSegmentSelectorOffset, void (*handler)(),
        unsigned char DescriptorPrivilegeLevel, unsigned char DescriptorType);

    // Default handler for interrupts that are ignored.
    static void InterruptIgnore();

    // Handlers for specific hardware interrupt requests (IRQs).
    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();
    static void HandleInterruptRequest0x02();
    static void HandleInterruptRequest0x03();
    static void HandleInterruptRequest0x04();
    static void HandleInterruptRequest0x05();
    static void HandleInterruptRequest0x06();
    static void HandleInterruptRequest0x07();
    static void HandleInterruptRequest0x08();
    static void HandleInterruptRequest0x09();
    static void HandleInterruptRequest0x0A();
    static void HandleInterruptRequest0x0B();
    static void HandleInterruptRequest0x0C();
    static void HandleInterruptRequest0x0D();
    static void HandleInterruptRequest0x0E();
    static void HandleInterruptRequest0x0F();
    static void HandleInterruptRequest0x31();

    // Handlers for CPU exceptions.
    static void HandleException0x00();
    static void HandleException0x01();
    static void HandleException0x02();
    static void HandleException0x03();
    static void HandleException0x04();
    static void HandleException0x05();
    static void HandleException0x06();
    static void HandleException0x07();
    static void HandleException0x08();
    static void HandleException0x09();
    static void HandleException0x0A();
    static void HandleException0x0B();
    static void HandleException0x0C();
    static void HandleException0x0D();
    static void HandleException0x0E();
    static void HandleException0x0F();
    static void HandleException0x10();
    static void HandleException0x11();
    static void HandleException0x12();
    static void HandleException0x13();

    // Static method to handle a generic interrupt and modify the stack pointer (ESP).
    static unsigned int HandleInterrupt(unsigned char interrupt, unsigned int esp);

    // Instance-level method to perform the actual interrupt handling for a specific interrupt.
    unsigned int DoHandleInterrupt(unsigned char interrupt, unsigned int esp);

    // Ports for configuring and managing the Programmable Interrupt Controller (PIC).
    Port8BitSlow picMC; // Master PIC command port.
    Port8BitSlow picMD; // Master PIC data port.
    Port8BitSlow picSC; // Slave PIC command port.
    Port8BitSlow picSD; // Slave PIC data port.

public:
    // Constructor initializes the interrupt manager with the hardware interrupt offset and GDT.
    InterruptManager(unsigned short int hardwareInterruptOffset, GDT* globalDescriptorTable);

    // Destructor to clean up resources.
    ~InterruptManager();

    // Returns the hardware interrupt offset.
    unsigned short int HardwareInterruptOffset();

    // Activates the interrupt manager, enabling interrupt handling.
    void Activate();

    // Deactivates the interrupt manager, disabling interrupt handling.
    void Deactivate();
};

#endif
