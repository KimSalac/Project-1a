  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi  $1, $0, 5
   addi  $2, $0, 10
   addi  $5, $0, 1
   addi  $7, $0, 3
   addi  $10, $0, 4
   addi  $11, $0, 13
   add   $12, $1, $2
	.end	__start
	.size	__start, .-__start
