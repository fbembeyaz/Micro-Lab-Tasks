
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


.equ GPIOA_BASE,       (0x50000000)          // GPIOA base address
.equ GPIOA_MODER,      (GPIOA_BASE + (0x00)) // GPIOA MODER register offset
.equ GPIOA_ODR,        (GPIOA_BASE + (0x14)) // GPIOA ODR output offset
.equ GPIOA_IDR,        (GPIOA_BASE + (0x10)) // GPIOA ODR input offset

.equ GPIOB_BASE,       (0x50000400)          // GPIOB base address
.equ GPIOB_MODER,      (GPIOB_BASE + (0x00)) // GPIOB MODER register offset
.equ GPIOB_ODR,        (GPIOB_BASE + (0x14)) // GPIOB ODR output offset
.equ GPIOB_IDR,        (GPIOB_BASE + (0x10)) // GPIOB ODR input offset

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
	/* enable GPIOA and GPIOB clock,  */
	ldr r6, =RCC_IOPENR
	ldr r5, [r6]
	movs r4, 0x3
	orrs r5, r5, r4
	str r5, [r6]

    ldr r6, =GPIOB_MODER
	ldr r5, [r6]
	/* cannot do with movs, so use pc relative */
	ldr r4, =0x1555
	ands r5,r5,r4
	str r5,[r6]

	ldr r6, =GPIOA_MODER
	ldr r5, [r6]
	ldr r4, =0x3000
	mvns r4, r4
	ands r5, r5, r4
	ldr r4, =0x1000
	orrs r5, r5, r4
	str r5, [r6]




    //8'den geriye
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, 0x7f //// 8 no
	orrs r5, r5, r4
	str r5, [r6]
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x7
	str r5, [r6] // 7no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x7D //6 no
	str r5, [r6]
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x6D
	str r5, [r6] // 5no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x66
	str r5, [r6] // 4no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x4f
	str r5, [r6] //3 no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x5B
	str r5, [r6] // 2 no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x6
	str r5, [r6] // 1 no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x3F // 0 no
	str r5, [r6]

	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	movs r4, 0x40
	orrs r5, r5, r4
	str r5, [r6]
	ldr r1,=#5000000
    bl delay



////////2 numara
    ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	movs r4, 0x40
	orrs r5, r5, r4
    movs r4, #0x40
    mvns r4, r4
    ands r5, r5, r4
    str r5, [r6]


	ldr r6, =GPIOB_ODR
	ldr r5, = #0x5B
	str r5, [r6] // 2 no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x6
	str r5, [r6] // 1 no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x3F // 0 no
	str r5, [r6]


	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	movs r4, 0x40
	orrs r5, r5, r4
	str r5, [r6]
	ldr r1,=#5000000
    bl delay


//////////////9 numara
    ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	movs r4, 0x40
	orrs r5, r5, r4
    movs r4, #0x40
    mvns r4, r4
    ands r5, r5, r4
    str r5, [r6]

    ldr r6, =GPIOB_ODR
    ldr r5, = #0x6f
	str r5, [r6] // 9no
	ldr r1,=#5000000
	bl delay
    ldr r5, = #0x7f
	str r5, [r6] // 8no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x7
	str r5, [r6] // 7no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x7D //6 no
	str r5, [r6]
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x6D
	str r5, [r6] // 5no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x66
	str r5, [r6] // 4no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x4f
	str r5, [r6] //3 no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x5B
	str r5, [r6] // 2 no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x6
	str r5, [r6] // 1 no
	ldr r1,=#5000000
	bl delay
	ldr r5, = #0x3F // 0 no
	str r5, [r6]


	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	movs r4, 0x40
	orrs r5, r5, r4
	str r5, [r6]
	ldr r1,=#5000000
    bl delay






 delay:
		subs r1,r1,#1
		bne delay
		bx lr
	b .

	/* this should never get executed */
	nop
