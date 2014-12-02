//
// Terminal.cc
//
//     Created: 22.01.2012
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

#include "Terminal.hh"
#include "Random.hh"
#include "i18n.h"
#include "Matcher.hh"

#include <iostream>
#include <sstream>
#include <cstdio>
#include <stdexcept>
#include <boost/scope_exit.hpp>
#include <boost/format.hpp>
#include <readline/readline.h>
#include <sys/ioctl.h>
#include <ncurses.h>

namespace gPWS {

using namespace std;
typedef boost::format bfmt;

RawTerminal::RawTerminal(bool new_line)
	: _new_line(new_line)
{
	tcgetattr(STDIN_FILENO, &_tio);

	struct termios new_tio = _tio;
	new_tio.c_lflag &= ~(ICANON);
	// A hack for sparc (from pwsafe)
	new_tio.c_cc[VMIN] = 1;
	new_tio.c_cc[VTIME] = 0;
	// Turn off echo too; no need to show them the char they pressed
	new_tio.c_lflag &= ~(ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

RawTerminal::~RawTerminal()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &_tio);
	if (_new_line)
		cout << endl;
}

//////////////////////////////////////////////////////////////////////////////

StringX Terminal::GetPassword(char const *prompt)
{
	RawTerminal raw_terminal(true);

	rl_completion_entry_function = NULL;
	char *input = readline(prompt);
	if (!input)
		throw runtime_error(_("Can't get password"));
	BOOST_SCOPE_EXIT((&input)) {
		memset(input, 0, strlen(input));
		delete input;
	} BOOST_SCOPE_EXIT_END

	return StringX(input);
}

StringX Terminal::GetText(char const *prompt,
                          StringX const &def)
{
	rl_completion_entry_function = NULL;
	char *input = readline(prompt);
	if (!input)
		throw runtime_error(_("Can't get text"));
	BOOST_SCOPE_EXIT((&input)) {
		memset(input, 0, strlen(input));
		delete input;
	} BOOST_SCOPE_EXIT_END

	StringX res(input);
	if (res.empty())
		return def;
	return res;
}

StringX Terminal::EnterPassword(char const *prompt1, char const *prompt2)
{
	// Copied from pwsafe
	while (true)
	{
		StringX pw1 = GetPassword(prompt1);
		if (pw1.empty())
		{
			if (GetYN(_("Generate random password? [y] "), true))
			{
				pw1 = RandomPassword();
				if (!pw1.empty())
					return pw1;
				else
					continue; // back to entering by hand for them
				// (perhaps they want to copy only a subset
				//  of the original pw)
			} // else let them have an empty password, though they'll have
			// to enter it twice
		}
		StringX pw2 = GetPassword(prompt2);
		if (pw1 == pw2)
		{
			return pw1;
		}
		cerr << _("Passwords do not match") << endl;
	}
}

bool Terminal::GetYN(char const *prompt, int def_val)
{
	// Copied from pwsafe
	while (true)
	{
		char c = GetChar(prompt, def_val > 0 ? 'y' : def_val == 0 ? 'n' : -1);
		switch (tolower(c))
		{
		case 'y':
			return true;
		case 'n':
			return false;
			// default: prompt again until we get a good answer
		}
	}
}

char Terminal::GetChar(char const *prompt, const int def_val)
{
	// Copied from pwsafe
	struct termios tio;
	tcgetattr(STDIN_FILENO, &tio);
	{
		termios new_tio = tio;
		new_tio.c_lflag &= ~(ICANON);
		// now that we turn ICANON off we *must* set VMIN=1
		// or on sparc the read() buffers 4 at a time
		new_tio.c_cc[VMIN] = 1;
		new_tio.c_cc[VTIME] = 0;
		tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
	}
	BOOST_SCOPE_EXIT((&tio)){
		tcsetattr(STDIN_FILENO, TCSANOW, &tio);
	} BOOST_SCOPE_EXIT_END

	while (true)
	{
		cout << prompt << flush;
		char x;
		ssize_t rc = read(STDIN_FILENO, &x, 1);

		if (rc != 1)
			throw runtime_error(_("Read error"));
		switch (x)
		{
		case '\r':
			cout << endl;
			// fall through to '\n'
		case '\n':
			if (def_val >= 0)
				return def_val;
			// else there is no default and the user must hit a proper char
			break;
		default:
			cout << endl;
			return x;
		}
		// if we get this far the user didn't answer, and we loop
		// and reprompt them
	}
}

StringX Terminal::RandomPassword()
{
	// Copied from pwsafe

	// here I implement the 'easyvision' mode of pwsafe 1.9.x
	// where the resulting ascii has nice legibility properties
	// for those who copy these by hand
	const static char all_alphanum[] = "abcdefghijklmnopqrstuvwxyz"
	                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	                                   "0123456789";
	const static char easyvision_alphanum[] = "abcdefghijkmnopqrstuvwxyz"
	                                          "ABCDEFGHJKLMNPQRTUVWXY"
	                                          "346789";
	const static char easyvision_symbol[] = "+-=_@#$%^&<>/~\\?";
	const static char hex_only[] = "0123456789abcdef";
	const static char digits_only[] = "0123456789";


	int entropy_needed = 20*8; // enough for proper initialization
	                           // of a SHA1 hash, and enough for proper direct
	                           // keying of 128-bit block ciphers
	int type = 0;
	while (true)
	{
		const char* type_name = "";
		const char* sets[2] = { "", "" };
		int entropy_per_char;
		bool one_char_from_each_type = true;
		switch (type)
		{
		case 0:
			type_name = _("alpha/digit/symbol");
			sets[0] = all_alphanum;
			sets[1] = easyvision_symbol;
			entropy_per_char = 628;
			// 100 * log2(26+26+10+16) = log2(78); best case
			break;
		case 1:
			type_name = _("alpha/digit");
			sets[0] = all_alphanum;
			entropy_per_char = 595;
			break;
		case 2:
			type_name = _("easy-to-read alpha/digit");
			sets[0] = easyvision_alphanum;
			entropy_per_char = 555;
			// 100 * log2(25+22+10) = log2(57); worse case
			break;
		case 3:
			type_name = _("easy-to-read alpha/digit/symbol");
			sets[0] = easyvision_alphanum;
			sets[1] = easyvision_symbol;
			entropy_per_char = 597;
			break;
		case 4:
			type_name = _("digits only");
			sets[0] = digits_only;
			entropy_per_char = 332; // 100 * log2(10)
			one_char_from_each_type = false;
			break;
		case 5:
			type_name = _("hex digits only");
			sets[0] = hex_only;
			entropy_per_char = 400; // 100 * log2(16)
			one_char_from_each_type = false;
			break;
		default:
			// wrap around back to type 0
			type = 0;
			continue;
		}

		const int set0_chars = strlen(sets[0]);
		const int total_chars = set0_chars + strlen(sets[1]);

		// But we are not going to generate all possible passwords
		// because we are going to exclude those that don't have at least
		// one char from each type, so that reduces the entropy_per_char
		// if originally we had 2^(num_chars * entropy_per_char) possible
		// passwords, and we exclude (in the worst case) (and double-counting
		// those passwords that have two types of char missing)
		// (57-25)/57 ^ num_chars + (57-22)/57 ^ num_chars +
		// + (57-10)/57 ^ num_chars of these, we reduce the bits of entropy
		// per char by
		// log2(57)-log2(57-25) + log2(57)-log2(57-22) + log2(57)-log2(57-10) =
		// = 1.82 pessimist bits/char
		if (one_char_from_each_type)
			entropy_per_char -= 182;

		const int num_chars = 1+100*entropy_needed/entropy_per_char;
		// we want 20*8 bits of entropy in our password (thus good enough
		// to create good SHA1 hashes/to key 128-bit key secret key algo's);
		// +1 is in lou of rounding the division properly

		StringX pw;
		bool got_upper, got_lower, got_num, got_sym;
		do
		{
			pw.erase();

			got_upper = false;
			got_lower = false;
			got_num = false;
			got_sym = false;

			for (int i=0; i<num_chars; i++)
			{
				unsigned char idx;
				do
				{
					Random::Randomize(&idx, 1, true);
					idx &= 0x7f;
					// might as well strip off the upper bit since
					// total_chars is never more than 64, and
					// such a stripping doesn't change the distribution
				} while (idx >= total_chars);

				char c;
				if (idx < set0_chars)
					c = sets[0][idx];
				else
					c = sets[1][idx-set0_chars];

				pw += c;

				if (islower(c)) got_lower = true;
				else if (isupper(c)) got_upper = true;
				else if (isdigit(c)) got_num = true;
				else got_sym = true;
			}
		} while (one_char_from_each_type &&
		         (!got_lower ||
		          !got_upper ||
		          !got_num   ||
		          (sets[1][0] && !got_sym)));
		// some systems want one of each type of char in the password,
		// so might as well do it all the time, even though it is a tiny bit
		// less random this way (but we already took that into account
		// in entropy_per_char)

		// see what the user thinks of this one
		OStringStreamX prompt_oss;
		prompt_oss << bfmt(_("Use %s")) % pw << "\n"
		           << bfmt(_("type %s, length %d, %d bits of entropy [y/N/ /+/-/q/?] ? "))
		               % type_name % pw.length() % entropy_needed;
		StringX prompt = prompt_oss.str();
		switch (tolower(GetChar(prompt, 'n')))
		{
		case 'y':
			return pw;
		case 'q':
			return "";
		case ' ':
			type++;
			break;
		case '-':
			if (entropy_needed > 128)
				entropy_needed -= 32;
			else if (entropy_needed > 64)
				entropy_needed -= 16;
			else if (entropy_needed > 32)
				entropy_needed -= 8;
			// else you can't go any lower
			break;
		case '+': case '=':
			if (entropy_needed < 64)
				entropy_needed += 8;
			else if (entropy_needed < 128)
				entropy_needed += 16;
			// so we can hit 112, the magic number for WEP keys
			else
				entropy_needed += 32;
			break;
		case '?': case 'h':
			cout <<
				_("Commands:\n"
				  "  Y      Yes, accept this password\n"
				  "  N      No, generate another password of same type\n"
				  "  <space> Cycle through password types\n"
				  "  -      Lower the entropy & password length\n"
				  "  +      Raise the entropy & password length\n"
				  "  Q      Quit\n"
				  "  ?      Help\n")
				<< endl;
			continue;
			// default: show another password
		}
	}
}

bool Terminal::IsOutput()
{
	return isatty(fileno(stdout));
}

int Terminal::GetColumns()
{
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	return w.ws_col;
}

template <typename F>
class Filter
{
public:
	Filter(size_t count, const F &feed)
		: _count(count)
		, _feed(feed)
	{
	}

