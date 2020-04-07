  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $6, $1, 4
   addi $1, $2, 3
   add $6, $6, $2
   add $9, $1, $4 
	.end	__start
	.size	__start, .-__start
