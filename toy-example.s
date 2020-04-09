  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi  $1, $0, 5
   addi  $6, $0, 5
   addiu $2, $0, 0x0a
   andi  $3, $0, 0x0002
   beq   $6, $1, loop 
   add   $5, $0, 9
loop: addi $4, $0, 9

	.end	__start
	.size	__start, .-__start
