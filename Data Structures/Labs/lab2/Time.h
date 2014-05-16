    #ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED


class Time {
public:
    Time();
    Time(int nHour, int nMinute, int nSecond);

    int getHour() const;
    int getMinute() const;
    int getSecond() const;

    void setHour(int nHour);
    void setMinute(int nMinute);
    void setSecond(int nSecond);

    void printAmPm();

private:
    int hour;
    int minute;
    int second;
};

bool isEarlierThan(const Time& t1, const Time& t2);


#endif // TIME_H_INCLUDED
