.data

str1:		.asciz	"*****Print Team Member ID and ID Summation*****\n"
prompt1:	.asciz  "Enter the first sid:\n"  @ user prompt1
			.align  2
			
prompt2:	.asciz  "Enter the second sid:\n"  @ user prompt2
			.align  2
			
prompt3:	.asciz  "Enter the third sid:\n"  @ user prompt3
			.align  2
			
prompt4:	.asciz  "Please enter the command:\n"  @ user prompt4
			.align  2
			
prompt5:	.asciz  "Command error.\n"  @ user prompt5
			.align  2
			
letterp:	.asciz	"p"
			.align  2

printint:.asciz  "%d\n"             @ Format string for reading an int with scanf
        .align  2
			
sformat:.asciz  "%d"                @ Format string for reading an int with scanf
        .align  2
		
pformat:.asciz  "The sum is: %d\n"  @ Format string for printf
        .align  2
		
stringformat:	.asciz "%s"
		.align  2

n:      .word   0                   @ int n = 0
m:      .word   0                   @ int m = 0
a:		.word   0    				@ int a = 0
b:		.word   0    				@ int b = 0 */sum*/
scanword: .asciz

.text

.globl ID

ID:   
	stmfd   sp!, {lr}       @ push lr onto stack
	
	st1:
		@ printf("Enter the first sid: ")
		ldr     r0, =prompt1
		bl      printf

		@ scanf("%d\0", &n)		*/scanf一定由r0儲存format r1儲存變數的地址*/
		ldr     r0, =sformat    @ load address of format string
		ldr     r1, =n          @ load address of int n variable
		bl      scanf           @ call scanf("%d", &n)

	st2:	
		@ printf("Enter the second sid: ")
		ldr     r0, =prompt2
		bl      printf

		@ scanf("%d\0", &n)		*/scanf一定由r0儲存format r1儲存變數的地址*/
		ldr     r0, =sformat    @ load address of format string
		ldr     r1, =m          @ load address of int n variable
		bl      scanf           @ call scanf("%d", &m)
	
	st3:
		@ printf("Enter the third sid: ")
		ldr     r0, =prompt3
		bl      printf

		@ scanf("%d\0", &n)		*/scanf一定由r0儲存format r1儲存變數的地址*/
		ldr     r0, =sformat    @ load address of format string
		ldr     r1, =a          @ load address of int n variable
		bl      scanf           @ call scanf("%d", &a)
		
	@ printf("Please enter the command: ")
	ldr     r0, =prompt4
	bl      printf
	
	@ scanf("%d", &scanword)
	ldr     r0, =stringformat
	ldr     r1, =scanword 
	bl      scanf				@ scanf("%d", &scanword)
	
	ldr     r0, =letterp
	ldrb	r0,[r0]
	ldr     r1, =scanword 
	ldrb     r2, [r1]
	
	cmp		r0, r2
	ldreqb	r2, [r1, #1]
	cmpeq	r2, #0
	beq		ifpprint
	bne		exit
	
	@mov     r0, #0          @ load return value
	@mov     pc, lr          @ return from main
	@ldmfd   sp!, {lr}       @ pop lr from stack

	
ifpprint:
		ldr		r0, =str1
		bl		printf
		ldr     r2, =n			@ print int n
		ldr		r0, =printint
		ldr		r1,	=n
		ldr		r1,	[r1]
		bl 		printf
		
		ldr     r2, =m			@ print int m
		ldr		r0, =printint
		ldr		r1,	=m
		ldr		r1,	[r1]
		bl 		printf
		
		ldr     r2, =a			@ print int a
		ldr		r0, =printint
		ldr		r1,	=a
		ldr		r1,	[r1]
		bl 		printf
		

		
		ldr     r2, =m			@ load address of int m to r2
		ldr		r3, [r2]
		ldr     r1, =b			@ load address of int b to r5
		ldr     r1, [r1]		@ load value of int b to r5(b=0)
		add 	r1,	r1, r3		
		ldr     r2, =n			@ load address of int n to r2
		ldr		r3, [r2]
		add		r1, r1, r3		
		ldr     r2, =a			@ load address of int a to r2
		ldr		r3, [r2]
		add		r1, r1, r3	
		ldr		r3, =b
		STR		r1, [r3]
		
		
		
	
		@ printf("The sum is: %d\n", n)
		ldr     r0, =pformat    @ load address of format string
		bl      printf          @ call printf("The sum is: %d\n", n) */bl printf印出r1內容*/

		ldr		r3, =b
		ldr		r0, =n
		ldr 	r1, =m
		ldr 	r2, =a
		ldr 	r0, [r0]
		ldr 	r1, [r1]
		ldr 	r2, [r2]
		ldr 	r3, [r3]
		
exit:
		ldmfd sp!, {pc}
