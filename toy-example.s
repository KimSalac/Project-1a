  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $6, $1, 4
   addi $1, $6, 3
   add $6, $1, $2
   addi $7, $6, 2
   add $1, $2, $7
   add $2, $1, $1
	.end	__start
	.size	__start, .-__start
