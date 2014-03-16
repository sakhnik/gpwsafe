//
// GtkEmitter.cc
//
//     Created: 25.01.2012
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

#include "GtkEmitter.hh"
#include "Terminal.hh"
#include "i18n.h"

#include <iostream>
#include <gtk/gtk.h>
#include <boost/format.hpp>

namespace gPWS {

using namespace std;
typedef boost::format bfmt;

cGtkEmitter::cGtkEmitter()
{
}

cGtkEmitter::~cGtkEmitter()
{
}

void cGtkEmitter::PrintIntention(string const &subject) const
{
	cout << bfmt(_("Going to copy %s to X selection")) % subject << endl;
}

namespace {

void
_TextGetFunc(GtkClipboard     *clipboard,
             GtkSelectionData *selection_data,
             guint             info,
             gpointer          data)
{
	StringX const *val = reinterpret_cast<StringX const *>(data);
	gtk_selection_data_set_text (selection_data, val->c_str(), val->size());
	//gtk_clipboard_clear(clipboard);
	gtk_main_quit();
}

void
_TextClearFunc(GtkClipboard *clipboard,
               gpointer      data)
{
}

gboolean
_OnStdin(GIOChannel *source,
         GIOCondition condition,
         gpointer data)
{
	// Read the ready byte from the stdin
	char ch;
	int res = ::read(g_io_channel_unix_get_fd(source), &ch, 1);
	if (-1 == res)
	{
	}
	// Quit the clipboard waiting
	gtk_main_quit();
	return FALSE;
}

} //namespace;

void cGtkEmitter::Emit(StringX const &name, StringX const &val)
{
	int argc(0);
	char **argv = { NULL };
	gtk_init(&argc, &argv);

	GdkAtom primary = gdk_atom_intern("PRIMARY", TRUE);
	GtkClipboard *clipboard = gtk_clipboard_get(primary);
	if (!clipboard)
	{
		cerr << _("Failed to get clipboard") << endl;
		return;
	}

	GtkTargetList *list = gtk_target_list_new(NULL, 0);
	gtk_target_list_add_text_targets (list, 0);

	gint n_targets;
	GtkTargetEntry *targets =
		gtk_target_table_new_from_list (list, &n_targets);

	gtk_clipboard_set_with_data (clipboard,
	                             targets, n_targets,
	                             _TextGetFunc, _TextClearFunc,
	                             const_cast<StringX *>(&val));
	gtk_clipboard_set_can_store (clipboard, NULL, 0);

	gtk_target_table_free (targets, n_targets);
	gtk_target_list_unref (list);

	cout << bfmt(_("You are ready to paste the %s from PRIMARY")) % name << endl;
	cout << _("Press any key when done") << endl;

	// Switch off the canonical mode
	cRawTerminal raw_terminal;

	// Poll the stdin for any events to react on key press
	GIOChannel *channel = g_io_channel_unix_new(STDIN_FILENO);
	/*gint evt_id =*/ g_io_add_watch(channel, G_IO_IN, _OnStdin, NULL);

	gtk_main();

	gtk_clipboard_clear(clipboard);
	g_io_channel_unref(channel);
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