	const string &GetQuery() const { return _query; }

	size_t GetMaxWidth()
	{
		if (_refilter)
			_Refilter();
		return _max_width;
	}

	// Index in the feed, match position, count of matched characters
	typedef tuple<size_t, size_t, size_t> IdxPosCountT;
	typedef vector<IdxPosCountT> FilteredT;

	const FilteredT &GetFiltered()
	{
		if (_refilter)
			_Refilter();
		return _filtered;
	}

	const char *GetAbbreviation() const { return _matcher->GetAbbreviation(); }

	void ClearQuery()
	{
		if (!_query.empty())
			_refilter = true;
		_query.clear();
	}

	void QueryBackSpace()
	{
		if (_query.empty())
			return;
		_query.erase(_query.size() - 1);
		_refilter = true;
	}

	void SwitchMatcher()
	{
		if (typeid(*_matcher.get()) == typeid(FuzzyMatcher))
			_matcher.reset(new SubstringMatcher);
		else
			_matcher.reset(new FuzzyMatcher);
		_refilter = true;
	}

	void QueryAdd(char ch)
	{
		_refilter = true;
		_query.push_back(ch);
	}

private:
	size_t _count;
	const F &_feed;

	bool _refilter = true;
	string _query;
	size_t _max_width = 0;
	FilteredT _filtered;
	unique_ptr<Matcher> _matcher{ new FuzzyMatcher };

