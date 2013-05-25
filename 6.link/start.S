.globl _start
_start:

/* 硬件相关的设置 */
    /* Peri port setup */
    ldr r0, =0x70000000
    orr r0, r0, #0x13
    mcr p15,0,r0,c15,c2,4       @ 256M(0x70000000-0x7fffffff)
    
/* 关看门狗 */
/* 往WTCON(0x7E004000)写0 */
	
	ldr r0, =0x7E004000
	mov r1, #0
	str r1, [r0]

	/* 设置栈 
	因为c语言的调用需要到栈
	*/
	ldr sp, =4*1024+0x50000000	//设置栈地址，+0x50000000是因为下载到DDR里面运行的，而DDR开始地址是0x50000000
	bl main						//跳转到main运行
halt:
	b halt	
