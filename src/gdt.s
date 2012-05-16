; ASM for loading the GDT

[GLOBAL gdt_gen]
[GLOBAL idt_gen]

gdt_gen:
    mov eax, [esp+4] ; pointer to GDT passed as a param
    lgdt [eax] ; Load the GDT

    ;  The following sets up the segment registers
    mov ax, 0x10 ; Offset to the data segment desc (the third entry)
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    jmp 0x08:.done ; Far jump to set cs to 0x08 (second GDT entry)
.done:
    ret

idt_gen:
    mov eax, [esp+4] ; get the pointer to the IDT passed as a param
    lidt [eax] ; Load the IDT
    ret

