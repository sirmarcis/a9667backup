#ifndef SUPERHERO_H_INCLUDED
#define SUPERHERO_H_INCLUDED

#include <string>
#include <iostream>
#include <sstream>

class Superhero;

#include "team.h"


class Superhero{
public:
    friend Team;
    Superhero(const std::string nName, const std::string nTrueIdent, const std::string nPower)
        : name(nName), trueIdentity(nTrueIdent), power(nPower), villanp(true) {}
    const std::string getName() const { return name; }
    const std::string getPower() const { return power; }
    const std::string getTrueIdentity() const { return trueIdentity; }

    bool operator==(const std::string& lft) const { return lft == trueIdentity; }
    bool operator!=(const std::string& lft) const { return lft != trueIdentity; }
    bool operator>(const Superhero& lft);

    void changeSide() { villanp = !villanp; }
    bool isGood() const { return villanp; }
private:
    std::string name, trueIdentity, power;
    bool villanp;
};

std::ostream& operator<<(std::ostream& ostr, const Superhero& hero);
void operator-(Superhero& hero);

#endif // SUPERHERO_H_INCLUDED
