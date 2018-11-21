global exc0
global exc1
global exc2
global exc3
global exc4
global exc5
global exc6
global exc7
global exc8
global exc9
global exc10
global exc11
global exc12
global exc13
global exc14
global exc15
global exc16
global exc17
global exc18
global exc19
global exc20
global exc21
global exc22
global exc23
global exc24
global exc25
global exc26
global exc27
global exc28
global exc29
global exc30
global exc31

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

extern global_exc_handler
extern global_int_handler

exc0:
  push 0
  jmp exc_wrapper

exc1:
  push 1
  jmp exc_wrapper

exc2:
  push 2
  jmp exc_wrapper

exc3:
  push 3
  jmp exc_wrapper

exc4:
  push 4
  jmp exc_wrapper

exc5:
  push 5
  jmp exc_wrapper

exc6:
  push 6
  jmp exc_wrapper

exc7:
  push 7
  jmp exc_wrapper

exc8:
  push 8
  jmp exc_wrapper

exc9:
  push 9
  jmp exc_wrapper

exc10:
  push 10
  jmp exc_wrapper

exc11:
  push 11
  jmp exc_wrapper

exc12:
  push 12
  jmp exc_wrapper

exc13:
  push 13
  jmp exc_wrapper

exc14:
  push 14
  jmp exc_wrapper

exc15:
  push 15
  jmp exc_wrapper

exc16:
  push 16
  jmp exc_wrapper

exc17:
  push 17
  jmp exc_wrapper

exc18:
  push 18
  jmp exc_wrapper

exc19:
  push 19
  jmp exc_wrapper

exc20:
  push 20
  jmp exc_wrapper

exc21:
  push 21
  jmp exc_wrapper

exc22:
  push 22
  jmp exc_wrapper

exc23:
  push 23
  jmp exc_wrapper

exc24:
  push 24
  jmp exc_wrapper

exc25:
  push 25
  jmp exc_wrapper

exc26:
  push 26
  jmp exc_wrapper

exc27:
  push 27
  jmp exc_wrapper

exc28:
  push 28
  jmp exc_wrapper

exc29:
  push 29
  jmp exc_wrapper

exc30:
  push 30
  jmp exc_wrapper

exc31:
  push 31
  jmp exc_wrapper

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

exc_wrapper:
  push edi
  push esi
  push ebp
  push esp
  push edx
  push ecx
  push ebx
  push eax

  call global_exc_handler

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
  
  ; Skip interrupt number
  add esp, 4

  iret