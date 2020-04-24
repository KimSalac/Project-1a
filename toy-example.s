  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $6, $1, 4
   addi $5, $6, 12
   sw   $6, 0($5)
   lw	$7, 0($5)
	.end	__start
	.size	__start, .-__start
