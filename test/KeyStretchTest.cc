//
// KeyStretchTest.cc
//
//     Created: 16.01.2012
//      Author: A. Sakhnik
//

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "../src/KeyStretch.hh"
#include "../src/Defs.hh"

#include <boost/range.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(KeyStretchTest)

BOOST_AUTO_TEST_CASE(First)
{
	using namespace gPWS;
	StringX pass("first123");
	string salt("\x12\xE8\xA7\x6F\x0B\x20\x84\x7A"
	            "\x51\x49\xC3\x9D\xD3\xA5\xB1\xD5"
	            "\x44\x46\x3A\xE6\x2D\x89\x99\x10"
	            "\x44\x44\x85\x59\x7A\x89\xE3\x87");
	cKeyStretch key_stretch(pass.c_str(), pass.size(),
	                        salt.c_str(), salt.size(),
	                        2048);
	const uint8_t ref[] =
	{
		0x97, 0xE3, 0xD3, 0x0F, 0x55, 0x86, 0x76, 0xC1,
		0x1C, 0x2A, 0xA6, 0xFE, 0xD1, 0xEC, 0xA4, 0x04,
		0xD9, 0xF4, 0xB2, 0x88, 0xB4, 0x73, 0x14, 0x07,
		0xDC, 0x76, 0xD0, 0x1B, 0xEC, 0x2A, 0x02, 0x65
	};
	BOOST_CHECK_EQUAL_COLLECTIONS(boost::begin(ref),
	                              boost::end(ref),
	                              key_stretch.Get(),
	                              key_stretch.Get() + key_stretch.LENGTH);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: set noet ts=4 sw=4 tw=80:
