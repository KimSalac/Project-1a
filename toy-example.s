  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
    addi $3, $4, 7      
	sw $3, 0($4)
	addi $6, $6, 6
	add $9, $3, $6
	.end	__start
	.size	__start, .-__start
