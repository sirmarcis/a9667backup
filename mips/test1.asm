	.data
N:	.word	100
Sum:	.word	0

	.text
main:
	lw	$s0, N
	li	$s1, 0

loop:
	add	$s1, $s1, $s0     # update running total
	addi	$s0, $s0, -1      # decrement loop counter
	bne	$s0, $zero, loop  # repeat until counter reaches 0
	
	sw	$s1, Sum
	
	li	$v0, 10
	syscall
