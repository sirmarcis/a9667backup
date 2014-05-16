//  This program converts a day and month within a given year to the
//  Julian day.

#include <iostream>


//  This is a constant array of integer values.  The array entry
//  associated with index i is the number of days in the ith month of
//  the year.  Since C++ array indices start at 0 and month counting
//  starts at 1, there are 13 locations in the array, and the
//  first location is given a default value of 0.
const int DaysInMonth[13] = { 0, 31, 28, 31, 30, 31, 30,
				31, 31, 30, 31, 30, 31 };


//  Function returns true if the given year is a leap year and
//  returns false otherwise.
bool
is_leap_year( int year )
{
  return year % 4 == 0 && ( year % 100 != 0 || year % 400 == 0 );
}


//  Calculate and return the Julian day associated with the given
//  month and day of the year.
int
julian_day( int month, int day, int year )
{
  int jday = 0;
  for ( unsigned int m=1; m<month; ++m )
    {
      jday += DaysInMonth[m];
      if ( m == 2 && is_leap_year(year)) ++jday;  // February 29th
    }
  jday += day;
  return jday;
}

int
main()
{
  std::cout << "Please enter three integers (a month, a day and a year): ";
  int month, day, year, jday;
  std::cin >> month >> day >> year;
  int monthComboArr[10], dayComboArr[11], jdayArr[11];
  for (int cnt = 0; cnt < 10; ++cnt){
	  if(month > 12){
		  std::cout << "ERROR: Bad month " << month << std::endl;
		  return 0;}
	  else if (day > 31){
		  std::cout << "ERROR: bad day " << day << std::endl;
		  return 0;
	  }
	  monthComboArr[cnt] = month; dayComboArr[cnt] = day;
	  std::cout << "please input grouping " << cnt << " according to previous instructions: ";
	  std::cin >> month >> day >> year;
	  std::cout << std::endl;
  	  }
  for( int cnt = 0; cnt < 10; ++cnt){
	  month = monthComboArr[cnt]; day = dayComboArr[cnt];
	  jday = julian_day( month, day, year );
	  jdayArr[cnt] = jday;
  }
  for (int cnt = 0; cnt < 10; ++cnt)
	  std::cout << "Julian day " << cnt << " is " << jdayArr[cnt] <<std::endl;
  return 0;
}





