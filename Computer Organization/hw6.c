#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

int division(int* x, int* y){
	int ans;
	int temp = *x;
	while(temp >= (*y)){
		temp = temp - (*y);
		ans++;
	}
	if(temp != 0)
		printf("%d / %d = %d remainder: %d\n\n", (*x), (*y), ans, temp);
	else
		printf("%d / %d = %d\n\n", (*x),(*y),ans);
	ans = 0;
}

int preformOperation( int* x, int* y, char arg[]){
	char theArg = arg[0];
	if(theArg == '+')
		return (*x)+(*y);
	else if(theArg == '-')
		return (*x)-(*y);
	else if(theArg == '*')
		return (*x)*(*y);
	else if(theArg == '/')
		return division(x, y);
	else
		printf("Invalid Operator!\n");
}

int main(int argc, char* argv[]){
	printf("My C calculator 1.0!\n");
	char arg[100];
	bool runAnotherCalc = true;
	char anotherCalc[100];
	char* xVal;
	char* yVal;
	int x, y, ans;
	while(runAnotherCalc == true){
		printf("Enter the first number: ");
		scanf("%d", &x);
		printf("Enter the second number: ");
		scanf("%d", &y);
		printf("Enter the operation (+, -, *, /):");
		scanf("%s", &arg);
		ans = preformOperation(&x, &y, arg);
		if(arg[0] != '/')
			printf("%d %s %d = %d\n\n",x, &arg, y, ans);
		printf("Another Calculation (y, n)? ");
		scanf("%s", &anotherCalc);
		if(anotherCalc[0] == 'y')
			runAnotherCalc = true;
		else
			runAnotherCalc = false;
	}
	printf("Program Terminated\n");
	return(0);
}