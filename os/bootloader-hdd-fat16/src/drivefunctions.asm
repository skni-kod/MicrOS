CheckInt13Extenstion:
;============================================
;Function checks if extenstion is installed.
;Input parameters:
;  - dl = drive number
;Output parameters:
;  - carry flag = present ? 0 : 1
;  - ah = error code or Major Version Number
;  - bx = 0xaa55
;  - cx = interface support bitmask
;============================================
;CODE:
; Set parameters 
mov ah, 0x41
mov bx, 0xaa55

; Call interrupt
int 0x13

; Return from function
ret