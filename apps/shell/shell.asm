bits 64

section .data
prompt_msg db "> ", 0
newline db 10, 0
cmd_hello db "hello", 0
hello db "Hello, World!", 10, 0
unknown_msg db "Unknown command", 10, 0
cmd_buffer  times 128 db 0

section .text
    global _start

_start:
    mov rax, 3
    mov rdi, 0
    mov rsi, 0
    mov rdx, 800
    mov r10, 600
    mov r8,  0x0D0D0D
    int 0x80

main_loop:
    mov rax, 1
    lea rdi, [rel prompt_msg]
    int 0x80

    xor rcx, rcx
.clear_buf:
    mov byte [cmd_buffer + rcx], 0
    inc rcx
    cmp rcx, 128
    jne .clear_buf

    xor rcx, rcx
.read_char:
    mov rax, 4
    int 0x80
    cmp al, 10
    je .check_command
    mov [cmd_buffer + rcx], al
    inc rcx
    cmp rcx, 127
    je .check_command

    mov rax, 1
    lea rdi, [rel cmd_buffer + rcx - 1]
    int 0x80
    jmp .read_char

.check_command:
    mov rax, 1
    lea rdi, [rel newline]
    int 0x80

    ; strcmp(cmd_buffer, "hello")
    lea rdi, [rel cmd_buffer]
    lea rsi, [rel cmd_hello]
    call strcmp
    cmp rax, 0
    je .do_hello

    mov rax, 1
    lea rdi, [rel unknown_msg]
    int 0x80
    jmp main_loop
.do_hello:
    mov rax, 1
    lea rdi, [rel hello]
    int 0x80
    jmp main_loop

global strcmp

strcmp:
    xor rax, rax
.next:
    mov al, [rdi]
    mov bl, [rsi]
    cmp al, 0
    je .check_end
    cmp bl, 0
    je .check_end
    cmp al, bl
    jne .diff
    inc rdi
    inc rsi
    jmp .next

.check_end:
    cmp al, bl
    je .same
.diff:
    mov rax, 1
    ret

.same:
    xor rax, rax
    ret