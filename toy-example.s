  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   loop: addi $6, $6, 4
   addi $5, $1, 4
   beq $5, $6, loop
   addi $3, $5, 6
   addi $7, $1, 6
	.end	__start
	.size	__start, .-__start
