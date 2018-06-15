#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <stdexcept>

#include <slugger/slugger.h>

#include "scope_guard.h"

using namespace slugger;


// a bit better than global variables
static struct log_data
{
    logging_level level_default;
#if defined(PER_THREAD_LEVEL)
    typedef std::map<pthread_t, logging_level> levels_type;
    levels_type levels;
    pthread_mutex_t levels_mux;
    
    log_data():
        level_default(debug_t)
    {
        pthread_mutex_init(&levels_mux, 0);
        init();
    }

    ~log_data()
    {
        pthread_mutex_destroy(&levels_mux);
    }
#else
    log_data():
        level_default(debug_t)
    {
        init();
    }
#endif

    /**
      * The syslog facility (when using syslog target)
      */
    syslog_facility facility;

    /**
      * The program's name
      */
    std::string program_name;

    /**
     * User defined stream id prefix
     */
    stream_prefix_func stream_op;

    void init()
    {
        facility = local0_t;
        program_name = "Untitled Application";
        stream_op = 0;
    }

} priv_data;

// In the absence of boost::shared_ptr, this class will ensure the destination
// is freed.
struct singleton_guard
{
    destination *singleton;

    singleton_guard(): 
        singleton(0)
    {
    }

    virtual ~singleton_guard()
    {
        replace(0);
    }

    void replace(destination *dest)
    {
        if(singleton)
            delete singleton;
        singleton = dest;
    }

};
static singleton_guard guard;


void
slugger::register_stream(const destination::pointer ls)
{
    guard.replace(ls);
}

static bool
has_no_control_tty()
{
    int fd = open("/dev/tty", O_RDONLY);
    if(fd >= 0) {
        close(fd);
        return false;
    }
    return true;
}

destination&
slugger::get_singleton()
{
    destination *ptr = guard.singleton;
    if (!ptr) {
        if(has_no_control_tty())
            ptr = stream::sys_log::create();
        else
            ptr = stream::std_err::create();
        assert(ptr);
        guard.replace(ptr);
    }
    return *ptr;
}

const char*
slugger::level_to_string(logging_level level)
{
    switch(level) {
        case debug_t:
            return "debug";
        case info_t:
            return "info";
        case notice_t:
            return "notice";
        case warning_t:
            return "warning";
        case err_t:
            return "err";
        case crit_t:
            return "crit";
        case alert_t:
            return "alert";
        case emerg_t:
            return "emerg";
        default:
            return "unknown";
    }
}

std::ostream& slugger::operator<<(std::ostream& o, const slugger::logging_level& ll)
{
    o << slugger::level_to_string(ll);
    return o;
}

struct _log_parse_table_t 
{       
    std::string name;
    logging_level level;
    bool operator==(const std::string &str)
    {
        return name == str;
    }       
};  
        
logging_level
slugger::string_to_level(const std::string& level)
{
    static _log_parse_table_t table[] = {
        { "debug",   debug_t   },
        { "info",    info_t    },
        { "notice",  notice_t  },
        { "warning", warning_t },
        { "err",     err_t     },
        { "crit",    crit_t    },
        { "alert",   alert_t   },
        { "emerg",   emerg_t   },
        { "INVALID", debug_t   }
    };
    static _log_parse_table_t* table_end = table + ((sizeof(table) / sizeof(table[0])) - 1);
    _log_parse_table_t* t = std::find(table, table_end, level);
    if(t == table_end) {
        throw std::invalid_argument("Invalid log level");
    } else {
        return t->level;
    }
}

std::istream& slugger::operator>>(std::istream& i, slugger::logging_level& l)
{
    std::string buf;
    i >> buf;
    l = slugger::string_to_level(buf);
    return i;
}


void
slugger::set_facility(syslog_facility f)
{
    priv_data.facility = f;
}

syslog_facility
slugger::get_facility()
{
    return priv_data.facility;
}

void
slugger::set_program_name(const std::string &p)
{
    priv_data.program_name = p;
}

const std::string&
slugger::get_program_name()
{
    return priv_data.program_name;
}

void
slugger::set_logging_level_default(logging_level level)
{
    priv_data.level_default = level;
}

void
slugger::set_logging_level(logging_level level)
{
#if defined(PER_THREAD_LEVEL)
    priv_lock_guard guard(&priv_data.levels_mux);
    priv_data.levels[pthread_self()] = level;
#else
    priv_data.level_default = level;
#endif
}

logging_level
slugger::get_logging_level()
{
#if defined(PER_THREAD_LEVEL)
    priv_lock_guard guard(&priv_data.levels_mux);
    log_data::levels_type::iterator i = priv_data.levels.find(pthread_self());
    if(i == priv_data.levels.end())
        return priv_data.level_default;
    else
        return i->second;
#else
    return priv_data.level_default;
#endif
}

void
slugger::set_stream_prefix(stream_prefix_func fn)
{
    priv_data.stream_op = fn;
}

stream_prefix_func
slugger::get_stream_prefix()
{
    return priv_data.stream_op;
}

// vim: set ts=4 sw=4 et :
