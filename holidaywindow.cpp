//
//  File:   holidaywindow.cpp
//  Holiday Table View
//
//  Created by Erich Kuester on July 14, 2014
//  Renewed for C++ on June 16, 2018
//  Copyright (c) 2014-2018  Erich Kuester. All rights reserved.
//

#include "holidaywindow.h"

using namespace std;

HolidayWindow::HolidayWindow()
: m_VBox(Gtk::ORIENTATION_VERTICAL, 5),
m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL),
m_Label(),
m_Text(),
m_ScrolledWindow(),
m_HBoxBottom(Gtk::ORIENTATION_HORIZONTAL),
m_SaveButtonBox(Gtk::ORIENTATION_HORIZONTAL),
m_ButtonSave("Speichern...", true),
m_ButtonBox(Gtk::ORIENTATION_HORIZONTAL),
m_ButtonQuit("Beenden", true) {
    set_title("Gtk: German Holidays");
    set_border_width(5);
    set_default_size(640, 480);

    add(m_VBox);
    // fill vertical box
    m_Label.set_markup("<b>Jahreszahl eingeben:</b>");
    m_Label.set_vexpand(false);
    m_Label.set_hexpand(false);
    m_Label.set_halign(Gtk::ALIGN_START);
    m_Text.set_vexpand(false);
    m_Text.set_hexpand(false);
    m_Text.set_halign(Gtk::ALIGN_END);
    m_Text.set_text("2018");
    // Label, then TextEntry left to right in horizontal row
    m_HBoxTop.pack_start(m_Label, Gtk::PACK_SHRINK, 16);
    m_HBoxTop.pack_start(m_Text, Gtk::PACK_SHRINK);
    m_Text.signal_activate().connect(sigc::mem_fun(*this, &HolidayWindow::on_activated));
    //m_Text.signal_changed().connect(sigc::mem_fun(*this, &HolidayWindow::on_entry_insert));
    m_VBox.pack_start(m_HBoxTop, Gtk::PACK_SHRINK);

    // Add the TreeView, inside a ScrolledWindow, with the button underneath:
    m_ScrolledWindow.add(m_TreeView);
    // Only show the scrollbars when they are necessary:
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_VBox.pack_start(m_ScrolledWindow);
    m_VBox.set_border_width(8);
    // SaveButtonBox to the left end of vertical Box
    m_HBoxBottom.pack_start(m_SaveButtonBox, Gtk::PACK_SHRINK);
    m_SaveButtonBox.pack_start(m_ButtonSave, Gtk::PACK_SHRINK);
    m_SaveButtonBox.set_border_width(2);
    m_SaveButtonBox.set_layout(Gtk::BUTTONBOX_START);
    gpointer yearPtr = GUINT_TO_POINTER(yearNumber);
    m_ButtonSave.signal_clicked().connect(sigc::bind<guint*>(sigc::mem_fun(*this,
        &HolidayWindow::save_holidays_clicked), &yearNumber));
    // ButtonBox to the right end of vertical box
    m_HBoxBottom.pack_end(m_ButtonBox, Gtk::PACK_SHRINK);
    m_HBoxBottom.set_border_width(8);
    m_ButtonBox.pack_start(m_ButtonQuit, Gtk::PACK_SHRINK);
    m_ButtonBox.set_border_width(2);
    m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
    m_ButtonQuit.signal_clicked().connect(sigc::mem_fun(*this,
            &HolidayWindow::on_button_quit));
    m_VBox.pack_start(m_HBoxBottom, Gtk::PACK_SHRINK);

    // Create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_TreeView.set_model(m_refTreeModel);

    //Add the TreeView's view columns:
    //This number will be shown with the default numeric formatting.
    m_TreeView.append_column("ID", m_Columns.m_col_id);
    m_TreeView.append_column("Name des Feiertags", m_Columns.m_name_string);
    m_TreeView.append_column("Datum des Feiertags", m_Columns.m_date_string);

    //Make all the columns reorderable and resizable:
    //This is not necessary, but it's nice to show the feature.
    //You can use TreeView::set_column_drag_function() to more
    //finely control column drag and drop.
    for ( auto* column : m_TreeView.get_columns() ) {
        column->set_reorderable();
        column->set_resizable(true);
        //column->set_alignment(Gtk::ALIGN_FILL);
    }

    show_all_children();
}

