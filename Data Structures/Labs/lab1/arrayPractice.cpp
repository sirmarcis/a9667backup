#include <iostream>

int
main(){
	std::cout << "input the number of elements followed by those elements: ";
	int arrSize;
	std::cin >> arrSize;
	float arr[arrSize], avg = 0;
	for (int cnt = 0; cnt < arrSize; ++cnt){
		std::cin >> arr[cnt];
		avg += arr[cnt];
	}
	avg = avg / arrSize;
	std::cout << "the avreage in this array is " << avg << " and numbers that are above it are:" << std::endl;
	for (int cnt = 0; cnt < arrSize; ++cnt){
		if(arr[cnt] > avg)
			std::cout << arr[cnt] << std::endl;
	}
	return 0;
}
