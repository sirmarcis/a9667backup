# Computer Organization homework 3
# Anders Maraviglia
# Calculator

			.data
openMsg:	.asciiz "Welcome to SPIM Calculator 1.0!"
newLine:	.asciiz "\n" 			# line return
firstArg:	.asciiz "\nEnter the first number:"
secondArg:	.asciiz "Enter the second number:"
operator:	.asciiz "Enter the operation (+,-,*,/), then press enter key: "
anotherCalc:.asciiz "\n\nAnother Calculation (y/n)? " 
exitMsg:	.asciiz "\n\n Calculator Terminated.\n"

spaceC: 	.asciiz " " 	
invOpMsg:	.asciiz "Invalid Operator!"
eqOp:		.asciiz " = "
oParen:		.asciiz " ("
cParen:		.asciiz ")"

a0_op:			.word 0 			# address/space to hold an integer input
a1_op:			.word 0 			# address/space to hold an integer input
operation:		.space 16 			# address/space to hold the operator input
loopingBool:	.space 16 			# address/space to hold whether to preform another calculation 

			.text 
			.globl main	
# OPERATION BRANCHES (not subroutines, are the intermediary steps if a subroutine is required, and contains the whole operation if not)			
addOp:		add $s0, $a0, $a1 		# add the aruments stored in $a0 and $a1
			j print					# return to main to print solved function
subOp:		sub $s0, $a0, $a1		# subtract the aruments stored in $a0 and $a1
			j print					# return to main to print solved function
multi:		jal multiply			# call the subroutine multiply
			j print					# return to main
divi:		jal divide				# call the subroutine divide
			j print					# return to main to print solved function
divPrint:	li $v0, 4				# syscall 4 (print_str)
			la $a0, oParen			# open parenthesis
			syscall	
			li $v0, 1				# syscall 1 (print_int)
			move $a0, $s1			# print the remainder of the division loop
			syscall		
			li $v0, 4				# syscall 4 (print_str)
			la $a0, cParen			# close parenthesis
			syscall
			j query					# continue on to after the function is normally printed
			
# MULTIPLY SUBROUTINE (assumes arguments are stored in $a0 and $a1)
multiply:	sub $sp, $sp, 4         # make room for $t0
			sw $t0, 0($sp)          # save $t0 to the stack
			add $t0, $zero, $zero   # start, $t0 = 0
			
multLoop: 	beq $a1, $zero, multEol # loop on $a1
			add $t0, $t0, $a0    	# add another to $a0
			sub $a1, $a1, 1         # $a1 is a counter
			j multLoop			
			
multEol:	
			move $s0, $t0			# save the result in repository $s0
			lw $t0, 0($sp)  		# restore $t0
			add $sp, $sp, 4			# restore the stack
			jr $ra					# return to caller
		
# DIVIDE SUBROUTINE (assumes argumetnts are stored in $a0 and $a1)
divide:		sub $sp, $sp, 8			# make room for $t0
			sw $t0, 0($sp)			# save $t0 to the stack
			sw $t1, 4($sp)			# save $t1 to the stack
			add $t0, $zero, $a0		# start, $t0 = $a0 (will hold the remainder)
			add $t1, $zero, $zero	# start, set counter $t1 = 0
divLoop:	bgt	$a1, $t0, divEol	# loop while $t0 is still greater or equal to the divisor ($a1)
			sub $t0, $t0, $a1		# subtract one level of divisor from whats left in $t0
			add $t1, $t1, 1			# increment the counter ($t1++)
			j divLoop
divEol:		move $s0, $t1			# store the quotent in repository $s0
			move $s1, $t0			# store the remainder in repository $s1
			lw $t0, 0($sp)			# restore $t0
			lw $t1, 4($sp)			# restore $t1
			add $sp, $sp, 8			# restore the stack
			jr $ra					# return to caller
			
# BEGIN MAIN FUNCTION			
main:		li $v0, 4       		# syscall 4 (print_str)
			la $a0, openMsg 		# print the opening string
        	syscall         		

			la $a0, newLine 		# print a new line
        	syscall         		
			
