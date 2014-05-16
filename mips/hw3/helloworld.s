# helloworld.s
#CSCI-2500 Fall 2013
# Print out "Hello World"

        .data
msg:   	.asciiz "Hello World!"

       	 .text
        .globl main
main:   li $v0, 4       # syscall 4 (print_str)
       	la $a0, msg     # argument: string
        syscall         # print the string
        jr $ra          # retrun to caller
			
			

 

