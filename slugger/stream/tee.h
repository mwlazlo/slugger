#ifndef SLUGGER_TEE_H
#define SLUGGER_TEE_H

#include <slugger/log.h>

namespace slugger { namespace stream {


/**
  * The tee class is used to represent a logging stream which
  * can deliver the messages to two destinations simultaneously.
  */
class tee:
    public destination
{
public:
    /**
      * The destructor.
      */
    virtual ~tee();

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param lhs
      *     The first sub-log to write.
      * @param rhs
      *     The second sub-log to write.
      */
    tee(const pointer &lhs, const pointer &rhs);

public:
    /**
      * The create class method is used to create new dynmically
      * allocated instanced of this class.
      *
      * @param lhs
      *     The first sub-log to write.
      * @param rhs
      *     The second sub-log to write.
      */
    static pointer create(const pointer &lhs, const pointer &rhs);

protected:
    // See base class for documentation.
    void message(logging_level level, const std::string &txt);

private:
    /**
      * The lhs instance variable is used to remember the first logging
      * stream.
      */
    pointer lhs;

    /**
      * The rhs instance variable is used to remember the second logging
      * stream.
      */
    pointer rhs;

    /**
      * The default constructor.  Do not use.
      */
    tee();

    /**
      * The copy constructor.  Do not use.
      */
    tee(const tee &);

    /**
      * The assignment operator.  Do not use.
      */
    tee &operator=(const tee &);
};

}} // namespace slugger::stream

#endif // SLUGGER_TEE_H
// vim: set ts=4 sw=4 et :
