/* 
 * File:   gregorian.cpp
 * Author: kuestere
 * 
 * Created on 29. Juni 2018, 21:53
 */

#include "gregorian.h"

// Default constructor: today's date and time as gregorian day number
Gregorian::Gregorian() {
    touched = false;
    // time as GMT
    std::time_t seconds = time(0);
    // 719468 is gregorian day for January 1, 1970 (unix epoch date)
    // to preserve time as decimal places, expression of all terms must be double
    gregorian = 719468.0 + seconds / 86400.0;
}

// simply construct from valid double number
Gregorian::Gregorian(double timestamp) {
    touched = false;
    gregorian = timestamp;
}

// construct from calendar date
Gregorian::Gregorian(int y, unsigned m, unsigned d) {
    yeardate = std::make_tuple(y, m, d);
    touched = true;
    //is_leap = y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
    y -= m <= 2;
    int era = (y >= 0 ? y : y - 399) / 400;
    unsigned yoe = static_cast<unsigned> (y - era * 400); // [0, 399]
    unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]
    unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0, 146096]
    gregorian = era * 146097.0 + doe;
}

// compute year, month, day from gregorian number
std::tuple<int, unsigned, unsigned> Gregorian::calendarDate(double gregorian) {
    touched = true;
    const int era = (gregorian >= 0 ? gregorian : gregorian - 146096.0) / 146097;
    const unsigned doe = static_cast<unsigned> (gregorian - era * 146097); // [0, 146096]
    const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
    int y = static_cast<int> (yoe) + era * 400;
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100); // [0, 365]
    const unsigned mp = (5 * doy + 2) / 153; // [0, 11]
    unsigned d = doy - (153 * mp + 2) / 5 + 1; // [1, 31]
    //            d = f(m) = (306 * m + 5) / 10
    unsigned m = mp + (mp < 10 ? 3 : -9); // [1, 12]
    y += (m <= 2);
    //is_leap = y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
    return std::make_tuple(y, m, d);
}

// compute hours, minutes, seconds from gregorian number
std::tuple<unsigned, unsigned, unsigned> Gregorian::timeDay(double gregorian) {
    // get fraction of day in seconds
    unsigned seconds = ( gregorian - floor(gregorian) ) * 86400.0;
    unsigned h = seconds / 3600;
    unsigned rest = seconds % 3600;
    unsigned m = rest / 60;
    unsigned s = rest % 60;
    return std::make_tuple(h, m, s);
}

// output date in human readable format
Glib::ustring Gregorian::formatDateLong() {
    // gregorian date with format "%w, %d. %m %Y"
    if (!touched) {
        yeardate = calendarDate(gregorian);
    }
    Glib::ustring weekdays[7] = {"Montag", "Dienstag", "Mittwoch",
        "Donnerstag", "Freitag", "Samstag", "Sonntag"};
    Glib::ustring months[12] = {"Januar", "Februar", "März", "April", "Mai",
        "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};
    std::stringstream dateStream;
    int y = std::get<0>(yeardate);
    int m = std::get<1>(yeardate) - 1;
    unsigned d = std::get<2>(yeardate);
    // January 1, 1 was a monday in proleptic gregorian calendar
    double ddays = gregorian - 306.0;
    weekday = static_cast<int> (ddays - floor(ddays / 7) * 7);
    dateStream << weekdays[weekday] << ", " << d << ". " << months[m] << " " << y;
    return dateStream.str();
}

// output date in human readable format
Glib::ustring Gregorian::formatDateShort() {
    // gregorian date with format "%Y%m%d"
    if (!touched) {
        yeardate = calendarDate(gregorian);
    }
    std::stringstream dateStream;
    int y = std::get<0>(yeardate);
    int m = std::get<1>(yeardate);
    unsigned d = std::get<2>(yeardate);
    dateStream << setfill('0') << y
        << setw(2) << m
        << setw(2) << d;
    return dateStream.str();
}

// output time in human readable format
Glib::ustring Gregorian::formatTimeShort() {
    daytime = timeDay(gregorian);
    std::stringstream timeStream;
    auto h = std::get<0>(daytime);
    auto m = std::get<1>(daytime);
    auto s = std::get<2>(daytime);
    timeStream << setfill('0') << h << ":"
        << setw(2) << m << ":"
        << setw(2) << s;
    return timeStream.str();
}