#include <cassert>
#include <slugger/stream/tee.h>

using namespace slugger;
using namespace slugger::stream;

tee::~tee()
{
    delete lhs;
    delete rhs;
}


tee::tee(const pointer &a1, const pointer &a2) :
    destination(),
    lhs(a1),
    rhs(a2)
{
    assert(lhs);
    assert(rhs);
}


destination::pointer
tee::create(const pointer &a1, const pointer &a2)
{
    return pointer(new tee(a1, a2));
}


void
tee::message(logging_level level, const std::string &txt)
{
    lhs->message(level, txt);
    rhs->message(level, txt);
}


// vim: set ts=4 sw=4 et :
