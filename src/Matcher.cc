//
// SubstringMatcher.cc
//
//     Created: 28.11.2014
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


#include "Matcher.hh"
#include <string>

namespace gPWS {

using namespace std;

SubstringMatcher::SubstringMatcher()
{
}

void SubstringMatcher::SetQuery(const char *query)
{
	_query = query;
}

Matcher::MatchT SubstringMatcher::Search(const StringX &entry)
{
	auto pos = entry.find(_query.c_str());
	if (pos == entry.npos)
		return MatchT{ -1u, 0 };
	return MatchT{ pos, _query.size() };
}

FuzzyMatcher::FuzzyMatcher()
{
	_CompileQuery("");
}

FuzzyMatcher::~FuzzyMatcher()
{
	regfree(&_re);
}

void FuzzyMatcher::SetQuery(const char *query)
{
	assert(query);

	regfree(&_re);
	_CompileQuery(query);
}

void FuzzyMatcher::_CompileQuery(const char *query)
{
	bool ignore_case{true};
	string str;
	while (auto ch = *query++)
	{
		if (!str.empty())
			str += string{"[^"} + ch + "]*";
		str += ch;
		if (isupper(ch))
			ignore_case = false;
	}

	auto status = regcomp(&_re, str.c_str(), ignore_case ? REG_ICASE : 0);
	if (status != 0)
	{
		assert(false);
	}
}

Matcher::MatchT FuzzyMatcher::Search(const StringX &entry)
{
	regmatch_t match = {};
	if (regexec(&_re, entry.c_str(), 1, &match, 0) != 0)
		return MatchT{ -1u, 0 };
	return MatchT{ match.rm_so, match.rm_eo - match.rm_so };
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
