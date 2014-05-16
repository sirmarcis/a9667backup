#include <iostream>

#include "costume.h"

Costume::Costume(){
    cosName = "";
    copies = 0;
}

Costume::Costume(std::string nName, int nCopies){
    cosName = nName;
    copies = availableRentals = nCopies;
}

void Costume::rent(const std::string& customer, std::map<std::string, std::string>& people ,std::map<std::string, Costume>& costume_shop){
    if (availableRentals == 0){ //check to see if there are any costumes left to rent
        std::cout << "No " << cosName << " costumes available." <<std::endl;
        if(people.find(customer) == people.end())
            people[customer] = "";
        return;
    }
    for(int x = 0; x < rentalList.size(); x++){ //see if this customer has already rented this costume
        if(customer == rentalList[x]){
            std::cout << customer << " already has a " << cosName << " costume." <<std::endl;
            return;
        }
    }
    rentalList.push_back(customer);
    if(people.find(customer) != people.end() && people[customer] != ""){ // see if the customer has already rented another costume, and return the other if he/she has
        costume_shop[people[customer]].returnRental(customer);
        std::cout << customer << " returns a " << people[customer] << " costume before renting a " << cosName << " costume." <<std::endl;
    }
    else  // otherwise just rent the costume
        std::cout << customer << " rents a " << cosName << " costume." <<std::endl;
    people[customer] = cosName;
    availableRentals -= 1;
}

void Costume::returnRental(const std::string& customer){
    //adds the rental back into whats available, and removes the customer from the list of rentees
    availableRentals++;
    std::vector<std::string> newRentalList;
    for(int x = 0; x < rentalList.size(); x++){
        if(rentalList[x] != customer)
            newRentalList.push_back(rentalList[x]);
    }
    rentalList = newRentalList;
}

