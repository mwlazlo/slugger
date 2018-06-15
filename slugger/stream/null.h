#ifndef SLUGGER_NULL_H
#define SLUGGER_NULL_H

#include <slugger/log.h>

namespace slugger { namespace stream {

/**
  * The null class is used to represent a logging destination
  * that throws everything away.
  */
class null:
    public destination
{
public:
    /**
      * The destructor.
      */
    virtual ~null();

private:
    /**
      * The default constructor.
      * It is private on purpose, use the #create class method instead.
      */
    null();

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
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    null(const null &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The rights hand side of the assignment.
      */
    null &operator=(const null &rhs);
};

}} // namespace slugger::stream

#endif // SLUGGER_NULL_H
// vim: set ts=4 sw=4 et :
