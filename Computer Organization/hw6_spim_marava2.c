#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int* convert2bin(const int* num, int* binaryNum){ // convert a decimal number to a 16 bit binary using the divide by 2 method
	int intermediateNum = *num;
	int currPos = 15;
	while(intermediateNum != 0){ 
		if(intermediateNum%2 == 1){
			binaryNum[currPos] = 1;
			currPos--;
		}
		else{
			binaryNum[currPos] = 0;
			currPos--;
		}
		intermediateNum = intermediateNum/2;
	}
	while(currPos >= 0){
		binaryNum[currPos] = 0;
		currPos--;
	}
	return binaryNum;
}

void MIPSLUI(int* binaryNum1, int* luiArray){ // put num_1 in binary format into the first 16 bits of luiArray in reverse order
	int num1Pos = 15;
	int luiPos = 0;
	while(num1Pos >= 0){
		luiArray[luiPos] = binaryNum1[num1Pos];
		num1Pos--;
		luiPos++;
	}
}

void MIPSORI(int* binaryNum2, int* luiArray){ //fill in the rest of luiArray with num_2 in binary format in the same order for the ori command 
	int num1Pos = 0;
	int oriPos = 16;
	while(oriPos != 32){
		luiArray[oriPos] = binaryNum2[num1Pos];
		num1Pos++;
		oriPos++;
	}
}

void multiply(int* num_1, int* num_2, int* product){
	*product = 0; //set the initoial value of the product to zero
	int x = *num_1;
	int y = *num_2;
	while(y != 0){
		if(y&1) //bitwise & 01 to the value of y
			*product = *product + x; // if y is odd, add x to the product
		x <<= 1; //left shift the value in x
		y >>= 1; // right shift the value in y
	}
}

int divide(int* num_1, int* num_2, int* remain){
	int x = *num_1; // make ccopies of the inputted numbers to avoid conflicts 
	int y = *num_2;
	int quotent = 1; // inintialize quotent for this iteration
	if(x == y){ // ifthe numbers are equal, then the quotent is 1 and there is no remainder
		*remain = 0;
		return 1;
	} else if(x < y){ // if the dividend is greater then the divisor, return 0 as the quotent and the dividend as the remainder
		*remain = x;
		return 0;
	}
	do{ // bitshift the divisor and the quotent in anticipation of the recursive call
		y <<= 1;
		quotent <<= 1;
	}while(y <= x);
	int newNum = *num_1 - *num_2; // send in neew difference between num_1 and num_2 to the following recursive call
	quotent = quotent + divide(&newNum, num_2, remain); // call divide recursivly 
	return quotent;
}

char getHexVal(char* bGroup){
	if(bGroup[0] == '0' && bGroup[1] == '0' && bGroup[2] == '0' && bGroup[3] == '0') // 0000 = 0
		return '0';
	if(bGroup[0] == '0' && bGroup[1] == '0' && bGroup[2] == '0' && bGroup[3] == '1') // 0001 = 1
		return '1';
	if(bGroup[0] == '0' && bGroup[1] == '0' && bGroup[2] == '1' && bGroup[3] == '0') // 0010 = 2
		return '2';
	if(bGroup[0] == '0' && bGroup[1] == '0' && bGroup[2] == '1' && bGroup[3] == '1') // 0011 = 3
		return '3';
	if(bGroup[0] == '0' && bGroup[1] == '1' && bGroup[2] == '0' && bGroup[3] == '0') // 0100 = 4
		return '4';
	if(bGroup[0] == '0' && bGroup[1] == '1' && bGroup[2] == '0' && bGroup[3] == '1') // 0101 = 5
		return '5';
	if(bGroup[0] == '0' && bGroup[1] == '1' && bGroup[2] == '1' && bGroup[3] == '0') // 0110 = 6
		return '6';
	if(bGroup[0] == '0' && bGroup[1] == '1' && bGroup[2] == '1' && bGroup[3] == '1') // 0111 = 7
		return '7';
	if(bGroup[0] == '1' && bGroup[1] == '0' && bGroup[2] == '0' && bGroup[3] == '0') // 1000 = 8
		return '8';
	if(bGroup[0] == '1' && bGroup[1] == '0' && bGroup[2] == '0' && bGroup[3] == '1') // 1001 = 9
		return '9';
	if(bGroup[0] == '1' && bGroup[1] == '0' && bGroup[2] == '1' && bGroup[3] == '0') // 1010 = A
		return 'A';
	if(bGroup[0] == '1' && bGroup[1] == '0' && bGroup[2] == '1' && bGroup[3] == '1') // 1011 = B
		return 'B';
	if(bGroup[0] == '1' && bGroup[1] == '1' && bGroup[2] == '0' && bGroup[3] == '0') // 1100 = C
		return 'C';
	if(bGroup[0] == '1' && bGroup[1] == '1' && bGroup[2] == '0' && bGroup[3] == '1') // 1101 = D
		return 'D';
	if(bGroup[0] == '1' && bGroup[1] == '1' && bGroup[2] == '1' && bGroup[3] == '0') // 1110 = E
		return 'E';
	if(bGroup[0] == '1' && bGroup[1] == '1' && bGroup[2] == '1' && bGroup[3] == '1') // 1111 = F
		return 'F';
	return 'X'; // Exception handler
}

