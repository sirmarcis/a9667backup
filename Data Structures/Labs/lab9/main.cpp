#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

int
main(int argc, char* argv[]){
    if(argc > 0){
        std::ifstream istr(argv[1]);
        if(!istr)
            std::cout<< "ERROR: could not open " << argv[1] << " for reading" << std::endl;
        std::map<int, int> inputMap;
        std::vector<int> inputArr;
        int nextInt;
        //Checkpoint 1 implementation
        /*
        while(istr >> nextInt){
            inputArr.push_back(nextInt);
            ++inputMap[nextInt];

        }
        */
        //checkpoint 2 implemantation

        while(istr >> nextInt){
            inputArr.push_back(nextInt);
            if(inputMap.find(nextInt) != inputMap.end()){
                std::map<int, int>::iterator oldVal = inputMap.find(nextInt);
                oldVal->second = oldVal->second + 1;
                //std::cout << inputMap[nextInt] <<std::endl;
            }
            else{
                //std::cout << "added new number " << nextInt << std::endl;
                inputMap.insert(std::pair<int, int>(nextInt, 1));
            }

        }

        std::map<int, int>::iterator it;
        int maxOccurance = 0;
        for(it = inputMap.begin(); it != inputMap.end(); ++it)
            if(it->second > maxOccurance)
                maxOccurance = it->second;
        std::map<int, int> modeVals;
        for(it = inputMap.begin(); it != inputMap.end(); ++it){
            //std::cout << "got to here 3.5: num: "  << it->first << ", " << it->second <<  ", " << maxOccurance << ", " << inputMap.size() <<std::endl;
            if(it->second != maxOccurance)
                inputMap.erase(it);
        }
        for(it = inputMap.begin(); it != inputMap.end(); ++it)
            std::cout << it->first << ", ";
        std::cout <<std::endl;

    }
    return 0;
}
