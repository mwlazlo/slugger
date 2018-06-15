#include <cerrno>
#include <cstring>
#include <stdlib.h>

#include <slugger/stream/file.h>

#include "../scope_guard.h"

using namespace slugger;
using namespace slugger::stream;

file::~file()
{
    if(fp.is_open())
        fp.close();
}


file::file(const std::string &a_filename) :
    destination(),
    filename(a_filename),
    fp(),
    since(0)
{
    fp.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    pthread_mutex_init(&mux, 0);
}


destination::pointer
file::create(const std::string &a_filename)
{
    return pointer(new file(a_filename));
}


void
file::message(logging_level level, const std::string &txt)
{
    priv_lock_guard guard(&mux);
    if(fp.is_open()) {
        time_t now;
        time(&now);
        if (now - since >= 60 * 60) {
            //
            // Close the file every hour.
            // This makes the logrotate(8) simple.
            //
            fp.close();
        }
    }

    if(!fp.is_open()) {
        fp.open(filename.c_str(), std::ios_base::out | std::ios_base::ate | std::ios_base::app);
        time(&since);
    }

    fp << level_to_string(level) << ": " << txt;
}


// vim: set ts=4 sw=4 et :
