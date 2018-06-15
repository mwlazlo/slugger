#include <iostream>
#include <algorithm>
#include <sstream>
#include <iterator>

#include <slugger/slugger.h>

#include <time.h>
#include <unistd.h>

namespace log = slugger;

void use_streams(const std::string& prefix="")
{
    log::debug << prefix << "Hello debug" << std::endl;
    log::info << prefix << "Hello info" << std::endl;
    log::notice << prefix << "Hello notice" << std::endl;
    log::warning << prefix << "Hello warning" << std::endl;
    log::err << prefix << "Hello err" << std::endl;
    log::crit << prefix << "Hello crit" << std::endl;
    log::alert << prefix << "Hello alert" << std::endl;
    log::emerg << prefix << "Hello emerg" << std::endl;
}

void fork_use_streams()
{
    if(!fork()) {
        log::debug << "Hello (forked) debug" << std::endl;
        log::info << "Hello (forked) info" << std::endl;
        log::notice << "Hello (forked) notice" << std::endl;
        log::warning << "Hello (forked) warning" << std::endl;
        log::err << "Hello (forked) err" << std::endl;
        log::crit << "Hello (forked) crit" << std::endl;
        log::alert << "Hello (forked) alert" << std::endl;
        log::emerg << "Hello (forked) emerg" << std::endl;
    }
}

void test_file()
{
    log::destination::pointer f = log::stream::file::create("test.txt");
    log::register_stream(f);
    use_streams();
}

void test_null() 
{
    log::destination::pointer f = log::stream::null::create();
    log::register_stream(f);
    use_streams();
}

void test_std_err()
{
    log::destination::pointer f = log::stream::std_err::create();
    log::register_stream(f);
    use_streams();
}

void test_std_out()
{
    log::destination::pointer f = log::stream::std_out::create();
    log::register_stream(f);
    use_streams();
}

void test_sys_log()
{
    log::destination::pointer f = log::stream::sys_log::create();
    log::register_stream(f);
    log::set_program_name("logging test");
    use_streams();
    fork_use_streams();
}

void test_tee()
{
    log::destination::pointer rhs = log::stream::file::create("tee.txt");
    log::destination::pointer lhs = log::stream::std_err::create();
    log::destination::pointer f = log::stream::tee::create(lhs, rhs);
    log::register_stream(f);
    use_streams();
}

void test_level()
{
    std::cerr << "Setting log level to \"warning_t\"" << std::endl;
    log::set_logging_level(log::warning_t);
    use_streams();
}

extern "C" {
void* logger_thread(void*);
}

// need to mix things up a bit
void rand_sleep()
{
    struct timespec sleepTime;
    struct timespec remainingSleepTime;

    sleepTime.tv_sec=0;
    sleepTime.tv_nsec=10000;
    nanosleep(&sleepTime,&remainingSleepTime);
}

void* logger_thread(void* param)
{
    unsigned int id = *(static_cast<unsigned int*>(param));
    log::logging_level l = log::debug_t;
    switch(id) {
        case 0:
            l = log::debug_t;
            break;
        case 1:
            l = log::info_t;
            break;
        case 2:
            l = log::notice_t;
            break;
        case 3:
            l = log::err_t;
            break;
        case 4:
            l = log::alert_t;
            break;
        default:
            l = log::debug_t;
            break;
            
    }
    rand_sleep();
    log::notice << "Hello, this is thread " << id << ". My log level is " << log::level_to_string(l) << std::endl;
    rand_sleep();
    log::set_logging_level(l);
    rand_sleep();
    log::debug << id << std::endl;
    rand_sleep();
    log::info << id << std::endl;
    rand_sleep();
    log::notice << id << std::endl;
    rand_sleep();
    log::warning << id << std::endl;
    rand_sleep();
    log::err << id << std::endl;
    rand_sleep();
    log::crit << id << std::endl;
    rand_sleep();
    log::alert << id << std::endl;
    rand_sleep();
    log::emerg << id << std::endl;
    rand_sleep();
    log::notice << "Bye, this is thread " << id << std::endl;
    return 0;
}

void test_thread()
{
    static const int nthreads = 5;
    pthread_t threads[nthreads];
    unsigned int ids[nthreads]; // storage for thread ids

    log::destination::pointer rhs = log::stream::sys_log::create();
    log::destination::pointer lhs = log::stream::std_err::create();
    log::destination::pointer f = log::stream::tee::create(lhs, rhs);
    log::register_stream(f);

    log::set_program_name("logging test");

    std::cerr << "Starting " << nthreads << " logging threads, output to syslog AND stderr" << std::endl;
    for(int i = 0; i < nthreads; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, &logger_thread, static_cast<void*>(&ids[i]));
    }

    for(int i = 0; i < nthreads; i++)
        pthread_join(threads[i], NULL);

}

typedef void(*func)();

struct table_t {
    std::string name;
    func fun;
    bool operator==(const std::string &str)
    {
        return name == str;
    }
};

std::ostream& operator<<(std::ostream& o, const table_t& t)
{
    o << t.name;
    return o;
}

int main(int argc, char **argv)
{
    table_t table[] = {
        { "file",    &test_file    },
        { "null",    &test_null    },
        { "std_err", &test_std_err },
        { "std_out", &test_std_out },
        { "sys_log", &test_sys_log },
        { "tee",     &test_tee     },
        { "level",   &test_level   },
        { "thread",  &test_thread  },
        { "",        NULL          }
    };
    table_t* table_end = table + ((sizeof(table) / sizeof(table[0])) - 1);
    func fun = 0;

    if(argc == 2) {
        table_t* t = std::find(table, table_end, argv[1]);
        fun = t->fun;
    } 
    if(!fun) {
        // concat options
        std::stringstream stream;
        std::copy(table, table_end, std::ostream_iterator<table_t>(stream, " | "));
        std::string help = stream.str();
        help.erase(help.length() - 2);

        std::cout << "Usage: " << argv[0] << ": " << help << std::endl;
        return 1;
    }
    fun();
    return 0;
}

// vim: set ts=4 sw=4 et :
