//
// App.cc
//
//     Created: 24.01.2012
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

#include "App.hh"
#include "StdoutEmitter.hh"
#include "Exceptions.hh"
#include "CommandAdd.hh"
#include "CommandList.hh"
#include "CommandCreate.hh"
#include "CommandEdit.hh"
#include "CommandDelete.hh"
#include "i18n.h"

#ifdef ENABLE_GTK
#include "GtkEmitter.hh"
#endif //ENABLE_GTK

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <boost/version.hpp>
#include <boost/program_options.hpp>
#include <sys/ioctl.h>


namespace gPWS {

using namespace std;


App::App(char const *program_name)
	: _program_name(program_name)
	, _use_weak_randomness_for_tests(false)
{
}

void App::Init(int argc, char *argv[])
{
	using namespace boost::program_options;

	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	int line_length = w.ws_col;

	auto named_option = [](char const *name)->typed_value<string>*
	{
#if BOOST_VERSION / 100 < 1053
		struct NamedOption
			: public typed_value<string>
		{
			const char *_name;

			NamedOption(const char *name)
				: typed_value<string>(nullptr)
				, _name(name)
			{
			}

			string name() const { return _name; }
		};
		return new NamedOption(name);
#else
		return (new typed_value<string>(nullptr))->value_name(name);
#endif
	};

	options_description desc_cmd(_("Commands"), line_length, line_length / 2);
	desc_cmd.add_options()
		("create",
			bool_switch(nullptr)
			->notifier([this](bool arg)
				{
					if (arg)
						this->_SetCommand(CommandCreate::Create());
				}),
			_("create an empty database"))
		("list",
			named_option("REGEX")
			->implicit_value("")
			->notifier(
				[this](string const &regex)
				{
					this->_SetCommand(CommandList::Create(regex));
				}),
			_("list all entries matching [REGEX]. If either -u or -p are given,"
			  " only one entry may match. If no REGEX and either of -u and -p"
			  " are given, launch TUI. If neither -u or -p are given and one"
			  " entry matches, print details."))
		("add,a",
			named_option("NAME")
			->implicit_value("")
			->notifier(
				[this](string const &name)
				{
					this->_SetCommand(CommandAdd::Create(name));
				}),
			_("add an entry"))
		("edit,e",
			named_option("REGEX")
			->implicit_value("")
			->notifier(
				[this](string const &regex)
				{
					this->_SetCommand(CommandEdit::Create(regex));
				}
			),
			_("edit an entry. If no REGEX is specified, TUI will be launched"
			  " to pick up an entry."))
		("delete",
			named_option("REGEX")
			->implicit_value("")
			->notifier(
				[this](string const &regex)
				{
					this->_SetCommand(CommandDelete::Create(regex));
				}
			),
			_("delete an entry. If no REGEX is specified, TUI will be launched"
			  " to pick up an entry."))
		;

	options_description desc_opts(_("Options"), line_length, line_length / 2);
	desc_opts.add_options()
		("file,f",
			value<string>(&_params.file_name)->default_value(_params.file_name),
			_("specify the database file"))
		("user,u",
			bool_switch(&_params.user),
			_("emit username of listed account"))
		("pass,p",
			bool_switch(&_params.pass),
			_("emit password of listed account"))
		("echo,E",
			bool_switch(nullptr)->notifier(
				[this](bool arg)
				{
					if (arg)
						this->_SetEmitter(new StdoutEmitter);
				}
			), _("force echoing of entry to stdout"))
#if ENABLE_XCLIP && ENABLE_GTK
		("xclip,x",
			bool_switch(nullptr)->notifier(
				[this](bool arg)
				{
					if (arg)
						this->_SetEmitter(new GtkEmitter);
				}
			),
			_("force copying of entry to X selection"))
		("selection,s",
			value<string>(&_params.selection)->default_value(_params.selection),
			_("X selection type ([P]RIMARY, [C]LIPBOARD, [B]OTH)"))
#endif //ENABLE_XCLIP
		("use-weak-randomness-for-tests",
			bool_switch(&_use_weak_randomness_for_tests),
			_("don't press on the pool of randomness"))
		("help,h", _("display this help and exit"))
		("version,V", _("output version information and exit"))
		;

	options_description desc(line_length, line_length / 2);
	desc.add(desc_cmd).add(desc_opts);

	positional_options_description p;
	p.add("list", 1);

	variables_map vm;
	store(command_line_parser(argc, argv)
			.options(desc)
			.positional(p)
			.run(),
		vm);
	notify(vm);

	if (vm.count("help"))
	{
		cout << _program_name << _(" - command line tool compatible with"
			" Conterpane's PasswordSafe\n\n");
		cout << _("Usage: ") << _program_name << _(" [OPTION] command [ARG]\n");
		cout << desc << endl;
		throw ExitEx(0);
	}

	if (vm.count("version"))
	{
		cout << _program_name << " " << VERSION << endl;
		throw ExitEx(0);
	}
}

void App::_SetCommand(Command::PtrT command)
{
	if (_command)
	{
		cerr << _("Ambiguous command") << endl;
		throw ExitEx(1);
	}
	_command.reset(command.release());
}

void App::_SetEmitter(iEmitter *emitter)
{
	_params.emitter.reset(emitter);
}

void App::Run()
{
	try
	{
		if (!_command)
			_command.reset(CommandList::Create("").release());
		_command->Execute(_params);
	}
	catch (std::exception const &e)
	{
		cerr << _("Exception: ") << e.what() << endl;
		throw ExitEx(1);
	}
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
