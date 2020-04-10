  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi  $10, $1, 0x000A
   addi $4, $1, 0x0002
   addi $4, $1, 0x0005
   add  $2, $10,  $4
   add $3, $10, $4
   sw $3, 0($3)
   lw $5, 0($3)
   sub $5, $10, $3
   add $7, $5, $5
    
	.end	__start
	.size	__start, .-__start
