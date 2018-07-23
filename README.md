# GTK-Holiday-Table-View
Calendar for German holidays with creating of ICS files ( C++ / Linux / Gnome / GTK / gtkmm ) 

See all German holidays of a given year and put the dates into your personal calendar

The program was written in C++ with use of gtkmm wrapper for Gnome GTK environment of Linux.

The development environment in the moment is Netbeans IDE 8.2 under Fedora 28 Cinnamon. Alternatively you can easily compile and run the program in Terminal with the added shell script:

$ sh ./HolidayTableView.sh

The program follows the design of an earlier (unpublished) program Holiday-Database written in Swift. Since I switched from macOS to Fedora Linux (for what reasons ever) I had to change my programming language. For "simplicity" I choose C++ and to my pleasure found its "way to think" very similar to Swift and Objective-C. In some aspects I found C++ in combination with GTK easier compared to Swift ...

I wrote this program to become familiar with the C++ language, especially the GTK-API under the gtkmm wrapper and to get a feeling how to display liststore treeviews on the screen. Take it as example for handling of windows, calendars and dates. Since modern Smartphones have all the functions built-in to remember the national holidays, my program nowadays is relatively useless ... but ... you can learn a lot about calendars (julian, gregorian, ecclesiastic) plus calculation of the easter date with all of its arduousness and complexity.

Usage: You will find the program mostly self explaining. On input of a year number greater than 1583 the dates for German holidays are calculated. By clicking on "Speichern..." you have the possibilty to save the dates of the holidays in a calendar file with the extension .ics, which will accepted by calendar programs on PC or mobile phones. Thereby you are able to view the holidays on your personal calendar. Quit the program with "Beenden".

There is an in my eyes excellent article from Howart Hinnant
<http://howardhinnant.github.io/date_algorithms.html>
regarding calculating and converting gregorian dates. I adopted some of his routines in an own class Gregorian which assigns every day in the gregorian calendar an unique number. In my implementation this number is defined as double. If this number has decimal places these are interpreted as fraction of the day which means if you multiply this fraction with 86400 (24x60x60) you will get the seconds elepsed after midnight. From that it is easy to calculate the time in hours, minutes and seconds ...

Disclaimer: Use the program for what purpose you like, but hold in mind, that I will not be responsible for any harm it will cause to your hard- or software. It was your decision to use this piece of software.
