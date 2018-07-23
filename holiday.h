/* 
 * File:   Holiday.h
 * Author: kuestere
 *
 * Created on 29. Juni 2018, 22:18
 */

#ifndef HOLIDAY_H
#define HOLIDAY_H

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <gtkmm-3.0/gtkmm.h>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <sstream>
#include <string>
#include "gregorian.h"

using namespace std;

class Holiday : public Gregorian {
// return value of holiday class is double() because Gregorian() has double value
public:
    // default constructor from gregorian number
    Holiday(Gregorian& dayNumber, Glib::ustring dayName);

    // constructor with date
    Holiday(int year, unsigned month, unsigned day, Glib::ustring dayName);

    // constructor for floating holidays
    Holiday(int category, int year, Glib::ustring dayName);

    Holiday(const Holiday& orig);

    virtual ~Holiday();

    Gregorian numberOfDay;
    Glib::ustring nameOfDay = "";

private:

};

#endif /* HOLIDAY_H */

