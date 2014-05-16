#include <iostream>

//  O(n^2)
int findPrimes(int arr1[], int n)
{
	int numPrimes;
	int index = 0;
	for(int i = 3; i < n; i++)
	{
		bool isprime = true;
		for(int x = 2; x < i; x++)
		{
			if(i % x == 0)
			{
				isprime = false;
			}
		}

		if(isprime)
		{
			arr1[index] = i;
			index++;
		}
			
	}
	return index;
}
/*
We can reach base case of GCD with 2n recursive calls. and mod is n^2 then you multiply those together and gcd
is n^3. LCM is just constant run time so nothing goes into that all the work is done in GCD

overall runtime = 0(n) = n^3
*/

int GCD(int a, int b){
	if(b == 0)return a;
	else return GCD(b, a % b);
}

int LCM(int a, int b){
	int theLCM;
	int theGCD = GCD(a,b);
	int temp = a * b;
	theLCM = temp / theGCD;
	return theLCM;
}

void swap(int arr[], int currIndex, int destIndex){
	int oldVal = arr[destIndex];
	arr[destIndex] = arr[currIndex];
	arr[currIndex] = oldVal;
}


//this function is to the order notation O(n), because it only loops over the array three times
void split(int S[], int n, int v){
	int currIndex = 0;
	for(int x = 0; x < n; x++){
		if(S[x] < v){
			swap(S, x, currIndex);
			currIndex++;
		}
	}
	for(int x = 0; x < n; x++){
		if(S[x] == v){
			swap(S, x, currIndex);
			currIndex++;
		}
	}
	for(int x = 0; x < n; x++){
		if(S[x] > v){
			swap(S, x, currIndex);
			currIndex++;
		}
	}
}

int main(){
	int primesToBeFound[9];
	int primesFound = findPrimes(primesToBeFound, 9);
	std::cout << "number of primes found between 3 and 9: " << primesFound << std::endl;  // question 1
	for(int x = 0; x < primesFound; x++)
		std::cout << "prime at " << x << " is " << primesToBeFound[x] << std::endl;
	

	std::cout << "lcm between 15 and 10: " << LCM(15, 10) <<std::endl;
	int primes[7] = { 3,7, 5 ,6 ,8, 12, 4};
	std::cout << "old array: ";
	for(int x = 0; x < 7; x++)
		std::cout << primes[x] << ", ";
	std::cout << std::endl;
	split(primes, 7, 6);
	std::cout << "new array: ";
	for(int x = 0; x < 7; x++)
		std::cout << primes[x] << ", ";
	std::cout << std::endl;
	return 0;
}