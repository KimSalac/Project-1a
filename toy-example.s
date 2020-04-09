  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $1, $0, 5
   addiu $2, $0, 0x0a
   andi $3, $0, 0x0002
   sb $0, 0($0)
   sh $0, 0($0)
   sw $0, 0($0)
   lbu $4, 0($0)
   lhu $5, 0($0)
   lw $7, 0($0)
   lui $8, 0x0023
   add $12, $1, $2	
	.end	__start
	.size	__start, .-__start
