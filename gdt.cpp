#include "gdt.h"

// Constructor for GDT class
GDT::GDT()
    // Initializing the segment selectors with specific parameters.
    : nullSegmentSelector(0, 0, 0),                      // null segment with base 0, limit 0, type 0
      unusedSegmentSelector(0, 0, 0),                    // unused segment with base 0, limit 0, type 0
      codeSegmentSelector(0, 64 * 1024 * 1024, 0x9A),    // code segment with base 0, 64MB limit, type 0x9A (code segment, read/write, accessed)
      dataSegmentSelector(0, 64 * 1024 * 1024, 0x92)     // data segment with base 0, 64MB limit, type 0x92 (data segment, read/write, accessed)
{
    unsigned int i[2];
    i[1] = (unsigned int)this;                       // Store the address of this GDT object in i[1]
    i[0] = sizeof(GDT) << 16;                        // Store the size of the GDT shifted by 16 bits in i[0]
    // Inline assembly to load the GDT (Global Descriptor Table)
    asm volatile("lgdt (%0)" : : "p"(((unsigned char *)i) + 2));  // Load the GDT into the processor's GDTR register
}

// Destructor for GDT class (empty as there's no dynamic memory to free)
GDT::~GDT()
{}

// Returns the offset of the Data Segment Selector (DSS) within the GDT object
unsigned short int GDT::DSS()
{
    return (unsigned char *)&dataSegmentSelector - (unsigned char *)this;  // Calculate the offset of the data segment
}

// Returns the offset of the Code Segment Selector (CSS) within the GDT object
unsigned short int GDT::CSS()
{
    return (unsigned char *)&codeSegmentSelector - (unsigned char *)this;  // Calculate the offset of the code segment
}

// Constructor for SD (Segment Descriptor) class
SD::SD(unsigned int base, unsigned int limit, unsigned char type)
{
    unsigned char *target = (unsigned char *)this;  // Treat the SD object as a byte array for manipulation

    // Set the 6th byte based on the limit size (16-bit or 32-bit address space)
    target[6] = (limit <= 65536) ? 0x40 : 0xC0;   // If limit <= 65536, set it to 16-bit address, else 32-bit

    // Adjust the limit if it's greater than 65536 (for 32-bit address space)
    if (limit > 65536) {
        limit = (limit & 0xFFF) ? (limit >> 12) - 1 : limit >> 12;  // Adjust the limit to be page-aligned
    }

    // Set the limit (0-15 bits) in bytes 0-1 and the 16-19 bits in byte 6
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    // Set the base address in bytes 2-7 (base is 32 bits, broken into 4 bytes)
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Set the type byte (describes the segment's properties like read/write, etc.)
    target[5] = type;
}

// Returns the base address of the segment described by this SD object
unsigned int SD::Base()
{
    unsigned char *target = (unsigned char *)this;   // Treat the SD object as a byte array
    unsigned int result = target[7];                 // Start with the highest byte (byte 7)
    result = (result << 8) + target[4];              // Add the 3rd byte (byte 4) of the base address
    result = (result << 8) + target[3];              // Add the 2nd byte (byte 3) of the base address
    result = (result << 8) + target[2];              // Add the 1st byte (byte 2) of the base address
    return result;                                   // Return the full 32-bit base address
}

// Returns the limit of the segment described by this SD object
unsigned int SD::Limit()
{
    unsigned char *target = (unsigned char *)this;  // Treat the SD object as a byte array
    unsigned int result = target[6] & 0xF;          // Start with the lower 4 bits of byte 6 (the 16-19 limit bits)
    result = (result << 8) + target[1];             // Add the 2nd byte (byte 1) of the limit
    result = (result << 8) + target[0];             // Add the 1st byte (byte 0) of the limit
    if ((target[6] & 0xC0) == 0xC0)                 // If the segment is 32-bit (0xC0 in the 6th byte)
        result = (result << 12) | 0xFFF;            // Adjust the limit to account for 4KB pages
    return result;                                  // Return the full segment limit
}
