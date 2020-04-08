  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
    addi $3, $4, 12      
	addi $6, $6, 9
	sh $6, 0($3)
	add $9, $3, $6
	.end	__start
	.size	__start, .-__start
