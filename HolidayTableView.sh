g++ `pkg-config gtkmm-3.0 --cflags --libs` -c main.cpp gregorian.cpp holidaywindow.cpp holiday.cpp
g++ main.o gregorian.o holiday.o holidaywindow.o -o HolidayView `pkg-config gtkmm-3.0 --cflags --libs`
./HolidayView