void binaryToHex(int* binaryNum, char* hexVal){
	char cGroup1[4]; // these four groups will hold the string representations of the 4 bit subsequences of binaryNum
	char cGroup2[4];
	char cGroup3[4];
	char cGroup4[4];
	char currChar;
	int x;
	for(x = 0; x < 4; x++){ // break up binaryNum into 4 subsequences
		currChar = '0'+ binaryNum[x];
		cGroup1[x] = currChar;
	}
	for(x = 0; x < 4; x++){
		currChar = '0'+ binaryNum[x+4];
		cGroup2[x] = currChar;
	}
	for(x = 0; x < 4; x++){
		currChar = '0'+ binaryNum[x+8];
		cGroup3[x] = currChar;
	}
	for(x = 0; x < 4; x++){
		currChar = '0'+ binaryNum[x+12];
		cGroup4[x] = currChar;
	}
	hexVal[0] = getHexVal(cGroup1); // convert each subsequence into hex
	hexVal[1] = getHexVal(cGroup2);
	hexVal[2] = getHexVal(cGroup3);
	hexVal[3] = getHexVal(cGroup4);
}

void bitwiseAnd(int* binaryNum1, int* binaryNum2, int* andNum){
	int x;
	for(x = 0; x < 16; x++){ //loop through the 16 bit binary
		if(binaryNum1[x] == binaryNum2[x] && binaryNum1[x] == 1) // 1 and 1 = 1
			andNum[x] = 1;
		else // all others evaluate to 0
			andNum[x] = 0;
	}
}

void bitwiseOr(int* binaryNum1, int* binaryNum2, int* orNum){
	int x;
	for(x = 0; x < 16; x++){
		if(binaryNum1[x] == binaryNum2[x] && binaryNum1[x] == 0) // 0 or 0 = 0
			orNum[x] = 0;
		else // all others evaluate to 1
			orNum[x] = 1;
	}
}

void bitwiseXor(int* binaryNum1, int* binaryNum2, int* xorNum){
	int x;
	for(x = 0; x < 16; x++){
		if(binaryNum1[x] == binaryNum2[x]) //  1 xor 1 and 0 xor 0 = 0
			xorNum[x] = 0;
		else // all others evaluate to 1
			xorNum[x] = 1;
	}
}

void bitwiseNot(int* binaryNum, int* notNum){
	int x;
	for(x = 0; x < 16; x++){ // 1's turn into 0's and 0's turn into 1's
		if(binaryNum[x] == 1)
			notNum[x] = 0;
		else
			notNum[x] = 1;
	}
}

