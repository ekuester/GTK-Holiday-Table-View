//
//  File:   holidaywindow.h
//  Holiday Table View
//
//  Created by Erich Kuester on July 14, 2014
//  Renewed for C++ on June 16, 2018
//  Copyright (c) 2014-2018  Erich Kuester. All rights reserved.
//

#ifndef HOLIDAYWINDOW_H
#define HOLIDAYWINDOW_H

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <gtkmm.h>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <sstream>
#include <string>
#include "gregorian.h"
#include "holiday.h"

using namespace std;

class Gregorian;
class Holiday;

class HolidayWindow : public Gtk::Window {
public:
    HolidayWindow();
    virtual ~HolidayWindow();

    guint yearNumber = 0;
    std::vector<Holiday*> holidays;

protected:
    guint get_yearnumber();

    //Signal handlers
    void on_activated();
    void on_button_quit();
    void save_holidays_clicked(guint* year);

    //Three model columns:
    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:

        ModelColumns() {
            add(m_col_id);
            add(m_name_string);
            add(m_date_string);
        }

        Gtk::TreeModelColumn<guint> m_col_id;
        Gtk::TreeModelColumn<Glib::ustring> m_name_string;
        Gtk::TreeModelColumn<Glib::ustring> m_date_string;
    };
    ModelColumns m_Columns;

    // Child widgets
    Gtk::Box m_HBoxTop;
    Gtk::Label m_Label;
    Gtk::Entry m_Text;

    Gtk::Box m_VBox;
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::TreeView m_TreeView;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
    Gtk::Box m_HBoxBottom;
    Gtk::ButtonBox m_SaveButtonBox;
    Gtk::Button m_ButtonSave;
    Gtk::ButtonBox m_ButtonBox;
    Gtk::Button m_ButtonQuit;
};

#endif /* HOLIDAYWINDOW_H */
