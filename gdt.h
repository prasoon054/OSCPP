#ifndef GDT_H
#define GDT_H
class SD
{
    private:
        unsigned short int limit_lo;   // Lower 16 bits of the segment limit
        unsigned short int base_lo;    // Lower 16 bits of the segment base address
        unsigned char base_hi;         // Next 8 bits of the segment base address
        unsigned char type;            // Type of segment (code, data, etc.)
        unsigned char limit_hi;        // Upper 4 bits of the segment limit
        unsigned char base_vhi;        // Upper 8 bits of the segment base address

    public:
        SD(unsigned int base, unsigned int limit, unsigned char type);
        unsigned int Base();
        unsigned int Limit();
} __attribute__((packed)); // Ensures that the structure is packed without padding between fields

class GDT
{
    private:
        SD nullSegmentSelector;
        SD unusedSegmentSelector;
        SD codeSegmentSelector;
        SD dataSegmentSelector;
    public:
        GDT();
        ~GDT();
        unsigned short int CSS();
        unsigned short int DSS();
};
#endif
