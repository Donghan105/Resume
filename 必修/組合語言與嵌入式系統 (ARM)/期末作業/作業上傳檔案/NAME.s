.data

msg1:	.asciz	"Team07\n"

msg2:	.asciz	"ZHUANG DONG HAN\n"

msg3:	.asciz	"DAI CHENG HAO\n"

msg4:	.asciz	"ZHUANG DONG HAN\n"

.text


.globl NAME
	
	
NAME:	
    stmfd	sp!, { r8-r11, lr }
	mov r8, r0
	mov r9, r1
	mov r10, r2
	mov r11, r3
	
firstmessage:
	MOV R7, #4 	@systemcall to 4, for writing information to screen
	
		MOV R0, #1 	@output to monitor(Set r0 to show in ARM code.)
	
		MOV R2, #7 	@length of string
	
		LDR R1, =msg1
	
		SWI 0

secondmessage:
	MOV R7, #4 	@systemcall to 4, for writing information to screen
	
		MOV R0, #1 	@output to monitor(Set r0 to show in ARM code.)
	
		MOV R2, #16 	@length of string
	
		LDR R1, =msg2
	
		SWI 0

thirdmessage:
		MOV R7, #4 	@systemcall to 4, for writing information to screen
	
		MOV R0, #1 	@output to monitor(Set r0 to show in ARM code.)
	
		MOV R2, #14 	@length of string
	
		LDR R1, =msg3
	
		SWI 0

fourthmessage:
	MOV R7, #4 	@systemcall to 4, for writing information to screen
	
		MOV R0, #1 	@output to monitor(Set r0 to show in ARM code.)
	
		MOV R2, #16 	@length of string
	
		LDR R1, =msg4
	
		SWI 0
	
store:
	mov r0, r8 
	ldr r1, =msg1
	bl strcpy
	mov r0, r9 
	ldr r1, =msg2
	bl strcpy
	mov r0, r10 
	ldr r1, =msg3
	bl strcpy
	mov r0, r11
	ldr r1, =msg4
	bl strcpy
	

	ldmfd sp!, { r8-r11,pc }
	