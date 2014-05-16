#include <string>
#include <iostream>
#include <sstream>

#include "superhero.h"

bool Superhero::operator>(const Superhero& lft){
    if((lft.getPower() == "fire" && power == "wood") || (lft.getPower() == "wood" && power == "water") || (lft.getPower() == "water" && power == "fire"))
        return false;
    else if(lft.getPower().size() > power.size()) return false;
    else return true;
}

std::ostream& operator<<(std::ostream& ostr, const Superhero& hero){
    ostr << "Superhero " << hero.getName() << " has power " << hero.getPower() << "\n";
    return ostr;
}

void operator-(Superhero& hero){
    hero.changeSide();
}
