/* 
 * File:   holiday.cpp
 * Author: kuestere
 * 
 * Created on 29. Juni 2018, 22:18
 */

#include "holiday.h"

Holiday::Holiday(Gregorian& dayNumber, Glib::ustring dayName) {
    numberOfDay = dayNumber;
    nameOfDay = dayName;
}

Holiday::Holiday(int year, unsigned month, unsigned day, Glib::ustring dayName) {
    nameOfDay = dayName;
    numberOfDay = Gregorian(year, month, day);
}

// constructor for floating holidays
Holiday::Holiday(int category, int year, Glib::ustring dayName) {

    enum {
        easter, mothers, memorial
    };
    double ddays;
    nameOfDay = dayName;
    switch (category) {
        case easter:
        {
            /*
             * Ostern faellt auf den Sonntag nach dem ersten Vollmond im Fruehling
             * Carl Friedrich Gauß (1777 - 1855) hat aus den langen und schwer verstaendlichen
             * Texten von Christophorus Clavius eine praktische Rechenvorschrift abgeleitet.
             * Diese wird in einer von Dr. Heiner Lichtenberg, Bonn, modifizierten Form angegeben.
             * H. Lichtenberg, Zur Interpretation der Gaussschen Osterformel und ihrer Ausnahmeregeln,
             * Historia Mathematica 24, 441 - 444 (1997).
             * siehe insbesondere
             * <https://de.wikipedia.org/wiki/Gau%C3%9Fsche_Osterformel> sowie
             * <http://www.ptb.de/cms/ptb/fachabteilungen/abt4/fb-44/ag-441/darstellung-der-gesetzlichen-zeit/wann-ist-ostern.html>
             *  1. secular number:                               K = X div 100
             *  2. secular moon switching:                       M = 15 + (3K + 3) div 4 − (8K + 13) div 25
             *  3. secular sun switching:                        S = 2 − (3K + 3) div 4
             *  4. moon parameter (Meton-cycle):                 A = X mod 19
             *  5. seed for first spring full moon:              D = (19A + M) mod 30
             *  6. calendar adjustment:                          R = D div 29 + (D div 28 − D div 29) (A div 11)
             *  7. easter limit:                                OG = 21 + D − R
             *  8. first sunday in month March:                 SZ = 7 − (X + X div 4 + S) mod 7
             *  9. distance of easter sunday from
             *     easter limit (easter distance in days):      OE = 7 − (OG − SZ) mod 7
             * 10. date of easter sunday as date in March       OS = OG + OE
             * OG March date of easter full moon. This date corresponds with day 14
             * of first month in lunar calendar, named Nisanu.
             * SZ is date of first sunday in March.
             * OS = OG + OE is date of easter sunday represented as date in month March
             * March 32 becomes April 1,and so on
             */
            int k = year / 100;
            int m = 15 + (3 * k + 3) / 4 - (8 * k + 13) / 25;
            int s = 2 - (3 * k + 3) / 4;
            int a = year % 19;
            int d = (19 * a + m) % 30;
            int r = d / 29 + (d / 28 - d / 29) * (a / 11);
            int og = 21 + d - r;
            int sz = 7 - (year + year / 4 + s) % 7;
            int oe = 7 - (og - sz) % 7;
            unsigned os = og + oe;
            unsigned om = 3;
            if (os > 31) {
                om += 1;
                os -= 31;
            }
            numberOfDay = Gregorian(year, om, os);
            break;
        }
        case mothers:
        {
            double number = Gregorian(year, 5, 15);
            ddays = number - 306.0;
            double interim = (ddays - floor(ddays / 7) * 7) + 1;
            numberOfDay = Gregorian(number - interim);
            break;
        }
        case memorial:
        {
            double number = Gregorian(year, 11, 27);
            ddays = number - 306.0;
            double interim = (ddays - floor(ddays / 7) * 7) + 1;
            numberOfDay = Gregorian(number - interim);
            break;
        }
        default:
            break;
    }
};

Holiday::~Holiday() {
}
