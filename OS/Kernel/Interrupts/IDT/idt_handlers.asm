; Global declarations for idt_exc
%macro global_idt_exc 1
  global idt_exc%1
%endmacro

%assign i 0 
%rep    32 
  global_idt_exc i
%assign i i+1 
%endrep

; Global declarations for idt_int
%macro global_idt_int 1
  global idt_int%1
%endmacro

%assign i 32 
%rep    32 
  global_idt_int i
%assign i i+1 
%endrep

extern idt_global_exc_handler
extern idt_global_int_handler

; Global declarations for idt_exc_wrapper
%macro idt_exc 1
idt_exc%1:
  push %1
  jmp idt_exc_wrapper
%endmacro

%assign i 0 
%rep    32 
  idt_exc i
%assign i i+1 
%endrep

; Global declarations for idt_int_wrapper
%macro idt_int 1
idt_int%1:
  push %1
  jmp idt_int_wrapper
%endmacro

%assign i 32 
%rep    32 
  idt_int i
%assign i i+1 
%endrep

idt_exc_wrapper:
  pusha
  pushf
  push esp

  call idt_global_exc_handler

  pop esp
  popf
  popa
  
  ; Skip interrupt number and error code
  add esp, 8
  iret

idt_int_wrapper:
  pusha
  pushf
  push esp

  call idt_global_int_handler

  pop esp
  popf
  popa
  
  ; Skip interrupt number
  add esp, 4

  iret