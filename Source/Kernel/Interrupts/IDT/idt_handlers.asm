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

global int0_handler
global int1_handler
global int2_handler
global int3_handler
global int4_handler
global int5_handler
global int6_handler
global int7_handler
global int8_handler
global int9_handler
global int10_handler
global int11_handler
global int12_handler
global int13_handler
global int14_handler
global int15_handler
global int48_handler
global int49_handler
global int50_handler
global int51_handler
global int52_handler
 
extern int0_handler
extern int1_handler
extern int2_handler
extern int3_handler
extern int4_handler
extern int5_handler
extern int6_handler
extern int7_handler
extern int8_handler
extern int9_handler
extern int10_handler
extern int11_handler
extern int12_handler
extern int13_handler
extern int14_handler
extern int15_handler
extern int48_handler
extern int49_handler
extern int50_handler
extern int51_handler
extern int52_handler

int0:
  pusha
  call int0_handler
  popa
  iret

int1:
  pusha
  call int1_handler
  popa
  iret

int2:
  pusha
  call int2_handler
  popa
  iret

int3:
  pusha
  call int3_handler
  popa
  iret

int4:
  pusha
  call int4_handler
  popa
  iret

int5:
  pusha
  call int5_handler
  popa
  iret

int6:
  pusha
  call int6_handler
  popa
  iret

int7:
  pusha
  call int7_handler
  popa
  iret
  
int8:
  pusha
  call int8_handler
  popa
  iret
  
int9:
  pusha
  call int9_handler
  popa
  iret
  
int10:
  pusha
  call int10_handler
  popa
  iret
  
int11:
  pusha
  call int11_handler
  popa
  iret
  
int12:
  pusha
  call int12_handler
  popa
  iret
  
int13:
  pusha
  call int13_handler
  popa
  iret
  
int14:
  pusha
  call int14_handler
  popa
  iret
  
int15:
  pusha
  call int15_handler
  popa
  iret

int48:
  push ecx
  push edx
  push ebx
  push esp
  push ebp
  push esi
  push edi
  call int48_handler
  pop edi
  pop esi
  pop ebp
  pop esp
  pop ebx
  pop edx
  pop ecx
  
  iret

int49:
  pusha
  call int49_handler
  popa
  iret

int50:
  pusha
  call int50_handler
  popa
  iret

int51:
  pusha
  call int51_handler
  popa
  iret

int52:
  pusha
  call int52_handler
  popa
  iret