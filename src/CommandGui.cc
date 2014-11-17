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
#include <boost/filesystem.hpp>


namespace gPWS {

using namespace std;

struct Context
{
	string file_name;

	unique_ptr<GtkWidget, void(&)(GtkWidget *)>
		main_window,
		about_dialog;
};

extern "C" void on_help_about(GtkMenuItem *, gpointer data)
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

gboolean on_start(gpointer data)
{
	Context *context = reinterpret_cast<Context *>(data);

	if (!boost::filesystem::exists(context->file_name))
		return FALSE;

	auto dialog = Manage(
		gtk_dialog_new_with_buttons("Enter password",
		                            GTK_WINDOW(context->main_window.get()),
		                            GTK_DIALOG_MODAL,
		                            _("_OK"),
		                            GTK_RESPONSE_OK,
		                            _("_Cancel"),
		                            GTK_RESPONSE_CANCEL,
		                            nullptr),
		gtk_widget_destroy
		);

	auto password_entry = gtk_entry_new();
	gtk_box_pack_end(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog.get()))),
	                 password_entry,
	                 FALSE,
	                 FALSE,
	                 0);

	gtk_widget_show_all(dialog.get());
	auto response = gtk_dialog_run(GTK_DIALOG(dialog.get()));
	cout << response << endl;
	return FALSE;
}

void CommandGui::Execute(const Params &params)
{
	auto builder = Manage(gtk_builder_new_from_file("src/gui.glade"),
	                      [](GtkBuilder *b) { g_object_unref(G_OBJECT(b)); });

	auto get_widget = [&](const char *id) -> decltype(Context::main_window)
	{
		return {GTK_WIDGET(gtk_builder_get_object(builder.get(), id)),
		        gtk_widget_destroy};
	};

	Context context = {
		.file_name = params.ExpandFileName(),
		.main_window = get_widget("main_window"),
		.about_dialog = get_widget("aboutdialog"),
	};

	gtk_builder_connect_signals(builder.get(), &context);

	gtk_widget_show_all(context.main_window.get());
	g_timeout_add(0, on_start, &context);
	gtk_main();
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
