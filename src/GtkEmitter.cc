//
// GtkEmitter.cc
//
//     Created: 25.01.2012
//      Author: A. Sakhnik
//

#include "GtkEmitter.hh"

#include <iostream>
#include <gtk/gtk.h>

namespace gPWS {

using namespace std;

cGtkEmitter::cGtkEmitter()
{
}

cGtkEmitter::~cGtkEmitter()
{
}

void cGtkEmitter::PrintIntention(string const &subject) const
{
    cout << "Going to copy " << subject << " to X selection" << endl;
}

static void
text_get_func(GtkClipboard     *clipboard,
              GtkSelectionData *selection_data,
              guint             info,
              gpointer          data)
{
    StringX const *val = reinterpret_cast<StringX const *>(data);
    gtk_selection_data_set_text (selection_data, val->c_str(), val->size());
    gtk_clipboard_clear(clipboard);
    gtk_main_quit();
}

static void
text_clear_func(GtkClipboard *clipboard,
                gpointer      data)
{
}

void cGtkEmitter::Emit(StringX const &name, StringX const &val)
{
    int argc(0);
    char **argv = { NULL };
    gtk_init(&argc, &argv);

    GdkAtom primary = gdk_atom_intern("PRIMARY", TRUE);
    GtkClipboard *clipboard = gtk_clipboard_get(primary);
    if (!clipboard)
    {
        cerr << "Failed to get clipboard" << endl;
        return;
    }

    GtkTargetList *list = gtk_target_list_new(NULL, 0);
    gtk_target_list_add_text_targets (list, 0);

    gint n_targets;
    GtkTargetEntry *targets =
        gtk_target_table_new_from_list (list, &n_targets);

    gtk_clipboard_set_with_data (clipboard,
                                 targets, n_targets,
                                 text_get_func, text_clear_func,
                                 const_cast<StringX *>(&val));
    gtk_clipboard_set_can_store (clipboard, NULL, 0);

    gtk_target_table_free (targets, n_targets);
    gtk_target_list_unref (list);

    cout << "You are ready to paste the " << name << " from PRIMARY" << endl;
    cout << "Press any key when done" << endl;

    gtk_main();
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
