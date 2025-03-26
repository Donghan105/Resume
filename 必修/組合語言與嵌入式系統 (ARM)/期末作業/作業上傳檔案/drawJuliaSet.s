.data

color:		.hword  0  	@2 bytes integer		@r10       
maxIter:    .word   255
n:      .word   1500				@ int n = 0
m:      .word   4000000					@ int m = 0
zx:		.word	0							@r9
zy:		.word	0						@r8
i:		.word	0					@r7
tmp:	.word	0				@r6
x:		.word	0			@r5
y:		.word	0		@r4
num:	.word	65535

	.global	__aeabi_idiv

.text
	.global	drawJuliaSet
	
drawJuliaSet:

	stmfd	sp!, { r4-r11, lr}
	
	@stroe initial value ↓
	
	str	r3, [sp, #-4]		@height
	str	r2, [sp, #-8]		@width
	str	r1, [sp, #-12]		@cY
	str	r0, [sp, #-16]		@cX
	mov r4, #0
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0
	mov r9, #0
	mov r10, #0
	sbcs r11, r0, r1		
	
	b loop1
	
	
exit:					
	ldmfd sp!, {r4-r11, pc}		@ function end
	
loop1:
	ldr r0, [sp, #-8]
	cmp r5, r0		@  x < width
	bge	exit
	mov r4, #0		@ default y

loop2:
	ldr r0, [sp, #-4]
	cmp r4, r0			@ y < height
	addge r5, r5,#1		@ x = x + 1
	addge r5, r5,#1		@ x = x + 1
	addge r5, r5,#1		@ x = x + 1
	subge r5, r5,#2		@ x = x - 2
	bge	loop1

	ldr r1, [sp, #-8]
	mov r1, r1, lsl #1		@width >> 1		Operand2
	mov r1, r1, asr #1		@width >> 1		Operand2
	mov r1, r1, ror #1		@width >> 1		Operand2
	
	
	ldr r1, [sp, #-8]		
	mov r1, r1, lsr #1		@width >> 1		Operand2
	sub r9, r5, r1			
	ldr r0, =n
	ldr r0, [r0]
	mul r0, r9, r0			
	ldr r1, [sp, #-8]		
	mov r1, r1, lsr #1		
	bl	__aeabi_idiv		
	mov r9, r0				@ zx = [ 1500 * ( x - width >> 1 ) ]  / ( width >> 1 )

	ldr r1, [sp, #-4]
	mov r1, r1, lsr #1		@ height >> 1
	sub r0, r4, r1			
	mov r1, #1000
	mul r0, r0, r1
	ldr r1, [sp, #-4]
	mov r1, r1, lsr #1		
	bl	__aeabi_idiv		
	mov r8, r0				@ zy = [ 1000 * (x - height>>1) ]  / ( height >> 1 )
	ldr r7, =maxIter		@ i = maxIter
	ldr r7, [r7]
	
	b if1
	
outif:
	and r0, r7, #0xff			@ r0 = ( i & 0xff )
	orr r10, r0, r0, lsl #8  	@ set color
	ldr r1, =num
	ldr r1, [r1]				
	bic r10, r1, r10			
	mov r2, #640
	mul r0, r4, r2				@ r0 = y * 640
	add r0, r0, r5				@ r0 = y * 640 + x
	mov r3, #2					@ * bytes
	mul r0, r0, r3				
	add r0, r0, #260
	strh r10, [sp, r0]			@ frame[y][x] = color
	add r4, r4, #1				@ y = y + 1
	
	b loop2

if1:		@ condition	zx * zx + zy * zy < 4000000	
	mul r0, r8, r8			@zy平方
	mul r1, r9, r9			@zx平方
	add r3, r0, r1			@r3 = zx	zy的平方和
	ldr r2, =m				@int 4000000
	ldr r2, [r2]
	cmp r3, r2			
	bge outif

if2:		@ condition	i > 0
	cmp r7, #0			
	ble	outif	
	
loop3:
	mul r0, r8, r8				@ zy平方
	mul r1, r9, r9				@ zx平方
	sub r0, r1, r0				@ r0 = zx	zy的平方差
	mov r1, #1000	
	bl	__aeabi_idiv			
	ldr r1, [sp, #-16]			
	add r6, r0, r1				@ tmp =...
	mov r0, #2
	mul r1, r0, r9				
	mul r0, r1, r8				
	mov r1, #1000
	bl	__aeabi_idiv			
	ldr r1, [sp, #-12]			
	add r8, r0, r1				@ zy =....
	mov r9, r6					@ zx = tmp
	sub r7, r7, #1				@ i = i - 1

	b if1	
	