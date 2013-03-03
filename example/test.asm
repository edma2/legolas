section .data
  msg:      db  'Hello, world!', 10, 0 ; "Hello, world!\n\0"
  msg_len:  equ $-msg

section .text
  global _start

_start:
  ; sys_write
  ;mov eax, 4
  ;mov ebx, 1
  ;mov ecx, msg
  ;mov edx, msg_len
  ;int 0x80

  ; exit(eax = prev)
  mov ebx, 8 ;test
  mov eax, 1
  int 0x80
