global IRQ0
global IRQ1
global IRQ2
global IRQ3
global IRQ4
global IRQ5
global IRQ6
global IRQ7
global IRQ8
global IRQ9
global IRQ10
global IRQ11
global IRQ12
global IRQ13
global IRQ14
global IRQ15
global IRQ16
global IRQ17
global IRQ18
global IRQ19
global IRQ20
global IRQ21
global IRQ22
global IRQ23
global IRQ24
global IRQ25
global IRQ26
global IRQ27
global IRQ28
global IRQ29
global IRQ30
global IRQ31

global IRQ0_handler
global IRQ1_handler
global IRQ2_handler
global IRQ3_handler
global IRQ4_handler
global IRQ5_handler
global IRQ6_handler
global IRQ7_handler
global IRQ8_handler
global IRQ9_handler
global IRQ10_handler
global IRQ11_handler
global IRQ12_handler
global IRQ13_handler
global IRQ14_handler
global IRQ15_handler
global IRQ16_handler
global IRQ17_handler
global IRQ18_handler
global IRQ19_handler
global IRQ20_handler
global IRQ21_handler
global IRQ22_handler
global IRQ23_handler
global IRQ24_handler
global IRQ25_handler
global IRQ26_handler
global IRQ27_handler
global IRQ28_handler
global IRQ29_handler
global IRQ30_handler
global IRQ31_handler
 
extern IRQ0_handler
extern IRQ1_handler
extern IRQ2_handler
extern IRQ3_handler
extern IRQ4_handler
extern IRQ5_handler
extern IRQ6_handler
extern IRQ7_handler
extern IRQ8_handler
extern IRQ9_handler
extern IRQ10_handler
extern IRQ11_handler
extern IRQ12_handler
extern IRQ13_handler
extern IRQ14_handler
extern IRQ15_handler
extern IRQ16_handler
extern IRQ17_handler
extern IRQ18_handler
extern IRQ19_handler
extern IRQ20_handler
extern IRQ21_handler
extern IRQ22_handler
extern IRQ23_handler
extern IRQ24_handler
extern IRQ25_handler
extern IRQ26_handler
extern IRQ27_handler
extern IRQ28_handler
extern IRQ29_handler
extern IRQ30_handler
extern IRQ31_handler

IRQ0:
  pusha
  call IRQ0_handler
  popa
  iret
 
IRQ1:
  pusha
  call IRQ1_handler
  popa
  iret
 
IRQ2:
  pusha
  call IRQ2_handler
  popa
  iret
 
IRQ3:
  pusha
  call IRQ3_handler
  popa
  iret
 
IRQ4:
  pusha
  call IRQ4_handler
  popa
  iret
 
IRQ5:
  pusha
  call IRQ5_handler
  popa
  iret
 
IRQ6:
  pusha
  call IRQ6_handler
  popa
  iret
 
IRQ7:
  pusha
  call IRQ7_handler
  popa
  iret
 
IRQ8:
  pusha
  call IRQ8_handler
  popa
  iret
 
IRQ9:
  pusha
  call IRQ9_handler
  popa
  iret
 
IRQ10:
  pusha
  call IRQ10_handler
  popa
  iret
 
IRQ11:
  pusha
  call IRQ11_handler
  popa
  iret
 
IRQ12:
  pusha
  call IRQ12_handler
  popa
  iret
 
IRQ13:
  pusha
  call IRQ13_handler
  popa
  iret
 
IRQ14:
  pusha
  call IRQ14_handler
  popa
  iret
 
IRQ15:
  pusha
  call IRQ15_handler
  popa
  iret
  
 
IRQ16:
  pusha
  call IRQ16_handler
  popa
  iret
  
 
IRQ17:
  pusha
  call IRQ17_handler
  popa
  iret
  
 
IRQ18:
  pusha
  call IRQ18_handler
  popa
  iret
  
 
IRQ19:
  pusha
  call IRQ19_handler
  popa
  iret
  
 
IRQ20:
  pusha
  call IRQ20_handler
  popa
  iret
  
 
IRQ21:
  pusha
  call IRQ21_handler
  popa
  iret
  
 
IRQ22:
  pusha
  call IRQ22_handler
  popa
  iret
  
 
IRQ23:
  pusha
  call IRQ23_handler
  popa
  iret
  
 
IRQ24:
  pusha
  call IRQ24_handler
  popa
  iret
  
 
IRQ25:
  pusha
  call IRQ25_handler
  popa
  iret
  
 
IRQ26:
  pusha
  call IRQ26_handler
  popa
  iret
  
 
IRQ27:
  pusha
  call IRQ27_handler
  popa
  iret
  
 
IRQ28:
  pusha
  call IRQ28_handler
  popa
  iret
  
 
IRQ29:
  pusha
  call IRQ29_handler
  popa
  iret
  
 
IRQ30:
  pusha
  call IRQ30_handler
  popa
  iret
  
 
IRQ31:
  pusha
  call IRQ31_handler
  popa
  iret