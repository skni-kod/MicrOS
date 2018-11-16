global int32
global int33
global int34
global int35
global int36
global int37
global int38
global int39
global int40
global int41
global int42
global int43
global int44
global int45
global int46
global int47
global int48
global int49
global int50
global int51
global int52

extern global_int_handler

int32:
  push 32
  jmp int_wrapper

int33:
  push 33
  jmp int_wrapper

int34:
  push 34
  jmp int_wrapper

int35:
  push 35
  jmp int_wrapper

int36:
  push 36
  jmp int_wrapper

int37:
  push 37
  jmp int_wrapper

int38:
  push 38
  jmp int_wrapper

int39:
  push 39
  jmp int_wrapper
  
int40:
  push 40
  jmp int_wrapper
  
int41:
  push 41
  jmp int_wrapper
  
int42:
  push 42
  jmp int_wrapper
  
int43:
  push 43
  jmp int_wrapper
  
int44:
  push 44
  jmp int_wrapper
  
int45:
  push 45
  jmp int_wrapper
  
int46:
  push 46
  jmp int_wrapper
  
int47:
  push 47
  jmp int_wrapper

int48:
  push 48
  jmp int_wrapper

int49:
  push 49
  jmp int_wrapper

int50:
  push 50
  jmp int_wrapper

int51:
  push 51
  jmp int_wrapper

int52:
  push 52
  jmp int_wrapper

int_wrapper:
  push edi
  push esi
  push ebp
  push esp
  push edx
  push ecx
  push ebx
  push eax

  call global_int_handler

  pop eax
  pop ebx
  pop ecx
  pop edx
  pop esp
  pop ebp
  pop esi
  pop edi
  
  add esp, 4
  iret