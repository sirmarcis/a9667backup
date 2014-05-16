#ifndef COSTUME_H_INCLUDED
#define COSTUME_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Costume{
public:
    Costume();
    Costume(std::string nName, int nCopies);
    const std::string& getCostumeName() const { return cosName; }
    const int& getNumRentals() const { return availableRentals; }
    const int& getNumCopies() const { return copies; }
    const std::vector<std::string>& getRentalList() const{ return rentalList; }
    void rent(const std::string& customer, std::map<std::string, std::string>& people ,std::map<std::string, Costume>& costume_shop);
    void returnRental(const std::string& customer);
    void addRentals(int nCopies) { copies += nCopies; availableRentals += nCopies; }

private:
    std::string cosName;
    std::vector<std::string> rentalList;
    int copies, availableRentals;
};

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




#endif // COSTUME_H_INCLUDED
