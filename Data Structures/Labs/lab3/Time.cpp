#include <iostream>
#include "Time.h"

#include <cstdio>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include <iomanip>
#include <string>

Time:: Time(){
    hour = 0;
    minute = 0;
    second = 0;
}

Time:: Time(uintptr_t nHour, uintptr_t nMinute, uintptr_t nSecond){
    hour = nHour;
    minute = nMinute;
    second = nSecond;
}

uintptr_t Time:: getHour() const{
    return hour;
}

uintptr_t Time:: getMinute() const{
    return minute;
}

uintptr_t Time:: getSecond() const{
    return second;
}

void Time:: setHour(uintptr_t nHour){
    hour = nHour;
}

void Time:: setMinute (uintptr_t nMinute){
    minute = nMinute;
}

void Time:: setSecond (uintptr_t nSecond){
    second = nSecond;
}

void Time:: printAmPm(){
    uintptr_t curr_hour = getHour();
    std::string suffix = std::string(" am");
    if(curr_hour > 12){
        curr_hour = curr_hour - 12;
        suffix = std::string(" pm");
    }
    else if(curr_hour == 12){
        suffix = std::string(" pm");
    }
    else if(curr_hour == 0){
        curr_hour =  12;
    }
    if(getMinute() < 10){
        std::cout << curr_hour << ":0" << getMinute();
    }
    else{
        std::cout << curr_hour << ":" << getMinute();
    }
    if(getSecond() < 10){
        std::cout << ":0" << getSecond() << suffix << std::endl;
    }
    else{
        std::cout << ":" << getSecond() << suffix << std::endl;
    }

}

bool isEarlierThan (const Time& t1, const Time& t2){
    if(t1.getHour() == t2.getHour()){ //first level
        if(t1.getMinute() == t2.getMinute()){
            if(t1.getSecond() == t2.getSecond()){
                return false;
            }
            else if(t1.getSecond() > t2.getSecond()){
                return true;
            }
            else{
                return false;
            }
        }
        else if(t1.getMinute() > t2.getMinute()){
            return true;
        }
        else{
            return false;
        }
    }
    else if(t1.getHour() > t2.getHour()){
        return true;
    }
    else{
        return false;
    }
}