int main(int argc, char* argv[]){
	FILE* input;  //input file
	FILE* output; //output destination file
	int num_1; // will get value from an input file, same for num_2
	int num_2;
	int binaryNum1[16]; // will store the binary representation of num_1
	int binaryNum2[16]; // will store the binary representation of num_2
	int bitwiseNum[16]; // storage place of the the bitwise logical operations
	int* luiArray; // will store the mips lui and ori instruction
	int product; // will store the result of the bitwise multiplication
	int quotent; // will store the result of the bitwise division
	int remain; // whatever is left over from division
	char hexVal[4]; // stores a 16 bit hexadecimal value
	int x; // looping variable
	if(argc != 3){ // check to see that there are the correct number of command line inputs
		printf("Usage: %s input.txt output.txt\n", argv[0]);
		return(1);
	}
	input = fopen(argv[1], "r");
	if(input == NULL){ //check to see if the file opened successfully
		printf("error opening file\n");
		return(1);
	}
	fscanf(input, "%d", &num_1);//get the first number from the file
	fscanf(input, "%d", &num_2);//get the second number form the file
	fclose(input); //close the input file stream

	output = fopen(argv[2], "w"); //open the output file in write out format
	convert2bin(&num_1, binaryNum1); // convert num_1 to a 16 bit binary representation
	convert2bin(&num_2, binaryNum2); // convert num_2 to a 16 bit binary representation
	fprintf(output, "%d\t%d\n", num_1, num_2); // write num_1 and num_2 to the output seperated by a tab, all on one line
	for(x = 0; x < 16; x++) 
		fprintf(output, "%d", binaryNum1[x]); //write each bit of binarynum1 to the second line of the output, seperated by a tab
	fprintf(output, "\t");
	for(x = 0; x < 16; x++)
		fprintf(output, "%d", binaryNum2[x]); //write each bit of binarynum2 to the second line of the output, seperated from binarynum1 by a tab
	fprintf(output, "\n");
	luiArray = (int*)calloc(32, sizeof(int)); // initialize the luiArray to the size of a 32 bit command
	MIPSLUI(binaryNum1, luiArray); // call to set the first 16 bits of luiArray to a MIPS lui command
	for(x = 0; x < 32; x++) // add the result of the MIPSLUI function call to the output file
		fprintf(output, "%d", luiArray[x]);
	fprintf(output, "\n");
	MIPSORI(binaryNum2, luiArray); // fill in the rest of luiArray with the ori command found in binaryNum2
	for(x = 0; x < 32; x++) // add the result of the function call MIPSORI to the outupt file
		fprintf(output, "%d", luiArray[x]);
	fprintf(output, "\n");
	multiply(&num_1, &num_2, &product); // multiply num_1 and num_2 using bitshift operators and put the result into the output file
	fprintf(output, "%d\n", product); // multiplication in the output file

	quotent = divide(&num_1, &num_2, &remain); // divide num_1 and num_2 using bitshift operators 
	fprintf(output, "%d\t%d\n", quotent, remain); // put the division result into the output file 

	bitwiseAnd(binaryNum1, binaryNum2, bitwiseNum); // preform logical bitwise and operation on teh binaries of num_1 and num_2
	for(x = 0; x < 16; x++) // write the bitwise number to the output file
		fprintf(output, "%d", bitwiseNum[x]);
	fprintf(output, "\t");
	binaryToHex(bitwiseNum, hexVal); // get the hex value of the bitwise and result
	for(x = 0; x < 4; x++) // write the hex val to the output file
		fprintf(output, "%c", hexVal[x]);
	fprintf(output, "\n"); 

	bitwiseOr(binaryNum1, binaryNum2, bitwiseNum); // preform logical bitwise or operation on teh binaries of num_1 and num_2
	for(x = 0; x < 16; x++) // write the bitwise number to the output file
		fprintf(output, "%d", bitwiseNum[x]);
	fprintf(output, "\t");
	binaryToHex(bitwiseNum, hexVal); // get the hex value of the bitwise and result
	for(x = 0; x < 4; x++) // write the hex val to the output file
		fprintf(output, "%c", hexVal[x]);
	fprintf(output, "\n"); 

	bitwiseXor(binaryNum1, binaryNum2, bitwiseNum); // preform logical bitwise xor operation on teh binaries of num_1 and num_2
	for(x = 0; x < 16; x++) // write the bitwise number to the output file
		fprintf(output, "%d", bitwiseNum[x]);
	fprintf(output, "\t");
	binaryToHex(bitwiseNum, hexVal); // get the hex value of the bitwise and result
	for(x = 0; x < 4; x++) // write the hex val to the output file
		fprintf(output, "%c", hexVal[x]);
	fprintf(output, "\n"); 

	bitwiseNot(binaryNum2, bitwiseNum); // preform logical bitwise not operator on num_2's binary representation
	for(x = 0; x < 16; x++) // write the bitwise number to the output file
		fprintf(output, "%d", bitwiseNum[x]);
	fprintf(output, "\t");
	binaryToHex(bitwiseNum, hexVal); // get the hex value of the bitwise and result
	for(x = 0; x < 4; x++) // write the hex val to the output file
		fprintf(output, "%c", hexVal[x]);
	fprintf(output, "\n"); 

	free(luiArray); // free the memory used in luiArray
	fclose(output); // close the output file

	return(0);
}