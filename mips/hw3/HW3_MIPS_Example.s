# helloworld.s
#CSCI-2500 Fall 2014
# Print out "Calculator"

        	.data
msg:    	.asciiz "Calculator!"
newLine:   	.asciiz "\n"   #line return
prompt: 	.asciiz "\nEnter a letter, then hit return:"
exitMsg:    .asciiz "\nGood bye."

a0_op:  	.space 1  #address/space to hold a character input

        	.text
        	.globl main
main:   	li $v0, 4       # syscall 4 (print_str)
			la $a0, msg     # argument: string
        	syscall         # print the string

			la $a0, newLine # argument:
        	syscall         # print the string

			la	$a0, prompt        #"Enter a letter	prompt string	
			syscall 
		
			addi $v0, $zero, 8     #syscall 4 (read_str)
			la $a0, a0_op          #load to $a0 the address to hold the read operator
			syscall

			li $v0, 4        # syscall 4 (print_str)
			syscall

			la  $a0, exitMsg #string is a line return
			syscall
			#jr, $ra         #return to caller
		
			li $v0, 10       #syscall 10 (exit the program)
			syscall          
			
			

 

