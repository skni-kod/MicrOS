global idt_exc0
global idt_exc1
global idt_exc2
global idt_exc3
global idt_exc4
global idt_exc5
global idt_exc6
global idt_exc7
global idt_exc8
global idt_exc9
global idt_exc10
global idt_exc11
global idt_exc12
global idt_exc13
global idt_exc14
global idt_exc15
global idt_exc16
global idt_exc17
global idt_exc18
global idt_exc19
global idt_exc20
global idt_exc21
global idt_exc22
global idt_exc23
global idt_exc24
global idt_exc25
global idt_exc26
global idt_exc27
global idt_exc28
global idt_exc29
global idt_exc30
global idt_exc31

global idt_int32
global idt_int33
global idt_int34
global idt_int35
global idt_int36
global idt_int37
global idt_int38
global idt_int39
global idt_int40
global idt_int41
global idt_int42
global idt_int43
global idt_int44
global idt_int45
global idt_int46
global idt_int47
global idt_int48
global idt_int49
global idt_int50
global idt_int51
global idt_int52

extern idt_global_exc_handler
extern idt_global_int_handler

idt_exc0:
  push 0
  jmp idt_exc_wrapper

idt_exc1:
  push 1
  jmp idt_exc_wrapper

idt_exc2:
  push 2
  jmp idt_exc_wrapper

idt_exc3:
  push 3
  jmp idt_exc_wrapper

idt_exc4:
  push 4
  jmp idt_exc_wrapper

idt_exc5:
  push 5
  jmp idt_exc_wrapper

idt_exc6:
  push 6
  jmp idt_exc_wrapper

idt_exc7:
  push 7
  jmp idt_exc_wrapper

idt_exc8:
  push 8
  jmp idt_exc_wrapper

idt_exc9:
  push 9
  jmp idt_exc_wrapper

idt_exc10:
  push 10
  jmp idt_exc_wrapper

idt_exc11:
  push 11
  jmp idt_exc_wrapper

idt_exc12:
  push 12
  jmp idt_exc_wrapper

idt_exc13:
  push 13
  jmp idt_exc_wrapper

idt_exc14:
  push 14
  jmp idt_exc_wrapper

idt_exc15:
  push 15
  jmp idt_exc_wrapper

idt_exc16:
  push 16
  jmp idt_exc_wrapper

idt_exc17:
  push 17
  jmp idt_exc_wrapper

idt_exc18:
  push 18
  jmp idt_exc_wrapper

idt_exc19:
  push 19
  jmp idt_exc_wrapper

idt_exc20:
  push 20
  jmp idt_exc_wrapper

idt_exc21:
  push 21
  jmp idt_exc_wrapper

idt_exc22:
  push 22
  jmp idt_exc_wrapper

idt_exc23:
  push 23
  jmp idt_exc_wrapper

idt_exc24:
  push 24
  jmp idt_exc_wrapper

idt_exc25:
  push 25
  jmp idt_exc_wrapper

idt_exc26:
  push 26
  jmp idt_exc_wrapper

idt_exc27:
  push 27
  jmp idt_exc_wrapper

idt_exc28:
  push 28
  jmp idt_exc_wrapper

idt_exc29:
  push 29
  jmp idt_exc_wrapper

idt_exc30:
  push 30
  jmp idt_exc_wrapper

idt_exc31:
  push 31
  jmp idt_exc_wrapper

idt_int32:
  push 32
  jmp idt_int_wrapper

idt_int33:
  push 33
  jmp idt_int_wrapper

idt_int34:
  push 34
  jmp idt_int_wrapper

idt_int35:
  push 35
  jmp idt_int_wrapper

idt_int36:
  push 36
  jmp idt_int_wrapper

idt_int37:
  push 37
  jmp idt_int_wrapper

idt_int38:
  push 38
  jmp idt_int_wrapper

idt_int39:
  push 39
  jmp idt_int_wrapper
  
idt_int40:
  push 40
  jmp idt_int_wrapper
  
idt_int41:
  push 41
  jmp idt_int_wrapper
  
idt_int42:
  push 42
  jmp idt_int_wrapper
  
idt_int43:
  push 43
  jmp idt_int_wrapper
  
idt_int44:
  push 44
  jmp idt_int_wrapper
  
idt_int45:
  push 45
  jmp idt_int_wrapper
  
idt_int46:
  push 46
  jmp idt_int_wrapper
  
idt_int47:
  push 47
  jmp idt_int_wrapper

idt_int48:
  push 48
  jmp idt_int_wrapper

idt_int49:
  push 49
  jmp idt_int_wrapper

idt_int50:
  push 50
  jmp idt_int_wrapper

idt_int51:
  push 51
  jmp idt_int_wrapper

idt_int52:
  push 52
  jmp idt_int_wrapper

idt_exc_wrapper:
  push edi
  push esi
  push ebp
  push esp
  push edx
  push ecx
  push ebx
  push eax

  call idt_global_exc_handler

  pop eax
  pop ebx
  pop ecx
  pop edx
  pop esp
  pop ebp
  pop esi
  pop edi
  
  ; Skip interrupt number and error code
  add esp, 8
  iret

idt_int_wrapper:
  push edi
  push esi
  push ebp
  push esp
  push edx
  push ecx
  push ebx
  push eax

  call idt_global_int_handler

  pop eax
  pop ebx
  pop ecx
  pop edx
  pop esp
  pop ebp
  pop esi
  pop edi
  
  ; Skip interrupt number
  add esp, 4

  iret