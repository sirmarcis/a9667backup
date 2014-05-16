/**
	@file
	@author Andrew D. Zonenberg
	@brief Test program
 */

#include "registers.h"

.data
msg:	.asciiz "Hello World!\n"

.text
.globl __start
__start:
	//Simulator loads $sp with a valid address before starting program
	li		v0, 4
	la		a0, msg
	syscall	

	li		v0, 10			
	syscall
