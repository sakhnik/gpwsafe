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


MainWindow::MainWindow(BaseObjectType *cobject,
                       const Glib::RefPtr<Gtk::Builder> &builder)
	: Gtk::Window(cobject)
	, _builder{builder}
{
	auto action_group = Gio::SimpleActionGroup::create();
	action_group->add_action("help.about",
	                         sigc::mem_fun(*this, &MainWindow::on_help_about));
	insert_action_group("main", action_group);
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

void MainWindow::OpenRecent(const string &file_name)
{
	if (!boost::filesystem::exists(file_name))
		return;

	Glib::RefPtr<Gtk::Dialog> dialog{ new Gtk::Dialog("Enter password", true) };
	dialog->set_transient_for(*this);
	dialog->set_parent(*this);
	dialog->add_button(_("_Ok"), Gtk::RESPONSE_OK);
	dialog->add_button(_("_Cancel"), Gtk::RESPONSE_CANCEL);
	dialog->set_default_response(Gtk::RESPONSE_OK);

	auto password_entry = new Gtk::Entry();
	password_entry->set_visibility(false);
	dialog->get_content_area()->pack_end(*password_entry, true, false, 20);

	dialog->show_all();

	if (Gtk::RESPONSE_OK != dialog->run())
		return;
	dialog->hide();

	cout << password_entry->get_text() << endl;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