HolidayWindow::~HolidayWindow() {
    Gregorian logout = Gregorian();
    cout << "closing Holiday Database Window: " << logout.formatTimeShort() << endl;
}

// read yearnumber from entry
guint HolidayWindow::get_yearnumber() {
    guint year = 0;
    // This code converts from string to number safely.
    stringstream numberStream(m_Text.get_text());
    if (numberStream >> year)
        return year;
    cout << "Invalid number, please try again" << endl;
    return 0;
}

// handles enter pressed
void HolidayWindow::on_activated() {
    int year = get_yearnumber();
    if (year != yearNumber) {
        if (holidays.size() > 0) {
            holidays.clear();
            m_refTreeModel->clear();
        }
    }
    yearNumber = year;
    if ((yearNumber < 1583) || (yearNumber > 2399)) {
        Gtk::MessageDialog dialog(*this,
                "Error: Check input",
                false,
                Gtk::MESSAGE_ERROR,
                Gtk::BUTTONS_OK, // Gtk::BUTTONS_OK, Gtk::BUTTONS_NONE,
                true);
        dialog.set_secondary_text(
                "Year should be between 1583 and 2399");
        //Gtk::ButtonBox* action_area = dialog.get_action_area();
        int result = dialog.run();
        yearNumber = 1970;
    }
    Glib::ustring fixNames[12] = {
        "Neujahr",
        "Heilige Drei Könige",
        "Valentinstag",
        "Walpurgisnacht",
        "Tag der Arbeit",
        "Mariä Himmelfahrt",
        "Allerheiligen",
        "Allerseelen",
        "Heiligabend",
        "1. Weihnachtstag",
        "2. Weihnachtstag",
        "Sylvester"
    };
    std::vector<std::pair<unsigned, unsigned> > fixDates = {
        { 1,  1},
        { 6,  1},
        {14,  2},
        {30,  4},
        { 1,  5},
        {15,  8},
        { 1, 11},
        { 2, 11},
        {24, 12},
        {25, 12},
        {26, 12},
        {31, 12}
    };
    int i = 0;
    for ( auto fixDate : fixDates ) {
        unsigned d = fixDate.first;
        unsigned m = fixDate.second;
        Gregorian fixDay = Gregorian(yearNumber, m, d);
        Holiday *fixHoliday = new Holiday(fixDay, fixNames[i]);
        holidays.push_back(fixHoliday);
        i++;
    }
    // easter is first Sunday after the ecclesiastical full moon
    // occurring on or soonest after March 21
    Holiday* easter = new Holiday(0, yearNumber, "Ostern");
    holidays.push_back(easter);
    std::vector<std::pair<Glib::ustring, int> > easterDates = {
        {"Altweiberfastnacht", -52},
        {"Rosenmontag", -48},
        {"Fastnacht", -47},
        {"Aschermittwoch", -46},
        {"Karfreitag", -2},
        {"Ostermontag", 1},
        {"Christi Himmelfahrt", 39},
        {"Pfingsten", 49},
        {"Pfingstmontag", 50},
        {"Fronleichnam", 60}
    };
    for ( auto easterDate : easterDates ) {
        Gregorian easterRelative = Gregorian(easter->numberOfDay + easterDate.second);
        Holiday* easterHoliday = new Holiday(easterRelative, easterDate.first);
        holidays.push_back(easterHoliday);
    }
    // since 1923 mother's day is on second sunday in may
    if (yearNumber > 1923) {
        Holiday* mothersDay = new Holiday(1, yearNumber, "Muttertag");
        holidays.push_back(mothersDay);
    }
    // since 1953 there is a unity day in Germany
    if (yearNumber > 1953) {
        // Tag der d(D)eutschen Einheit
        Holiday* unityDay;
        if (yearNumber < 1991) {
            unityDay = new Holiday(yearNumber, 6, 17, "Tag der deutschen Einheit");
        }
        else {
            unityDay = new Holiday(yearNumber, 10, 3, "Tag der Deutschen Einheit");
        }
        holidays.push_back(unityDay);
    }
    // deads' sunday is last sunday in november before first sunday in advent
    Holiday* deadsSunday = new Holiday(2, yearNumber, "Totensonntag");
    holidays.push_back(deadsSunday);
    std::vector<std::pair<Glib::ustring, int> > novemberDates = {
        {"Volkstrauertag", -7},
        {"Buß- und Bettag", -4},
        {"1. Advent", 7}
    };
    for ( auto novemberDate : novemberDates ) {
        Gregorian novemberRelative = Gregorian(deadsSunday->numberOfDay + novemberDate.second);
        Holiday* novemberHoliday = new Holiday(novemberRelative, novemberDate.first);
        holidays.push_back(novemberHoliday);
    }
    std::sort(holidays.begin(), holidays.end(), [](const Holiday* h1, const Holiday* h2){
        return h1->numberOfDay < h2->numberOfDay;});
    // Fill the TreeView's model
    // Add table row for every holiday object
    i = 1;
    for ( auto* holiday : holidays) {
        Gtk::TreeModel::Row row = *(m_refTreeModel->append());
        row[m_Columns.m_col_id] = i++;
        row[m_Columns.m_name_string] = holiday->nameOfDay;
        row[m_Columns.m_date_string] = holiday->numberOfDay.formatDateLong();
    }
}

