global int0
global int1
global int2
global int3
global int4
global int5
global int6
global int7
global int8
global int9
global int10
global int11
global int12
global int13
global int14
global int15
global int48
global int49
global int50
global int51
global int52

extern int_handler

int0:
  push 0
  jmp int_wrapper

int1:
  push 1
  jmp int_wrapper

int2:
  push 2
  jmp int_wrapper

int3:
  push 3
  jmp int_wrapper

int4:
  push 4
  jmp int_wrapper

int5:
  push 5
  jmp int_wrapper

int6:
  push 6
  jmp int_wrapper

int7:
  push 7
  jmp int_wrapper
  
int8:
  push 8
  jmp int_wrapper
  
int9:
  push 9
  jmp int_wrapper
  
int10:
  push 10
  jmp int_wrapper
  
int11:
  push 11
  jmp int_wrapper
  
int12:
  push 12
  jmp int_wrapper
  
int13:
  push 13
  jmp int_wrapper
  
int14:
  push 14
  jmp int_wrapper
  
int15:
  push 15
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
  push eax
  push ebx
  push ecx
  push edx
  push esp
  push ebp
  push esi
  push edi

  call int_handler

  pop edi
  pop esi
  pop ebp
  pop esp
  pop edx
  pop ecx
  pop ebx
  pop eax
  
  add esp, 4
  iret