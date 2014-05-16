#include <iostream>

void stepToOrigin(int xPos, int yPos, int &numPaths){
    if(xPos > 0)
        stepToOrigin(xPos - 1, yPos, numPaths);
    if(yPos > 0)
        stepToOrigin(xPos, yPos - 1, numPaths);
    if(xPos == 0 && yPos == 0)
        numPaths++;
}



int main(){
    int numPaths = 0;
    int xPos = 12, yPos = 8;
    stepToOrigin( xPos, yPos, numPaths);
    std::cout << "num paths from: " << xPos << ", " << yPos << " is " << numPaths << std::endl;
    return 0;
}
