//
// CommandGui.cc
//
//     Created: 15.11.2014
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


#include "CommandGui.hh"
#include "i18n.h"
#include <gtk/gtk.h>
#include <memory>

namespace gPWS {

using namespace std;

Command::PtrT CommandGui::Create()
{
	return Command::PtrT{ new CommandGui() };
}

void CommandGui::Execute(const Params &params)
{
	auto ask_passwd = unique_ptr<GtkWidget, void(&)(GtkWidget*)>(
		gtk_dialog_new_with_buttons("gpwsafe",
		                            NULL,
		                            GTK_DIALOG_MODAL,
		                            _("_Cancel"),
		                            GTK_RESPONSE_REJECT,
		                            _("_Open"),
		                            GTK_RESPONSE_ACCEPT,
		                            NULL),
		gtk_widget_destroy);
	gtk_dialog_set_default_response(GTK_DIALOG(ask_passwd.get()),
	                                GTK_RESPONSE_ACCEPT);

	auto content_area = gtk_dialog_get_content_area(GTK_DIALOG(ask_passwd.get()));

	auto text = _("Enter password for ") + params.ExpandFileName();
	gtk_container_add(GTK_CONTAINER(content_area), gtk_label_new(text.c_str()));

	gtk_widget_show_all(ask_passwd.get());
	auto response = gtk_dialog_run(GTK_DIALOG(ask_passwd.get()));
	cout << response << endl;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
