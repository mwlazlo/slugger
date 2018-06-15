#include <iostream>
#include <stdio.h>

#include <slugger/stream/std_err.h>

#include "../scope_guard.h"

using namespace slugger;
using namespace slugger::stream;

std_err::~std_err()
{
}


std_err::std_err() :
    destination()
{
    // it would obviously be a mistake (and useless) to create 2 std_err
    // destinations...
    pthread_mutex_init(&mux, 0);
}


destination::pointer
std_err::create()
{
    return pointer(new std_err());
}


void
std_err::message(logging_level level, const std::string &txt)
{
    priv_lock_guard guard(&mux);

    fprintf(stderr, "%s: %s", level_to_string(level), txt.c_str());
    //std::cerr << level_to_string(level) << ": " << txt;
    //std::cerr.flush();
}

// vim: set ts=4 sw=4 et :
