#include <iostream>

#include "bidirectional_map.h"

int main(){
    bidirectional_map<int, std::string> newBMap;
    newBMap.insert(std::make_pair(1, "hello"));
    if(newBMap.find("hello") == newBMap.end())
        std::cout << "the value pair was not put in the tree" <<std::endl;
    else
        std::cout << "the value pair was successfully put in the tree" <<std::endl;
    return 0;
}
