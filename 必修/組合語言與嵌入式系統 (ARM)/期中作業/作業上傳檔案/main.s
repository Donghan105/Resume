.data

Str: .asciz "%s\n"
st0:	.asciz "Function1: Name\n"	 
st1:	.asciz "*****End Print*****\n"
st2:	.asciz "Function2: ID\n**** *Input ID*****\n"
st3:	.asciz "*****Print Team Member ID and ID Summation*****\n"
st4:	.asciz "\nMain Function:\n*****Print All*****\n"


All:	.asciz "%d\t%s\n"
pformat:.asciz  "ID Summation = %d\n"  @ Format string for printf
        .align  2

.text

.globl main

main:
      stmfd	sp!, { r4-r11, lr }
    ldr r0, =st0
	bl printf
	
	bl NAME
	
	MOV r4, r0   
	
	MOV r5, r1
	
	MOV r6, r2
	
	MOV r7, r3
    
	ldr	r0, =st1
	bl	printf
	ldr	r0, =st2
	bl	printf
	bl ID

	MOV r8, r0   
	
	MOV r9, r1
	
	MOV r10, r2
	
	MOV r11, r3
	
	ldr	r0, =st1
	bl	printf
	ldr	r0, =st4
	bl	printf

	MOV R0,R4
	bl	printf
	
	LDR R0,=All
	MOV R1,R8
	MOV R2,R5
	bl	printf
	
	LDR R0,=All
	MOV R1,R9
	MOV R2,R6
	bl	printf
	
	LDR R0,=All
	MOV R1,R10
	MOV R2,R7
	bl	printf
	
	LDR R0,=pformat
	MOV R1,R11
	bl	printf    
	
	ldr	r0, =st1
	bl	printf
	
	
	ldmfd sp!, { r4-r11, pc }
	mov pc, lr


.end
