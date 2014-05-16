#ifndef CUSTOMER_H_INCLUDED
#define CUSTOMER_H_INCLUDED

#include <string>
#include <map>

class Name {
public:
    Name(const std::string& first, const std::string& last) :
    m_first(first), m_last(last) {}
    const std::string& first() const { return m_first; }
    const std::string& last() const { return m_last; }
private:
    std::string m_first;
    std::string m_last;
};


class Customer{
public:
    Customer();
    Customer(std::string nFirstName, std::string nLastName, std::string costume, std::map<std::string, int>& costume_shop);

    bool hasRental()const{ return costumeRented; }
    void rentCostume(const std::string newCostume, std::map<std::string, int>& costume_shop);
    void returnCostume();
    std::string getCurrCostume() const{ return currCostume; }
    std::string getName() const{ return name; }
    const std::string& first() const{ return firstName; }
    const std::string& last() const{ return lastName; }

private:
    std::string name;
    std::string firstName, lastName;
    std::string currCostume;
    bool costumeRented;
};



#endif // CUSTOMER_H_INCLUDED
