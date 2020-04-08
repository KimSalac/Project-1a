  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $6, $1, 4
   addi $5, $6, 12
   add $3, $5, $6
   add $7, $1, $6
	.end	__start
	.size	__start, .-__start
