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
#include <gtk/gtk.h>
#include <memory>


namespace gPWS {

using namespace std;

struct Context
{
	unique_ptr<GtkWidget, void(&)(GtkWidget *)> main_window;
	unique_ptr<GtkWidget, void(&)(GtkWidget *)> about_dialog;
};

extern "C" void help_about(GtkMenuItem *, gpointer data)
{
	Context *context = reinterpret_cast<Context *>(data);
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(context->about_dialog.get()),
	                             VERSION);
	gtk_widget_show_all(context->about_dialog.get());
}

Command::PtrT CommandGui::Create()
{
	return Command::PtrT{ new CommandGui() };
}

template <typename T, typename D>
unique_ptr<T, D> Manage(T *t, D &&d)
{
	return unique_ptr<T, D>{ t, forward<D>(d) };
}

void CommandGui::Execute(const Params &params)
{
	auto builder = Manage(gtk_builder_new_from_file("src/gui.glade"),
	                      [](GtkBuilder *b) { g_object_unref(G_OBJECT(b)); });

	Context context = {
		.main_window = {GTK_WIDGET(gtk_builder_get_object(builder.get(), "main_window")),
		                gtk_widget_destroy},
		.about_dialog = {GTK_WIDGET(gtk_builder_get_object(builder.get(), "aboutdialog")),
		                 gtk_widget_destroy}
	};

	gtk_builder_connect_signals(builder.get(), &context);

	gtk_widget_show_all(context.main_window.get());
	gtk_main();
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
