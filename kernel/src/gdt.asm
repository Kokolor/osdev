global gdt_reload
gdt_reload:
    lgdt [rdi]
    ret

global segments_reload
segments_reload:
    push 0x08
    lea rax, [rel .cs_reload]
    push rax
    retfq
.cs_reload:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret