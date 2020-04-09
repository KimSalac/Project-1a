  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   loop: addi $6, $6, 4
   add $5, $5, $6
   addi $3, $1, 4
   beq $3, $5, loop
   addi $7, $1, 7
	.end	__start
	.size	__start, .-__start
