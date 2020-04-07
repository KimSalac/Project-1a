  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $1, $6, 6
   addi $7, $7, 6
   addi $8, $8, 6
   lw $9, 0($1)
   
	.end	__start
	.size	__start, .-__start
