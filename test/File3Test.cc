//
// File3Test.cc
//
//     Created: 29.01.2012
//      Author: A. Sakhnik
//

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "../src/File3.hh"
#include "../src/Defs.hh"
#include "../src/Gcrypt.hh"
#include "../src/Debug.hh"

#include <boost/format.hpp>
#include <boost/iterator/indirect_iterator.hpp>

using namespace std;

struct sGlobalFixture
{
    sGlobalFixture()
    {
        gPWS::cGcrypt::Init();
    }

} global_fixture;

BOOST_AUTO_TEST_SUITE(File3Test)

BOOST_AUTO_TEST_CASE(TestWriteRead)
{
    srand(static_cast<unsigned>(time(NULL)));

    using namespace gPWS;

    typedef vector<sField::PtrT> FieldsT;
    FieldsT fields1;
    for (unsigned i = 0, n = rand() % 999; i != n; ++i)
    {
        sField::PtrT field(new sField);
        field->type = rand() % 256;
        field->value.resize(rand() % 256);
        generate(field->value.begin(), field->value.end(), &rand);
        fields1.push_back(field);
    }

    char const *const fname = "/tmp/file3_test.psafe3";
    char const *const pass = "!23$QweR";

    cFile3 file1;
    file1.OpenWrite(fname, pass, true, false);

    for (FieldsT::const_iterator i = fields1.begin(); i != fields1.end(); ++i)
        file1.WriteField(*i);

    file1.CloseWrite();

    cFile3 file2;
    file2.OpenRead(fname, pass);
    FieldsT fields2;
    sField::PtrT field;
    while ((field = file2.ReadField()))
    {
        fields2.push_back(field);
    }

    BOOST_CHECK_EQUAL(fields1.size(), fields2.size());

    //typedef boost::indirect_iterator<FieldsT::iterator> IterT;
    //BOOST_CHECK_EQUAL_COLLECTIONS(IterT(fields1.begin()),
    //                              IterT(fields1.end()),
    //                              IterT(fields2.begin()),
    //                              IterT(fields2.end()));
    ::unlink(fname);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: set et ts=4 sw=4:
