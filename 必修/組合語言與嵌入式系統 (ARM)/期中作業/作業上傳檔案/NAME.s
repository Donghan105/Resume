.data

msg1:	.asciz	"Team07\n"

msg2:	.asciz	"ZHUANG DONG HAN\n"

msg3:	.asciz	"DAI CHENG HAO\n"

msg4:	.asciz	"ZHUANG DONG HAN\n"

.text


.globl NAME
	
	
NAME:	

    stmfd	sp!, { lr }
	
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
	LDR r0, =msg1 
	LDR r1, =msg2 
	LDR r2, =msg3 
	LDR r3, =msg4 
	

	ldmfd sp!, { pc }
	