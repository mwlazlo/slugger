# slugger
An easy to use, streams-based, C++ logging library

## Installation

```sh
$ make DESTDIR=/usr/local install
```

## Available streams:

1. null
2. file
3. std_out
4. std_err
5. sys_log
6. tee - for sending output to 2 streams

## How to use

### Initialising

```c++

// setup some shorthand
namespace lg = slugger;

static void
initialise_logging(bool debug = true)
{
    // set default stream
    lg::destination::pointer dest = lg::stream::sys_log::create();
    lg::logging_level level = lg::info_t;

    // switch to use syslog if we're running in production
    if (!debug)
    {
        dest = lg::stream::std_err::create();
        level = lg::debug_t;
    }

    lg::set_program_name("example");
    lg::set_facility(lg::local0_t);
    lg::set_logging_level(level)
    lg::register_stream(dest);
}

int main(int argc, char **argv) 
{
    bool debug;

    // scan argv for --debug flag

    initialise_logging(debug)

    lg::info << "Logging initialised" << std::endl;
    lg::debug << "Some debugging" << std::endl;
}

```


### Logging structs or classes

```cpp
struct type_t
{
    std::string name;
    int count;
    std::list<string> things;
};

std::ostream&
operator<<(std::ostream& ostr, const type_t& t)
{
    ostr << "type_t { ";
    ostr << "name=" << t.name << ", ";
    ostr << "count=" << t.count << ", ";
    ostr << "things=[";
    std::copy(t.things.begin(), t.things.end(), std::ostream_iterator(ostr, " "));
    ostr << "]}";
    return ostr;
}

// later on...

    type_t value;

...
    lg::info << "Value of type: " << value << std::endl;
```

<!-- vim: set ts=4 sw=4 et : -->
