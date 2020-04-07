  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $1, $6, 4
   addi $7, $1, 6
	.end	__start
	.size	__start, .-__start
