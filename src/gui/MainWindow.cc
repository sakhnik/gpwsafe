//
// MainWindow.cc
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


#include "MainWindow.hh"
#include "../i18n.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace gPWS {

using namespace std;


MainWindow::MainWindow(string &&file_name)
	: _file_name(std::move(file_name))
	, _first_time{true}
{
	this->set_default_size(320, 240);

	auto vbox = Gtk::manage(new Gtk::VBox);
	this->add(*vbox);

	auto menu_bar = Gtk::manage(new Gtk::MenuBar);
	vbox->pack_start(*menu_bar, Gtk::PACK_SHRINK, 0);

	auto menu_item = Gtk::manage(new Gtk::MenuItem(_("_File"), true));
	menu_bar->append(*menu_item);
	auto menu_file = Gtk::manage(new Gtk::Menu);
	menu_item->set_submenu(*menu_file);

	menu_item = Gtk::manage(new Gtk::MenuItem(_("_Quit"), true));
	menu_item->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_file_quit));
	menu_file->append(*menu_item);

	menu_item = Gtk::manage(new Gtk::MenuItem(_("_Help"), true));
	menu_bar->append(*menu_item);
	auto menu_help = Gtk::manage(new Gtk::Menu);
	menu_item->set_submenu(*menu_help);

	menu_item = Gtk::manage(new Gtk::MenuItem(_("_About"), true));
	menu_item->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_help_about));
	menu_help->append(*menu_item);

	vbox->pack_start(_query_entry, false, false, 0);
	_query_entry.signal_focus_in_event()
		.connect(sigc::mem_fun(*this, &MainWindow::on_query_focus_in));

	this->show_all();
}

void MainWindow::on_file_quit()
{
	Gtk::Main::quit();
}

void MainWindow::on_help_about()
{
	Gtk::AboutDialog dialog;
	dialog.set_transient_for(*this);
	dialog.set_attached_to(*this);
	dialog.set_program_name(PACKAGE);
	dialog.set_logo_icon_name("dialog-password");
	dialog.set_version(VERSION);
	dialog.set_website("http://github.com/sakhnik/gpwsafe");
	dialog.set_license_type(Gtk::LICENSE_GPL_3_0);
	dialog.set_comments(_("Password database compatible with the original "
	                      "Counterpane's Password Safe."));

	dialog.set_authors({"Anatolii Sakhnik <sakhnik@gmail.com>"});

	dialog.run();
}

bool MainWindow::on_query_focus_in(GdkEventFocus *event)
{
	if (!_first_time)
		return false;
	_first_time = false;

	if (!boost::filesystem::exists(_file_name))
		return false;

	Gtk::Dialog dialog{ _("Open database"), *this, true };
	dialog.set_transient_for(*this);
	dialog.add_button(_("_Ok"), Gtk::RESPONSE_OK)->grab_default();
	dialog.add_button(_("_Cancel"), Gtk::RESPONSE_CANCEL);
	dialog.set_default_response(Gtk::RESPONSE_OK);
	dialog.set_response_sensitive(Gtk::RESPONSE_OK);

	auto label = Gtk::manage(new Gtk::Label(_("Enter password for")));
	dialog.get_content_area()->pack_start(*label, true, false, 0);

	label = Gtk::manage(new Gtk::Label(_file_name));
	dialog.get_content_area()->pack_start(*label, true, false, 0);

	auto password_entry = Gtk::manage(new Gtk::Entry());
	password_entry->set_visibility(false);
	password_entry->set_activates_default();
	dialog.get_content_area()->pack_start(*password_entry, true, false, 20);

	dialog.show_all_children();
	auto response = dialog.run();
	if (Gtk::RESPONSE_OK != response)
		return false;

	try
	{
		Database::PtrT database{ new Database };
		StringX password{ password_entry->get_text().c_str() };
		database->Read(_file_name, password);
		_database = std::move(database);
	}
	catch (const std::exception &e)
	{
		cerr << "Failed" << endl;
	}
	return false;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
