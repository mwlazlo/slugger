#ifndef SLUGGER_STDERR_H
#define SLUGGER_STDERR_H

#include <slugger/log.h>
#include <pthread.h>

namespace slugger { namespace stream {

/**
  * The std_err class is used to represent an error logging destination which
  * uses the standard error stream to deliver messages.
  */
class std_err:
    public destination
{
public:
    /**
      * The destructor.
      */
    virtual ~std_err();

private:
    /**
      * The default constructor.
      * It is private on purpose, use the #create class method instead.
      */
    std_err();

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
    std_err(const std_err &);

    /**
      * The assignment operator.  Do not use.
      */
    std_err &operator=(const std_err &);

    /**
      * Locking is required for multi-threaded applications
      */
    pthread_mutex_t mux;
};

}} // namespace slugger::stream

#endif // SLUGGER_STDERR_H
// vim: set ts=4 sw=4 et :
