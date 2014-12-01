//
// Matcher.hh
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


#pragma once

#include "Defs.hh"
#include <regex.h>

namespace gPWS {

struct Matcher
{
	virtual ~Matcher() = default;
	virtual const char *GetAbbreviation() const = 0;
	virtual void SetQuery(const char *query) = 0;

	typedef std::pair<size_t, size_t> MatchT;
	virtual MatchT Search(const StringX &entry) = 0;

	Matcher() = default;
	Matcher(const Matcher &) = delete;
	Matcher& operator=(const Matcher &) = delete;
	Matcher(Matcher &&) = delete;
};

class SubstringMatcher
	: public Matcher
{
public:
	SubstringMatcher();
	virtual const char *GetAbbreviation() const { return "substring"; }
	virtual void SetQuery(const char *query);
	virtual MatchT Search(const StringX &entry);

private:
	std::string _query;
};

class FuzzyMatcher
	: public Matcher
{
public:
	FuzzyMatcher();
	~FuzzyMatcher();
	virtual const char *GetAbbreviation() const { return "fuzzy"; }
	virtual void SetQuery(const char *query);
	virtual MatchT Search(const StringX &entry);

private:
	regex_t _re;

	void _CompileQuery(const char *query);
};

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