loop:		li $v0, 4				# syscall 4 (print_str)
			la	$a0, firstArg  		# prompt for the first integer argument
			syscall 
			
			li $v0, 5     			# syscall 5 (read_int)
			syscall
			sw $v0, a0_op          	# load to $a0 the address to hold the read operator
			
			li $v0, 4          		# syscall 4 (print_str)
			la $a0, secondArg  		# prompt for the second integer argument
			syscall
			
			li $v0, 5				# syscall 5 (read_int)
			syscall
			sw $v0, a1_op      		# load to $a0 the address to hold the read operator
			
			li $v0, 4         		# syscall 4 (print_str)
			la $a0, operator  		# query for which operator to use
			syscall
			
			addi $v0, $zero, 8  	# syscall 8 (read_str)
			la $a0, operation   	# store the operation to use within a word
			li $a1, 16				# allocate size for the operation string
			syscall
			
			#remove the linefeed from the operator string in oder to just deal with the ascii number of the operator (and makes printing easier)
			li $s0,0        		# Set index to 0
remove:		lb $a3,operation($s0)   # Load character at index
			addi $s0,$s0,1      	# Increment index
			bnez $a3,remove     	# Loop until the end of string is reached
			beq $a1,$s0,skip    	# Do not remove \n when string = maxlength
			sub $s0,$s0,2     		# If above not true, Backtrack index to 
			sb $0, operation($s0)   # Add the terminating character in its place
			
skip:		la $a0, operation		# load the inputted operation into repository $a0
			li $s0, 43				# store the numerical representation for the string + in repository $s0
			li $s1, 45				# store the numerical representation for the string - in repository $s1
			li $s2, 42				# store the numerical representation for the string * in repository $s2
			li $s3, 47				# store the numerical representation for the string / in repository $s3
			lw $s4, 0($a0)			# store the inputted operator in repository $s4
			lw $a0, a0_op			# load in the first argument integer into $a0
			lw $a1, a1_op			# load in the second argument integer into $a1
			beq $s0, $s4, addOp		# if the operator is +, branch to addOp and then to cont
			beq $s1, $s4, subOp		# if the operator is -, branch to subOp and then to cont
			beq	$s2, $s4, multi 	# if the operator is *, run the multiply subroutine and branch to cont
			beq $s3, $s4, divi		# if the operator is /, run the divide subroutine and branch to cont
			
			li $v0, 4				# syscall 4 (print_str)
			la $a0, invOpMsg		# display error message for any string that is not an operator
			syscall
			j query					# skip printing formula and go directly to asking for another calculation 
						
print:		#the following is to print the result in equation form
			li $v0, 1				# syscall 1 (print_int)
			lw $a0, a0_op			# print the first argument
			syscall
			
			li $v0, 4				# syscall 4 (print_str)
			la $a0, spaceC			# add a space
			syscall
			la $a0, operation		# print the operation
			syscall
			la $a0, spaceC			# add a space
			syscall
			
			li $v0, 1				# syscall 1 (print_int)
			lw $a0, a1_op			# print the second argument
			syscall
			
			li $v0, 4				# syscall 4 (print_str)
			la $a0, eqOp			# print out the symbol = for the equation
			syscall
			
			li $v0, 1				# syscall 1 (print_int)
			move $a0, $s0			# print the result of the calculation 
			syscall
			
			beq $s3, $s4, divPrint	# handles division print case where remainder must be printed as well
			
query:		li $v0, 4           	# syscall 4 (print_str)
			la $a0, anotherCalc 	# query whether to do another calculation
			syscall
			
			addi $v0, $zero, 8 		# syscall 8 (read_str)
			la $a0, loopingBool 	# store the query for whether to continue looping
			li $a1, 16				# allocate space for the string input
			syscall 
			 
			li $s0, 2670       		# load string ascii value for 'n', which is 2670 (neither are the ascii values, they just have linefeeds on the end so they're values are off)
			li $s1, 2681			# load string ascii value for 'y', which is 2681
			lw $s2, 0($a0) 			# load result of whether to continue looping or not, and will branch when $t1 is equal to $t2
			beq $s0, $s2, exit 		# break the loop if the statement evaluates to true
			beq $s1, $s2,  loop		# continue looping if answer is yes
exit:
			
			li $v0, 4        		# syscall 4 (print_str)
			la  $a0, exitMsg 		# string is a line return (ALMOST END OF PROGRAM)
			syscall
			
			li $v0, 10       		# syscall 10 (exit the program)
			syscall 

