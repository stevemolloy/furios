; boot.s -- Kernel start location. Also defines multiboot header.
; Based on Bran's kernel development tutorial file start.asm
;
[BITS 32]                       ; All instructions should be 32-bit.
[section .setup]
[GLOBAL start]                  ; Kernel entry point.
[EXTERN main]                   ; This is the entry point of our C code

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

ALIGN 4
mboot:
  dd  MBOOT_HEADER_MAGIC        ; GRUB will search for this value on each
                                ; 4-byte boundary in your kernel file
  dd  MBOOT_HEADER_FLAGS        ; How GRUB should load your file / settings
  dd  MBOOT_CHECKSUM            ; To ensure that the above values are correct
   
; The OS code starts here!!
start:
  push    ebx                   ; Preserve multiboot header location

  mov ebx, page_dir             ; EBX now points at the page directory
  push ebx                      ; Preserve page_dir address to pass to the kernel
  mov edx, page_table0          ; EDX now points at the first table

; Loop to zero the page directory (to be filled later) and fill table0
  xor eax, eax                  ; set loop counter to zero
  mov ecx, 3                    ; ecx = PRESENT | WRITEABLE
table_loop:                     ; Loop around eax
  mov dword [edx + eax*4], ecx  ; EAX'th table entry = ECX (stepped by 4k each time)
  add ecx, 0x1000               ; Increment ECX by 4096 (the size of the page)
  inc eax                       ; EAX++
  cmp eax, 0x3ff                ; if EAX<1024 ...
  jle table_loop                ; loop again

  or edx, 3                     ; switch on PRESENT and WRITEABLE
  mov [ebx], edx                ; page_dir[0] = page_table0
  mov [ebx + 0xc00], edx        ; page_dir[0xC00] = page_table0
                                ; Since kernel is linked at 0xC0000000

; The following sets the appropriate registers to turn on 32bit paging
  mov cr3, ebx                  ; Set the paging pointer register
  mov eax, cr0
  or eax, 0x80010000            ; Set the appropriate bits in CR0
  mov cr0, eax                  ; And turn on paging!

  lgdt [gdt]                    ; Load the GDT ...
  mov ax, 0x10                  ; ... and set the registers
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:kernelcall           ; Far jump to set cs

ALIGN 4096
page_dir:
  times 4096 db 0                ; Reserve 4kB for the page directory

ALIGN 4096
page_table0:
  times 4096 db 0                ; Reserve 4kB for the first page (to map 1st 4MB)

gdt:
  dw gdt_end - flatgdt - 1        ; Size of the GDT
  dd flatgdt                      ; Address of the GDT

flatgdt:    ; A flat memory model GDT.  This needs to be expanded to allow ring 3
  dd 0, 0                     ; Null gate
  db 0xFF, 0xFF, 0, 0, 0, 10011010b, 11001111b, 0
  db 0xFF, 0xFF, 0, 0, 0, 10010010b, 11001111b, 0
gdt_end:

[section .text]
; Here is the higher-half kernel code
kernelcall:
  ; Execute the kernel:
  cli                         ; Disable interrupts.
  pop ebx                     ; Recover page_dir address and the...
  pop edx                     ; ...mboot header before moving to higher-half stack
  mov esp, sys_stack          ; set up a new stack for the higher-half kernel
  push ebx                    ; Push page_dir address onto the new stack
  push edx                    ; Push the multiboot info back onto the (new) stack
  call main                   ; call our main() function.
  jmp $                       ; Enter an infinite loop, to stop the processor
                              ; executing whatever rubbish is in the memory
                              ; after our kernel! 

[section .bss]

resb 0x1000
sys_stack:

