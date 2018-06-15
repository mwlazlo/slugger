#include <iostream>
#include <stdio.h>

#include <slugger/stream/std_out.h>

#include "../scope_guard.h"

using namespace slugger;
using namespace slugger::stream;

std_out::~std_out()
{
}


std_out::std_out() :
    destination()
{
    // it would obviously be a mistake (and useless) to create 2 std_out
    // destinations...
    pthread_mutex_init(&mux, 0);
}


destination::pointer
std_out::create()
{
    return pointer(new std_out());
}


void
std_out::message(logging_level level, const std::string &txt)
{
    priv_lock_guard guard(&mux);
    printf("%s: %s", level_to_string(level), txt.c_str());
    //std::cout << level_to_string(level) << ": " << txt;
    //std::cout.flush();
}

// vim: set ts=4 sw=4 et :
