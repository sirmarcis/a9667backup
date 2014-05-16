#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "costume.h"

typedef std::map<std::string, Costume> COSTUME_SHOP_TYPE;
typedef std::map<std::string, std::string> PEOPLE_TYPE;


void addCostume(COSTUME_SHOP_TYPE& costume_shop) {
    std::string costume_name;
    int num;
    std::cin >> costume_name >> num;
    Costume costume;
    if(costume_shop.find(costume_name) != costume_shop.end()){ //if the costume already exists, add more copies of that costume
        costume = costume_shop[costume_name];
        costume.addRentals(num);
    }
    else  //else, create a new costume
        costume = Costume(costume_name, num);
    costume_shop[costume_name] = costume;
    if(num > 1)
        std::cout << "Added " << num << " " << costume_name << " costumes." <<std::endl;
    else
        std::cout << "Added " <<num << " " << costume_name << " costume." << std::endl;

}

void rentCostume(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people) {
    std::string first_name;
    std::string last_name;
    std::string costume_name;
    std::cin >> first_name >> last_name >> costume_name;
    std::string customerName = first_name + " " + last_name;
    if(costume_shop.find(costume_name) == costume_shop.end()){  //check to see that the costume exists
        std::cout << "The shop doesn't carry " << costume_name << " costumes." <<std::endl;
        if(people.find(customerName) == people.end())
            people[customerName] = "";
        return;
    }
    Costume costume = costume_shop[costume_name];  //otherwise attempt to rent it
    costume.rent(customerName, people, costume_shop);
    costume_shop[costume_name] = costume;

}

void lookup(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people) {
    std::string costume_name;
    std::cin >> costume_name;
    if(costume_shop.find(costume_name) == costume_shop.end()){  //check to see that the costume exists
        std::cout << "The shop doesn't carry " << costume_name << " costumes." <<std::endl;
        return;
    }
    //otherwise print that costumes information
    Costume costume = costume_shop[costume_name];
    std::vector<std::string> customersWithRentals = costume.getRentalList();
    std::cout << "Shop info for " << costume_name << " costumes:" << std::endl;
    int copiesAvailable = costume.getNumRentals();
    if(copiesAvailable == 1)
        std::cout << "  1 copy available" <<std::endl;
    else if(copiesAvailable > 1)
        std::cout << "  " << copiesAvailable << " copies available" <<std::endl;
    copiesAvailable = costume.getNumCopies() - copiesAvailable;
    if(copiesAvailable == 1)
        std::cout << "  1 copy rented by:" <<std::endl;
    else if(copiesAvailable > 1)
        std::cout << "  " << copiesAvailable << " copies rented by:" <<std::endl;
    for(int x = 0; x < customersWithRentals.size(); x++){
        std::cout << "    " << customersWithRentals[x] <<std::endl;
    }
}

bool sortByLastName(const std::string& left, const std::string& right){
    std::string leftFirstName, rightFirstName, leftLastName, rightLastName;
    int leftSpacePos = left.find(' ');
    int rightSpacePos = right.find(' ');
    leftFirstName = left.substr(0, leftSpacePos);
    leftLastName = left.substr(leftSpacePos);
    rightFirstName = right.substr(0, rightSpacePos);
    rightLastName = right.substr(rightSpacePos);
    return leftLastName < rightLastName ||
    (leftLastName == rightLastName && leftFirstName < rightFirstName);
}

void printPeople(PEOPLE_TYPE& people) {
    //I know this is not printing these names in the right order, sorry
    if(people.size() == 1)
        std::cout << "Costume info for the " << people.size() << " party attendee:" <<std::endl;
    else
        std::cout << "Costume info for the " << people.size() << " party attendees:" <<std::endl;
    std::map<std::string, std::string>::iterator it;
    std::vector<std::string> temp;
    for(it = people.begin(); it != people.end(); it++){
        temp.push_back(it->first);
    }
    std::sort(temp.begin(), temp.end(), sortByLastName);
    for(int x = 0; x < temp.size(); x++){
        std::string cosName = people[temp[x]];
        if(cosName == "")
            std::cout << "  " << temp[x] << " does not have a costume." <<std::endl;
        else
            std::cout << "  " << temp[x]  << " is wearing a " << cosName << " costume." <<std::endl;
    }


}

int main() {
    // two data structures store all of the information for efficiency
    COSTUME_SHOP_TYPE costume_shop;
    PEOPLE_TYPE people;
    char c;
    //std::cout << "type q when done to quit program" << std::endl;
    while (std::cin >> c) {
        if (c == 'a') {
            addCostume(costume_shop);
        } else if (c == 'r') {
            rentCostume(costume_shop,people);
        } else if (c == 'l') {
            lookup(costume_shop, people);
        } else if (c == 'p') {
            printPeople(people);
        } else if(c == 'q')
            return 0;
        else {
            std::cout << "error unknown char " << c << std::endl;
            return 0;
        }
    }
}
