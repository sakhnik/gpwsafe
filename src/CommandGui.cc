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

void CommandGui::Execute(const Params &params)
{
	Gtk::Main app("org.sakhnik.gpwsafe");
	MainWindow window{params.ExpandFileName()};
	app.run(window);
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
