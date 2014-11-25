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


MainWindow::MainWindow(const string &file_name)
	: _record_store(Gtk::ListStore::create(_columns))
{
	this->set_default_size(640, 480);
	this->add(_vbox);

	_InitMenuBar();

	_vbox.pack_start(_query_entry, Gtk::PACK_SHRINK, 0);
	_query_entry.signal_changed().connect([&]() { this->on_query_changed(); });

	_vbox.pack_end(_scroll, Gtk::PACK_EXPAND_WIDGET, 0);

	_scroll.add(_record_list);
	_record_list.set_model(_record_store);
	_record_list.append_column(_("Record"), _columns._column_title);
	_record_list.set_headers_visible(false);

	this->show_all();

	_OpenDatabase(file_name);
}

void MainWindow::_InitMenuBar()
{
	auto menu_bar = Gtk::manage(new Gtk::MenuBar);
	_vbox.pack_start(*menu_bar, Gtk::PACK_SHRINK, 0);

	auto menu_item = Gtk::manage(new Gtk::MenuItem(_("_File"), true));
	menu_bar->append(*menu_item);
	auto menu_file = Gtk::manage(new Gtk::Menu);
	menu_item->set_submenu(*menu_file);

	menu_item = Gtk::manage(new Gtk::MenuItem(_("_Quit"), true));
	menu_item->signal_activate().connect([&]() { this->on_file_quit(); });
	menu_file->append(*menu_item);

	menu_item = Gtk::manage(new Gtk::MenuItem(_("_Help"), true));
	menu_bar->append(*menu_item);
	auto menu_help = Gtk::manage(new Gtk::Menu);
	menu_item->set_submenu(*menu_help);

	menu_item = Gtk::manage(new Gtk::MenuItem(_("_About"), true));
	menu_item->signal_activate().connect([&]() { this->on_help_about(); });
	menu_help->append(*menu_item);
}

void MainWindow::_OpenDatabase(const string &file_name)
{
	if (!boost::filesystem::exists(file_name))
		return;

	Gtk::Dialog dialog{ _("Open database"), *this, true };
	dialog.set_transient_for(*this);
	dialog.add_button(_("_Ok"), Gtk::RESPONSE_OK)->grab_default();
	dialog.add_button(_("_Cancel"), Gtk::RESPONSE_CANCEL);
	dialog.set_default_response(Gtk::RESPONSE_OK);
	dialog.set_response_sensitive(Gtk::RESPONSE_OK);

	Gtk::Label label1{ _("Enter password for") };
	dialog.get_content_area()->pack_start(label1, true, false, 0);

	Gtk::Label label2{ file_name };
	dialog.get_content_area()->pack_start(label2, true, false, 0);

	Gtk::Entry password_entry;
	password_entry.set_visibility(false);
	password_entry.set_activates_default();
	dialog.get_content_area()->pack_start(password_entry, true, false, 20);

	dialog.show_all_children();
	auto response = dialog.run();
	if (Gtk::RESPONSE_OK != response)
		return;

	try
	{
		Database::PtrT database{ new Database };
		StringX password{ password_entry.get_text().c_str() };
		database->Read(file_name, password);
		_file_name = file_name;
		_database = std::move(database);
		on_query_changed();
	}
	catch (const std::exception &e)
	{
		cerr << "Failed" << endl;
	}
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

void MainWindow::on_query_changed()
{
	auto q = _query_entry.get_text();
	auto match = _database->Find(q.c_str());

	_record_store->clear();

	for (size_t i = 0; i < match.size(); ++i)
	{
		const auto &title = match[i]->first;
		auto it = _record_store->append();
		(*it)[_columns._column_title] = title.c_str();
	}
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
