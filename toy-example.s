  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $1, $6, 6
   addi $2, $3, 3
   slti $3, $1, 8 
	.end	__start
	.size	__start, .-__start
