//
// EntryTest.cc
//
//     Created: 02.02.2012
//      Author: A. Sakhnik
//

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "../src/Entry.hh"

using namespace std;

BOOST_AUTO_TEST_SUITE(EntryTest)

BOOST_AUTO_TEST_CASE(TestDiff)
{
    using namespace gPWS;
    cEntry::PtrT entry0(cEntry::Create());
    entry0->SetGroup("arch");
    entry0->SetTitle("aur");

    cEntry::PtrT entry1(cEntry::Create());
    entry1->SetTitle("aur2");
    entry1->SetUser("sakhnik");

    cEntry::DiffT diff = entry1->Diff(entry0);

    BOOST_CHECK_EQUAL(diff.size(), 3);
    BOOST_CHECK_EQUAL(diff[0].type, cEntry::FT_GROUP);
    BOOST_CHECK_EQUAL(diff[0].change, cEntry::sChange::C_DELETED);
    BOOST_CHECK_EQUAL(diff[1].type, cEntry::FT_TITLE);
    BOOST_CHECK_EQUAL(diff[1].change, cEntry::sChange::C_MODIFIED);
    BOOST_CHECK_EQUAL(diff[2].type, cEntry::FT_USER);
    BOOST_CHECK_EQUAL(diff[2].change, cEntry::sChange::C_ADDED);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: set et ts=4 sw=4:
