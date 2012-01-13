//
// File3.cc
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#include "File3.hh"
#include "Defs.hh"

#include <cstring>
#include <iostream>

namespace gPWS {

using namespace std;

cFile3::cFile3()
{
    _file.exceptions(_file.failbit|_file.badbit);
}

cFile3::~cFile3()
{
}

int cFile3::Open(char const* fname)
{
    if (!_fname.empty())
        return -1;
    _fname = fname;
    if (_fname.empty())
        return -1;

    try
    {
        _file.open(fname, _file.in|_file.out|_file.binary);

        const string TAG ("PWS3");
        char tag[TAG.size() + 1];
        _file.read(tag, TAG.size());
        tag[TAG.size()] = 0;
        if (tag != TAG)
            return -1;
    }
    catch (fstream::failure const& e)
    {
        cerr << "Exception opening/reading file" << endl;
        return -1;
    }
    return 0;
}

int cFile3::Close()
{
    if (_fname.empty())
        return -1;
    _fname.clear();
    _file.close();
    return 0;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
