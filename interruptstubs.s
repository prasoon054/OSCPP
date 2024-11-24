.set IRQ_BASE, 0x20                     # Define IRQ base as 0x20, which is the base of IRQ vector addresses

.section .text                          # Begin the section that contains executable code

.extern _ZN16InterruptManager15HandleInterruptEhj  # Declare an external symbol for the C++ interrupt handler function

.macro HandleException num                                      # Define a macro to handle exceptions with the given exception number 'num'
.global _ZN16InterruptManager19HandleException\num\()Ev         # Declare the global handler function for this exception number
_ZN16InterruptManager19HandleException\num\()Ev:                # Define the handler function for this exception number
    movb $\num, (interruptnumber)                               # Move the exception number into the interruptnumber variable
    jmp int_bottom                                              # Jump to the 'int_bottom' label to perform common interrupt processing
.endm                                                           # End of the macro definition

.macro HandleInterruptRequest num                               # Define a macro to handle interrupt requests with the given IRQ number 'num'
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev  # Declare the global handler function for this IRQ number
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:         # Define the handler function for this IRQ number
    movb $\num + IRQ_BASE, (interruptnumber)                    # Move the IRQ number, adjusted by IRQ_BASE, into the interruptnumber variable
    jmp int_bottom                                              # Jump to the 'int_bottom' label for common interrupt handling
.endm                                                           # End of the macro definition

# Handle a series of exceptions (0x00 to 0x13)
HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0A
HandleException 0x0B
HandleException 0x0C
HandleException 0x0D
HandleException 0x0E
HandleException 0x0F
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13

# Handle a series of interrupt requests (0x00 to 0x0F and an additional IRQ 0x31)
HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0A
HandleInterruptRequest 0x0B
HandleInterruptRequest 0x0C
HandleInterruptRequest 0x0D
HandleInterruptRequest 0x0E
HandleInterruptRequest 0x0F
HandleInterruptRequest 0x31                                    # Handle an additional IRQ 0x31

int_bottom:  # Common processing point for all interrupts and exceptions

    # Save the registers to preserve their values across the interrupt handler
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    # Load the segment registers for ring 0 (kernel mode)
    # Note: These lines are commented out, suggesting they are not in use here
    # cld
    # mov $0x10, %eax
    # mov %eax, %eds
    # mov %eax, %ees

    # Call the C++ interrupt handler function
    pushl %esp                                      # Save the stack pointer before passing it to the handler
    push (interruptnumber)                          # Push the interrupt number to the stack for the handler
    call _ZN16InterruptManager15HandleInterruptEhj  # Call the handler function
    add %esp, 6                                     # Adjust the stack pointer after the function call
    mov %eax, %esp                                  # Restore the stack pointer from the returned value (this is a context switch)

    # Restore the registers that were saved earlier
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

.global _ZN16InterruptManager15InterruptIgnoreEv    # Declare the interrupt ignore function globally
_ZN16InterruptManager15InterruptIgnoreEv:           # Define the function to ignore the interrupt
    iret                                            # Return from interrupt (this effectively ignores the interrupt)

.data  # Begin the section for data storage
    interruptnumber: .byte 0  # Reserve a byte of memory to store the interrupt number
