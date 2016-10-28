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

GtkEmitter::GtkEmitter()
{
}

GtkEmitter::~GtkEmitter()
{
}

void GtkEmitter::PrintIntention(string const &subject) const
{
	cerr << bfmt(_("Going to copy %s to X selection")) % subject << endl;
}

void GtkEmitter::SetSelection(const string &selection)
{
	_selection = selection;
}

namespace {

void
_TextGetFunc(GtkClipboard     *,
             GtkSelectionData *selection_data,
             guint             /*info*/,
             gpointer          data)
{
	StringX const *val = reinterpret_cast<StringX const *>(data);
	gtk_selection_data_set_text (selection_data, val->c_str(), val->size());
	//gtk_clipboard_clear(clipboard);
	if (gtk_main_level())
		gtk_main_quit();
}

void
_TextClearFunc(GtkClipboard *,
               gpointer      /*data*/)
{
}

gboolean
_OnStdin(GIOChannel *source,
         GIOCondition /*condition*/,
         gpointer /*data*/)
{
	// Read the ready byte from the stdin
	char ch;
	int res = ::read(g_io_channel_unix_get_fd(source), &ch, 1);
	if (-1 == res)
	{
	}
	// Quit the clipboard waiting
	if (gtk_main_level())
		gtk_main_quit();
	return FALSE;
}

template<typename T, typename D>
unique_ptr<T, D> mk_ptr(T *t, D d)
{
	return unique_ptr<T, D>(t, d);
}

template <typename ClipboardsT>
void ClipboardSetup(ClipboardsT &clipboards, const StringX &val)
{
	auto list = mk_ptr(gtk_target_list_new(NULL, 0), &gtk_target_list_unref);
	gtk_target_list_add_text_targets (list.get(), 0);

	gint n_targets;
	GtkTargetEntry *targets =
		gtk_target_table_new_from_list (list.get(), &n_targets);

	for (auto &clipboard : clipboards)
	{
		gtk_clipboard_set_with_data (clipboard.get(),
									 targets, n_targets,
									 _TextGetFunc, _TextClearFunc,
									 const_cast<StringX *>(&val));
		gtk_clipboard_set_can_store (clipboard.get(), NULL, 0);
	}

	gtk_target_table_free (targets, n_targets);
}

template <typename ItemsT>
string ListItems(const ItemsT &items)
{
	string result;

	bool first{true};
	for (auto &item : items)
	{
		if (first)
			first = false;
		else
			result += ", ";
		result += item;
	}

	return result;
}

vector<const char *> ParseSelection(string selection)
{
	if (selection.empty())
		return {"BOTH"};

	switch (selection[0])
	{
	case 'P':
		return {"PRIMARY"};
	case 'C':
		return {"CLIPBOARD"};
	case 'B':
	default:
		return {"PRIMARY", "CLIPBOARD"};
	}
}

} //namespace;

void GtkEmitter::Emit(StringX const &name, StringX const &val)
{
	int argc(0);
	char **argv = { NULL };
	gtk_init(&argc, &argv);

	vector<decltype(mk_ptr(gtk_clipboard_get(GdkAtom{}),
	                                         &gtk_clipboard_clear))>
		clipboards;
	vector<const char *> names;

	for (auto selection : ParseSelection(_selection))
	{
		GdkAtom primary = gdk_atom_intern(selection, TRUE);
		auto clipboard = mk_ptr(gtk_clipboard_get(primary), &gtk_clipboard_clear);
		if (!clipboard)
		{
			cerr << _("Failed to get ") << selection << endl;
			continue;
		}
		clipboards.push_back(move(clipboard));
		names.push_back(selection);
	}
	assert(clipboards.size() == names.size());
	if (clipboards.empty())
		return;

	ClipboardSetup(clipboards, val);

	cout << bfmt(_("You are ready to paste the %s from ")) % name;
	cout << ListItems(names) << '\n';
	cout << _("Press any key when done") << endl;

	// Switch off the canonical mode
	RawTerminal raw_terminal;

	// Poll the stdin for any events to react on key press
	auto channel = mk_ptr(g_io_channel_unix_new(STDIN_FILENO),
	                      &g_io_channel_unref);
	/*gint evt_id =*/ g_io_add_watch(channel.get(), G_IO_IN, _OnStdin, NULL);

	gtk_main();
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
