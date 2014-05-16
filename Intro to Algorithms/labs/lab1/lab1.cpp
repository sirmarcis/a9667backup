

#include <iostream>
#include <vector>
#include <algorithm> 
#include <math.h>
#include "points.h"


void print(std::vector<int> blah)
{
  for(int i = 0; i < blah.size(); i++)
  {
    std::cout << blah[i] << std::endl;
  }
}

//the order notation of this function is O(n)
std::vector<int> findMissing(int arr1[], int n){
    std::sort(arr1, arr1+n);
    std::vector<int> missingVals;
    const int highestVal = pow(n , 2);
    std::vector<int> content(2 * n, highestVal);
    for(int i = 0; i < (pow(2, n) -1 ); ++i){
      content[arr1[i]] = arr1[i];
      
    }
    for(int x = 0; x < highestVal -1 ; ++x){
        if(content[x] != x){
            missingVals.push_back(x);
        }
    }
    return missingVals;
  }


bool sumToZeroPresent(int elts[], int n){
  std::sort(elts, elts+n);
  for(int x = 0; x < n; x++){
    if(std::binary_search(elts, elts+n, -1* elts[x]))
      return true;
  }
  return false;
}

//the runtime of question 3 is O(n^2) due to the embbeded for loop that starts at the point after the one the main loop is alreeady on 
double shortestDistance(std::vector<Point>& points){
  double currShortestDistane = 10000000;
  double currDistance = 0;
  for(int h = 0; h < points.size(); h++){
    if(h+1 < points.size()){
      for(int k = h + 1; k <  points.size(); k++){
        currDistance = points[h].dist(points[k]);
        if(currDistance < currShortestDistane)
          currShortestDistane = currDistance;
      }
    }
  }
  return currShortestDistane;
}

int main(){
  int theArr[8] = {8,5,3,4,2,1};
  std::cout << "question 1 numbers missing: " <<std::endl;
  std::vector<int> testerVec = findMissing(theArr, 3);
  print(testerVec);
  int testArr[10] = {9, 7,8,6,4,5,3,1,2, 11};
  std::cout << "question 2 there is a sum to zero: " << sumToZeroPresent(testArr, 10) <<std::endl;
  std::vector<Point> testPoints;
  testPoints.push_back(Point(1,2));
  testPoints.push_back(Point(4,3));
  testPoints.push_back(Point(0,1));
  testPoints.push_back(Point(1,2));
  std::cout << "question 3 shortest distance: " << shortestDistance(testPoints) << std::endl; 
  return 0;
}