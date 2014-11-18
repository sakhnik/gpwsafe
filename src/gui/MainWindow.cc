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

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
