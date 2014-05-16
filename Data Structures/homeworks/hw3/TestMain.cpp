#include <iostream>
#include "UndoArr.h"

int
main(){
    UndoArray<char> arr1(7);
    arr1.set(3, 'R');
    arr1.set(3, 'D');
    arr1.undo(3);
    std::cout << arr1.get(3);
    //arr1.print();
    return 0;
}
