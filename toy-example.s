  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $1, $0, 5
   addiu $2, $0, 0x0a
	.end	__start
	.size	__start, .-__start
