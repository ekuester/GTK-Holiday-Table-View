/* 
 * File:   main.cpp
 * Author: kuestere
 *
 * Created on 16. Juni 2018, 14:53
 */

#include <gtkmm/application.h>
#include "holidaywindow.h"

using namespace std;

int main(int argc, char *argv[])
{

    auto app = Gtk::Application::create(argc, argv, "org.kuester.application");

    HolidayWindow window;

    //Shows the window and returns when it is closed.
    return app->run(window);
}
