#include <iostream>

#include "customer.h"

Customer::Customer(){
    name = "";
    firstName = "";
    lastName = "";
    currCostume = "";
    costumeRented = false;
}

Customer::Customer(std::string nFirstName, std::string nLastName, std::string costume, std::map<std::string, int>& costume_shop){
    firstName = nFirstName;
    lastName = nLastName;
    name = nFirstName + " " + nLastName;
    currCostume = costume;
    costumeRented = true;
    std::cerr << name << " rents a " << costume << " costume." <<std::endl;
    costume_shop[costume]--;
}

void Customer::rentCostume(const std::string newCostume, std::map<std::string, int>& costume_shop){
    if(costumeRented){
        costume_shop[currCostume]++;
        std::cerr << name << " returns a " << currCostume << " costume before renting a " << newCostume << " costume." <<std::endl;
    }
    else{
        costumeRented = true;
        std::cerr << name << " rents a " << newCostume << " costume." <<std::endl;
    }
    currCostume = newCostume;
    costume_shop[newCostume]--;
}
