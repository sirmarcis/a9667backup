    #ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include <cstdio>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include <iomanip>
#include <string>

class Time {
public:
    Time();
    Time(uintptr_t nHour, uintptr_t nMinute, uintptr_t nSecond);

    uintptr_t getHour() const;
    uintptr_t getMinute() const;
    uintptr_t getSecond() const;

    void setHour(uintptr_t nHour);
    void setMinute(uintptr_t nMinute);
    void setSecond(uintptr_t nSecond);

    void printAmPm();

private:
    uintptr_t hour;
    uintptr_t minute;
    uintptr_t second;
};

bool isEarlierThan(const Time& t1, const Time& t2);


#endif // TIME_H_INCLUDED
