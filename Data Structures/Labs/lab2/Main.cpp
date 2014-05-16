#include <iostream>
#include <algorithm>
#include <vector>
#include "Time.h"


int
main(){
    Time t1(13, 30, 15);
    Time t2(12, 30, 15);
    Time t3(14, 25, 45);
    Time t4(0, 12, 44);
    std::vector<Time> times;
    times.push_back(t1); times.push_back(t2); times.push_back(t3); times.push_back(t4);
    sort(times.begin(), times.end(), isEarlierThan);
    for(int x = 0; x < times.size(); x++){

        std::cout << "time " << x << " is  ";
        times[x].printAmPm();
    }
    //t1.printAmPm();
    //std::cout << "hours: " << t1.getHour() << " minutes: " << t1.getMinute() << " seconds: " << t1.getSecond() << std::endl;
    return 0;
}
