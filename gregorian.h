/* 
 * File:   gregorian.h
 * Author: kuestere
 *
 * Created on 29. Juni 2018, 21:53
 */

#ifndef GREGORIAN_H
#define GREGORIAN_H

#include <ctime>
#include <gtkmm-3.0/gtkmm.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <tuple>

using namespace std;

// gregorian day (days since March 1, 800 ante Chr.)
// implements a proleptic Gregorian calendar
// see <http://howardhinnant.github.io/date_algorithms.html>
// first day 01/01/0001 which means 306.0 of proleptic gregorian calendar is MONDAY
class Gregorian {
public:
    // Default constructor: today's date and time as gregorian day number
    Gregorian();

    // simply construct from valid gregorian number
    Gregorian(double timestamp);

    // construct from calendar date
    Gregorian(int y, unsigned m, unsigned d);

    Gregorian(const Gregorian& orig);
    //virtual ~Gregorian();

    operator double() const {
        return this->gregorian;
    }

    // assign two Gregorians
    Gregorian& operator= (const Gregorian &other)
    {
        // self-assignment guard
        if (this == &other)
            return *this;
        // do the copy
        gregorian = other.gregorian;
        yeardate = other.yeardate;
        // return the existing object so we can chain this operator
        return *this;
    }

    // add constant to gregorian number
    Gregorian operator+ (const int days) const {
        return Gregorian(this->gregorian + days);
    }

    // tuple of year, month, day
    std::tuple<int, unsigned, unsigned> yeardate;
    // day of week 0 ... 6 monday ... sunday
    int weekday = -1;
    // tuple of hours, minutes, seconds
    std::tuple<unsigned, unsigned, unsigned> daytime;

    // compute calendar date from gregorian number
    std::tuple<int, unsigned, unsigned> calendarDate(double gregorian);
    // compute time of day from gregorian number
    std::tuple<unsigned, unsigned, unsigned> timeDay(double gregorian);
    // output date date portion of year as human readable string, long version
    Glib::ustring formatDateLong();
    // output date portion of gregorian number as human readable string, short version
    Glib::ustring formatDateShort();
    // output time portion of gregorian number as human readable string, short version
    Glib::ustring formatTimeShort();

private:
    // is calendar date already initialized
    bool touched = false;
    double gregorian;
};

#endif /* GREGORIAN_H */

