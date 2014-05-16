#include <iostream>
#include <list>
#include "dvd.h"
#include "customer.h"

int
main(int argc, char* argv[]){
    DVD d1("Hello", 2);
    d1.sub_copy();

    Customer c1("john Smith");
    c1.add_rental("hello");
    c1.add_rental("there");
    c1.add_rental("world");
    c1.remove_rental("is");
    std::cout << c1.get_name() << " "  << d1.get_name() << " " << c1.find_rental("hello") << std::endl;
    return 0;
}
