#include <slugger/stream/null.h>

using namespace slugger;
using namespace slugger::stream;

null::~null()
{
}


null::null()
{
}


destination::pointer
null::create()
{
    return pointer(new null());
}


void
null::message(logging_level, const std::string &)
{
    // Do nothing.
}


// vim: set ts=4 sw=4 et :
