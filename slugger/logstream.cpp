#include <slugger/logstream.h>

#include "scope_guard.h"

using namespace slugger;

// initialise static buffers...
basic_debugbuf debug_buf(debug_t);
basic_debugbuf info_buf(info_t);
basic_debugbuf notice_buf(notice_t);
basic_debugbuf warning_buf(warning_t);
basic_debugbuf err_buf(err_t);
basic_debugbuf crit_buf(crit_t);
basic_debugbuf alert_buf(alert_t);
basic_debugbuf emerg_buf(emerg_t);

// initialise streams...
debugstream slugger::debug(&debug_buf);
infostream slugger::info(&info_buf);
noticestream slugger::notice(&notice_buf);
warningstream slugger::warning(&warning_buf);
errstream slugger::err(&err_buf);
critstream slugger::crit(&crit_buf);
alertstream slugger::alert(&alert_buf);
emergstream slugger::emerg(&emerg_buf);

basic_debugbuf::basic_debugbuf(logging_level l) : level(l)
{
    pthread_mutex_init(&buffer_mux, 0);
}

basic_debugbuf::~basic_debugbuf()
{
    pthread_mutex_destroy(&buffer_mux);
}

int
basic_debugbuf::overflow(int c)
{
    if(get_logging_level() <= level) {
        if(c != traits_type::eof()) {
            priv_lock_guard guard(&buffer_mux);
            std::string& buf = buffer_data[pthread_self()];
            if(buf.size() == 0)
                if(get_stream_prefix())
                    buf.append(get_stream_prefix()());
            buf.append(1, c);
        }
    }
    return c;
}

int
basic_debugbuf::sync()
{
    if(get_logging_level() <= level) {
        priv_lock_guard guard(&buffer_mux);
        std::string& buf = buffer_data[pthread_self()];
        if(buf.size() > 0) {
            get_singleton().message(level, buf);
            buf.assign("");
        }
    }
    return 0;
}

std::streamsize
basic_debugbuf::xsputn(const char* s, std::streamsize n)
{
    if(get_logging_level() <= level) {
        priv_lock_guard guard(&buffer_mux);
        std::string& buf = buffer_data[pthread_self()];
        if(buf.size() == 0)
            if(get_stream_prefix())
                buf.append(get_stream_prefix()());
        buf.append(s, n);
    }
    return n;
}

logstream::logstream(basic_debugbuf* buf):
    std::ostream(buf)
{
}

logstream::~logstream()
{
}

// vim: set ts=4 sw=4 et :
