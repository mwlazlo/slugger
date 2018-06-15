#ifndef SLUGGER_STDOUT_H
#define SLUGGER_STDOUT_H

#include <slugger/log.h>
#include <pthread.h>

namespace slugger { namespace stream {

/**
  * The std_out class is used to represent an error logging destination which
  * uses the standard error stream to deliver messages.
  */
class std_out:
    public destination
{
public:
    /**
      * The destructor.
      */
    virtual ~std_out();

private:
    /**
      * The default constructor.
      * It is private on purpose, use the #create class method instead.
      */
    std_out();

public:
    /**
      * The create class method is used to create new dynmically
      * allocated instanced of this class.
      */
    static pointer create();

    // See base class for documentation.
    void message(logging_level level, const std::string &txt);

private:
    /**
      * The copy constructor.  Do not use.
      */
    std_out(const std_out &);

    /**
      * The assignment operator.  Do not use.
      */
    std_out &operator=(const std_out &);

    /**
      * Locking is required for multi-threaded applications
      */
    pthread_mutex_t mux;
};

}} // namespace slugger::stream

#endif // SLUGGER_STDOUT_H
// vim: set ts=4 sw=4 et :
