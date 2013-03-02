section .data
  msg:      db  'Hello, world!', 10, 0 ; "Hello, world!\n\0"
  msg_len:  equ $-msg

section .text
  global _start

_start:
  ; exit(0)
  mov eax, 1
  mov ebx, 0
  int 80h
