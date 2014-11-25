//
// MainWindow.hh
//
//     Created: 17.11.2014
//      Author: A. Sakhnik
//
// This file is part of gpwsafe.
//
// Gpwsafe is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Gpwsafe is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with gpwsafe.  If not, see <http://www.gnu.org/licenses/>


#pragma once

#include "../Database.hh"
#include <string>
#include <gtkmm.h>

namespace gPWS {

class MainWindow
	: public Gtk::Window
{
public:
	MainWindow(const std::string &file_name);

private:
	std::string _file_name;
	Database::PtrT _database;
	Gtk::VBox _vbox;
	Gtk::SearchEntry _query_entry;

	struct _Columns
		: Gtk::TreeModelColumnRecord
	{
		_Columns()
		{
			add(_column_title);
		}

		Gtk::TreeModelColumn<Glib::ustring> _column_title;
	} _columns;

	Glib::RefPtr<Gtk::ListStore> _record_store;
	Gtk::TreeView _record_list;
	Gtk::ScrolledWindow _scroll;

	void _OpenDatabase(const std::string &file_name);
	void _InitMenuBar();

	void on_help_about();
	void on_file_quit();
	void on_query_changed();
};

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