	void _Refilter()
	{
		_refilter = false;
		_max_width = 0;
		_filtered.clear();
		_matcher->SetQuery(_query.c_str());
		for (size_t i = 0; i != _count; ++i)
		{
			const auto &entry = _feed(i);
			auto match = _matcher->Search(entry);
			if (match.first != -1u)
			{
				_max_width = (std::max)(_max_width, entry.size());
				_filtered.emplace_back(i, match.first, match.second);
			}
		}
	}
};

size_t Terminal::PickUp(size_t count,
                        const function<const StringX &(size_t)> &feed)
{
	initscr();
	BOOST_SCOPE_EXIT((count)) {
		endwin();
	} BOOST_SCOPE_EXIT_END

	if (has_colors())
	{
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
	}

	noecho();
	//curs_set(FALSE);
	cbreak();
	keypad(stdscr, TRUE);

	Filter<decltype(feed)> filter(count, feed);
	int max_x{0}, max_y{0};
	size_t cursor{0};

	while (true)
	{
		getmaxyx(stdscr, max_y, max_x);
		clear();

		// First collect entries matching the query, and calculate maximum
		// width of an entry.
		size_t max_width = filter.GetMaxWidth();
		auto filtered = filter.GetFiltered();

		// Estimate parameters of the table with the selected entries.
		int columns = max_x / (max_width + 4);
		int square = sqrt(filtered.size());
		if (square && square < columns)
			columns = square;
		int rows = filtered.size() / columns;

		// Print entries
		int y{1}, x{0};
		for (auto it = begin(filtered); it != end(filtered); ++it)
		{
			auto str = feed(get<0>(*it)).c_str();
			auto pos = get<1>(*it);
			auto count = get<2>(*it);
			if (has_colors())
			{
				// Highlight the matching part in an entry
				attron(COLOR_PAIR(1));
				mvprintw(y, x, "%.*s", pos, str);
				mvprintw(y, x + pos + count, "%s", str + pos + count);
				attroff(COLOR_PAIR(1));
				attron(COLOR_PAIR(2));
				mvprintw(y, x + pos, "%.*s", count, str + pos);
				attroff(COLOR_PAIR(2));
			}
			else
				mvprintw(y, x, str);
			// Proceed to the next table cell
			++y;
			if (y > max_y || y > rows)
			{
				y = 1;
				x += max_width + 4;
				if (x + int(max_width) > max_x)
					break;
			}
		}

		// Print the query itself
		mvprintw(0, 0, "%s> %s",
		         filter.GetAbbreviation(),
		         filter.GetQuery().c_str());

		refresh();

		// Wait for user input and react on it
		auto ch = getch();
		if ((ch == '\n' || ch == EOF) && !filtered.empty())
			return get<0>(filtered[cursor]);
		else if (ch == 27) // Esc
		{
			if (!filter.GetQuery().empty())
			{
				filter.ClearQuery();
				continue;
			}
			return -1;
		}
		else if (ch == 127 || ch == 8)  // Back space
		{
			filter.QueryBackSpace();
			continue;
		}
		else if (ch == 18) // ^R -- switch matcher mode
		{
			filter.SwitchMatcher();
			continue;
		}
		else if (isprint(ch))
		{
			filter.QueryAdd(ch);
		}
	}
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
