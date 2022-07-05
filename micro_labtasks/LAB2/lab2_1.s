/*
 * asm.s
 *
 * author: Furkan
 *
 * description: external 8 led
 */


.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb


/* make linker see this */
.global Reset_Handler

/* get these from linker script */
.word _sdata
.word _edata
.word _sbss
.word _ebss


/* define peripheral addresses from RM0444 page 57, Tables 3-4 */
.equ RCC_BASE,         (0x40021000)          // RCC base address
.equ RCC_IOPENR,       (RCC_BASE   + (0x34)) // RCC IOPENR register offset

.equ GPIOA_BASE,       (0x50000000)          // GPIOC base address analog pinler
.equ GPIOB_BASE,       (0x50000400)          // GPIOC base address analog pinler
.equ GPIOA_MODER,      (GPIOA_BASE + (0x00)) // GPIOC A MODER register offset
.equ GPIOB_MODER,      (GPIOB_BASE + (0x00)) // GPIOC B MODER register offset
.equ GPIOA_ODR,        (GPIOA_BASE + (0x14)) // GPIOC A ODR register offset
.equ GPIOA_IDR,        (GPIOA_BASE + (0x10)) // GPIOC A IDR register offset
.equ GPIOB_ODR,        (GPIOB_BASE + (0x14)) // GPIOC B ODR register offset
.equ GPIOB_IDR,        (GPIOB_BASE + (0x10)) // GPIOC B IDR register offset


/* vector table, +1 thumb mode */
.section .vectors
vector_table:
	.word _estack             /*     Stack pointer */
	.word Reset_Handler +1    /*     Reset handler */
	.word Default_Handler +1  /*       NMI handler */
	.word Default_Handler +1  /* HardFault handler */
	/* add rest of them here if needed */


/* reset handler */
.section .text
Reset_Handler:
	/* set stack pointer */
	ldr r0, =_estack
	mov sp, r0

	/* initialize data and bss 
	 * not necessary for rom only code 
	 * */
	bl init_data
	/* call main */
	bl main
	/* trap if returned */
	b .


/* initialize data and bss sections */
.section .text
init_data:

	/* copy rom to ram */
	ldr r0, =_sdata
	ldr r1, =_edata
	ldr r2, =_sidata
	movs r3, #0
	b LoopCopyDataInit

	CopyDataInit:
		ldr r4, [r2, r3]
		str r4, [r0, r3]
		adds r3, r3, #4

	LoopCopyDataInit:
		adds r4, r0, r3
		cmp r4, r1
		bcc CopyDataInit

	/* zero bss */
	ldr r2, =_sbss
	ldr r4, =_ebss
	movs r3, #0
	b LoopFillZerobss

	FillZerobss:
		str  r3, [r2]
		adds r2, r2, #4

	LoopFillZerobss:
		cmp r2, r4
		bcc FillZerobss

	bx lr


/* default handler */
.section .text
Default_Handler:
	b Default_Handler


/* main function */
.section .text
main:
	/* enable GPIOA clock, bit0 on IOPENR */
	ldr r6, =RCC_IOPENR // adresini r6 ya aldık
	ldr r5, [r6]  // r6 alinip r5e  goturuldu
	/* movs expects imm8, so this should be fine */
	movs r4, 0x7 // A B C  1 olması icin
	orrs r5, r5, r4
	str r5, [r6] // A B C  portunu actık


	 /* enables B OUTPUT pins */
	 ldr r6, =GPIOB_MODER
	 ldr r5, [r6]
  	 ldr r4, = #0x00000FFF
     bics r5, r5, r4
     ldr r4, = #0xFFFF5555
     orrs r5, r5, r4
     str r5, [r6]

     /* enables A7 mode  input */
     ldr r6,=GPIOA_MODER
	 ldr r5, [r6]
	 ldr r4, = 0x3
	 lsls r4,r4,#14
	 mvns r4,r4
	 ands r5, r5, r4
	 str r5, [r6]

	  ldr r3, =#1000000

     button:

	 /* read button connected to A7 addressed at IDR*/
	 ldr r6, =GPIOA_IDR
   	 ldr r5, [r6]
 	 lsrs r5, r5, #7
	 movs r4, #0x1
	 ands r5, r5, r4
	// str r5, [r6]

	 /*check button value*/
	 cmp r5, #0x1
	 beq play  //Branch is equal
	 bne pause //Branch is equal



play:

	led_plloop:

	/* turn off led connected to B0 STATU LED in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x1
	mvns r4, r4
	ands r5, r5, r4
	str r5, [r6]
	movs r1, #0x0
	ands r1, r1
	bl delay_1s


	/* turn on led connected to B4 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x10
	orrs r5, r5, r4
	str r5, [r6]
	movs r1, #0x0
	ands r1, r1
	movs r1, #0x1
	;ldr r0, [pc]
	bl delay_1s

	/* turn on led connected to B3 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x8
	orrs r5, r5, r4
	str r5, [r6]

	/* turn on led connected to B5 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x20
	orrs r5, r5, r4
	str r5, [r6]
	movs r1, #0x0
	ands r1, r1
	movs r1, #0x1
	;ldr r0, [pc]
	bl delay_1s

	/* turn on led connected to B2 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x4
	orrs r5, r5, r4
	str r5, [r6]

	/* turn on led connected to B6 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x40
	orrs r5, r5, r4
	str r5, [r6]
	movs r1, #0x0
	ands r1, r1
	movs r1, #0x1
	;ldr r0, [pc]
	bl delay_1s


	/* turn on led connected to B1 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x2
	orrs r5, r5, r4
	str r5, [r6]


	/* turn on led connected to B7 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x80
	orrs r5, r5, r4
	str r5, [r6]
	movs r1, #0x0
	ands r1, r1
	movs r1, #0x1
	;ldr r0, [pc]
	bl delay_1s

	/* turn off led connected to B1 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x2
	mvns r4, r4
	ands r5, r5, r4
	str r5, [r6]

	/* turn off led connected to B7 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x80
	mvns r4, r4
	ands r5, r5, r4
	str r5, [r6]
	movs r1, #0x0
	ands r1, r1
	bl delay_1s

	/* turn off led connected to B2 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x4
	mvns r4, r4
	ands r5, r5, r4
	str r5, [r6]

	/* turn off led connected to B6 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x40
	mvns r4, r4
	ands r5, r5, r4
	str r5, [r6]
	movs r1, #0x0
	ands r1, r1
	bl delay_1s

	/* turn off led connected to B3 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x8
	mvns r4, r4
	ands r5, r5, r4
	str r5, [r6]


	/* turn off led connected to B5 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x20
	mvns r4, r4
	ands r5, r5, r4
	str r5, [r6]
	movs r1, #0x0
	ands r1, r1
	bl delay_1s

	/* turn off led connected to B4 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x10
	ands r5, r5, r4
	str r5, [r6]
	movs r1, #0x0
	ands r1, r1

	bl delay_1s


	b button
	b led_plloop



pause:

	/* turn off led connected to B4 in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x10
	ands r5, r5, r4
	str r5, [r6]

	/* turn on led connected to B0 STATU LED in ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x1
	orrs r5, r5, r4
	str r5, [r6]
	movs r1, #0x0
	ands r1, r1
	movs r1, #0x1
	;ldr r0, [pc]
	bl delay_1s

	b button


	delay_1s:
    adds r1,r1,#0x1
    cmp r1,r3
    blt delay_1s
    bx lr

	/* for(;;); */
	b .

	/* this should never get executed */
	nop

