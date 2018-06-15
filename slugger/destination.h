#ifndef SLUGGER_DESTINATION_H
#define SLUGGER_DESTINATION_H

#include <slugger/types.h>

#include <string>


namespace slugger {

// Forward declaration of stream::tee, see comment about friend at end of class.
namespace stream {
class tee;
}

/**
  * The destination class is used to represent an abstract logging destination.
  * It could be a log file, or it could be syslog, or it could be the standard
  * error stream.
  *
  * Usage: If you do not instantiate a destination instance, a stderr instance
  * will be created at run time.
  *
  * If you want something other than the standard error stream, use the
  * log::register_stream function to direct the output elsewhere.
  *
  *     log::register_stream(log::stream::file::create("application.log"));
  *
  * Note that this must be done very early in your program's lifetime.
  */
class destination
{
public:
    typedef destination *pointer;

    /**
      * The destructor.
      */
    virtual ~destination();

protected:
    /**
      * The default constructor.
      *
      * Notice that this constructor is protected.  This is because you must
      * derive from this class in order to create a concrete logger target.
      */
    destination();

protected:

    /**
      * The message method is used to actually deliver a log message to the
      * logger medium. All formatting has been completed, the text requires no
      * further substitutions.
      *
      * @param level
      *     The level of this message, such as "error", "warning", etc.
      * @param txt
      *     The line of text to be printed.
      */
    virtual void message(logging_level level, const std::string &txt) = 0;

private:
    /**
      * The copy constructor.  Do not use.
      */
    destination(const destination &);

    /**
      * The assignment operator.  Do not use.
      */
    destination &operator=(const destination &);

    /**
      * Allow basic_debugbuf access to the protected message() method.
      */
    friend class basic_debugbuf;

    /**
      * This is so that stream::tee can call message() directly, a
      * minor efficiency consideration.
      */
    friend class stream::tee;
};

} // namespace slugger

#endif
// vim: set ts=4 sw=4 et :
