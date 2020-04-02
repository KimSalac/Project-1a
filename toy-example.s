  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $1, $6, 6
   add $1, $6, $9
   sub $4, $5, $2
   sub $3, $2, $1
	.end	__start
	.size	__start, .-__start
