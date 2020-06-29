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
  %if i != 8 && i != 10 && i != 11 && i != 12 && i != 13 && i != 14 && i != 17 && i != 30
    push 0
  %endif
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

; Input: interrupt number and error code on stack
; Output: nothing
idt_exc_wrapper:
  ; Move stack pointer (fsave won't do this itself)
  sub esp, 108
  
  ; Save FPU state
  fnsave [esp]
  fwait
  
  ; Save registers
  pusha
  
  ; Save other registers for diagnostic view
%macro store_register 1
  mov eax, %1
  push eax
%endmacro

  store_register ds
  store_register cr0
  store_register cr2
  store_register cr3
  store_register cr4
  store_register es
  store_register fs
  store_register gs
  
  sub esp, 8
  sgdt [esp]
  sub esp, 8
  sidt [esp]
  
  push esp

  call idt_global_exc_handler

  pop esp
  add esp, 48
  popa
  
  ; Restore FPU state
  frstor [esp]
  fwait
  
  ; Move stack pointer (frstor won't do this itself)
  add esp, 108
  
  ; Skip interrupt number and error code
  add esp, 8
  iret

; Input: interrupt number on stack
; Output: nothing
idt_int_wrapper:
  ; Move stack pointer (fsave won't do this itself)
  sub esp, 108
  
  ; Save FPU state
  fnsave [esp]
  fwait
  
  ; Save registers
  pusha
  
  ; Push stack pointer (because esp in pusha is not valid for us)
  push esp

  ; Process interrupt
  call idt_global_int_handler

  ; Restore original stack pointer
  pop esp
  
  ; Restore registers
  popa
  
  ; Restore FPU state
  frstor [esp]
  fwait
  
  ; Move stack pointer (frstor won't do this itself)
  add esp, 108
  
  ; Skip interrupt number
  add esp, 4
  
  ; Bye!
  iret