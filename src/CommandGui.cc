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
#include "gui/MainWindow.hh"
#include <gtkmm.h>


namespace gPWS {

using namespace std;

Command::PtrT CommandGui::Create()
{
	return Command::PtrT{ new CommandGui() };
}

//template <typename T, typename D>
//unique_ptr<T, D> Manage(T *t, D &&d)
//{
//	return unique_ptr<T, D>{ t, forward<D>(d) };
//}
//
//gboolean on_start(gpointer data)
//{
//	Context *context = reinterpret_cast<Context *>(data);
//
//	if (!boost::filesystem::exists(context->file_name))
//		return FALSE;
//
//	Glib::RefPtr<Gtk::Dialog> dialog{ new Gtk::Dialog("Enter password", true) };
//	dialog->add_button(_("_Ok"), Gtk::RESPONSE_OK);
//	dialog->add_button(_("_Cancel"), Gtk::RESPONSE_CANCEL);
//
//	dialog->show_all();
//	auto response = dialog->run();
//	//auto password_entry = gtk_entry_new();
//	//gtk_box_pack_end(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog.get()))),
//	//                 password_entry,
//	//                 FALSE,
//	//                 FALSE,
//	//                 0);
//
//	cout << response << endl;
//	return FALSE;
//}

void CommandGui::Execute(const Params &params)
{
	Gtk::Main app("org.sakhnik.gpwsafe");

	auto builder = Gtk::Builder::create_from_file("src/gui.glade");

	MainWindow *window = nullptr;
	builder->get_widget_derived("main_window", window);
	app.run(*window);
	delete window;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
