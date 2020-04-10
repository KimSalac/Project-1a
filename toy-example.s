  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi  $1, $3, 5
   addi  $15, $4, 2
   addi  $12, $3, 4
   addi  $11, $9, 3
   add $10, $11, $11
   addi $5, $10, 10
	.end	__start
	.size	__start, .-__start
