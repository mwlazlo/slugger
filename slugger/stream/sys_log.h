#ifndef SLUGGER_SYSLOG_H
#define SLUGGER_SYSLOG_H

#include <slugger/log.h>

namespace slugger { namespace stream {

/**
  * The sys_log class is used to represent a loging stream
  * which is directed into the system log (sys_log(8)) facility.
  */
class sys_log:
    public destination
{
public:
    /**
      * The destructor.
      */
    virtual ~sys_log();

private:
    /**
      * The default constructor.
      * It is private on purpose, use the #create class method instead.
      */
    sys_log();

public:
    /**
      * The create class method is used to create new dynmically
      * allocated instanced of this class.
      */
    static pointer create();

protected:
    // See base class for documentation.
    void message(logging_level level, const std::string &txt);

private:
    /**
      * The is_open instance variable is used to remember whether or not
      * the openlog() function has been called to start logging (and
      * open the connection to syslogd).
      */
    bool is_open;

    /**
      * The copy constructor.  Do not use.
      */
    sys_log(const sys_log &);

    /**
      * The assignment operator.  Do not use.
      */
    sys_log &operator=(const sys_log &);
};

}} // namespace slugger::stream

#endif // SLUGGER_SYSLOG_H
// vim: set ts=4 sw=4 et :
