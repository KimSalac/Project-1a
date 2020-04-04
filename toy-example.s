  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $1, $6, 6
   add $2, $6, $1
   addi $7, $4, 3
   nor $10, $2, $7
   and $4, $1, $2
   or $3, $2, $1
   sll $9, $7, 2 
	.end	__start
	.size	__start, .-__start
