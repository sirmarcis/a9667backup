#include <iostream>
#include <string>
#include <unordered_map>

int main(){
    std::unordered_map<std::string, int> testHash;
    testHash["hello"] = 12;
    testHash["there"] = 10;
    testHash["zzzz"] = 2;
    std::unordered_map<std::string, int>::iterator it;
    for(it = testHash.begin(); it != testHash.end(); it++)
        std::cout <<"key: " << it->first<<std::endl;
    std::cout << "Value for key 'hello': " << testHash["hello"] << ", value for 'there': " << testHash["there"] <<std::endl;
    return 0;
}
