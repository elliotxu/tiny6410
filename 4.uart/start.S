.globl _start
.globl reset
_start:
/* Peri Port Setup */
 ldr r0,=0x70000000
 orr r0,r0,#0x13
 mcr p15,0,r0,c15,c2,4
/* Close Watch Dog */
 ldr r0,=0x7e004000
 mov r1,#0
 str r1,[r0]
/* Setup SP */
 ldr sp,=20*1024+0x50000000
/* Go To Setup Clock */
 bl clock_init
/* Go To main */
 bl main
 reset:
 b _start
halt:
 b halt