void HolidayWindow::on_button_quit() {
    hide();
}

void HolidayWindow::save_holidays_clicked(guint* year) {
    Glib::ustring info;
    if (*year > 0) {
        const char* dateFormat = "%Y%m%dT%H%M%SZ";
        // prepare timestamp
        time_t time_now = time(0);
        stringstream timeStream;
        timeStream << put_time(localtime(&time_now), dateFormat);
        string timeStamp = timeStream.str();
        stringstream filename;
        filename << "Holidays-" << *year << ".ics";
        // cout << filename.str() << endl;
        ofstream calendarFile;
        calendarFile.open(filename.str());
        calendarFile << "BEGIN:VCALENDAR\n";
        calendarFile << "METHOD:PUBLISH\n";
        calendarFile << "VERSION:2.0\n";
        calendarFile << "X-WR-CALNAME:Feiertage\n";
        calendarFile << "PRODID:-//Apple Inc.//iCal 5.0.0//DE\n";
        int i = 1;
        // loop through holidays in year
        for ( auto* holiday : holidays) {
            calendarFile << "BEGIN:VEVENT\n";
            calendarFile << "CREATED: " << timeStamp << "\n";
            // add-on "-kuester-" can be changed at will
            calendarFile << "UID:" << timeStamp << "@kuester-" << i << "\n";
            calendarFile << "SUMMARY: " << holiday->nameOfDay << "\n";
            calendarFile << "DESCRIPTION:Feiertage\n";
            calendarFile << "X-MOZILLA-ALARM-DEFAULT-LENGTH:0\n";
            calendarFile << "X-MOZILLA-RECUR-DEFAULT-UNITS:0\n";
            calendarFile << "RRULE:0\n";
            calendarFile << "TRANSP:TRANSPARENT\n";
            calendarFile << "DTSTART;VALUE=DATE:" 
                    << holiday->numberOfDay.formatDateShort() << "\n";
            Gregorian nextNumber = holiday->numberOfDay + 1;
            calendarFile << "DTEND;VALUE=DATE:"
                    << nextNumber.formatDateShort() << "\n";
            calendarFile << "DTSTAMP:" << timeStamp << "\n";
            calendarFile << "LAST-MODIFIED:" << timeStamp << "\n";
            calendarFile << "END:VEVENT\n";
            i++;
        }
        calendarFile << "END:VCALENDAR\n";
        calendarFile.close();
        stringstream infoStream;
        infoStream << holidays.size() << " holidays stored for year " << *year;
        info = infoStream.str();
        timeStream.clear();
    } else {
        info = "No data for calendar available";
    }
    Gtk::MessageDialog dialog(*this,
        "Information",
        false,
        Gtk::MESSAGE_INFO,
        Gtk::BUTTONS_OK, // BUTTONS_OK, BUTTONS_OK_CANCEL, BUTTONS_NONE,
        true);
    dialog.set_secondary_text(info);
    int result = dialog.run();
}