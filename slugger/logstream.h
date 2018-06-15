#ifndef SLUGGER_LOGSTREAM_H
#define SLUGGER_LOGSTREAM_H

#include <map>
#include <sstream>

#include <pthread.h>

#include <slugger/slugger.h>

namespace slugger {


/**
  * Overload stl's streambuf class to act as a buffer for our logging
  * streams. Writes to destination on flush (usually std::endl).
  */
class basic_debugbuf : public std::basic_streambuf<char>
{
private:
    /**
      * The (thread specific) output buffer
      */
    std::map<pthread_t, std::string> buffer_data;
    pthread_mutex_t buffer_mux;

    /** 
      * The associated logging level
      */
    logging_level level;

public:
    /**
      * The constructor
      */
    basic_debugbuf(logging_level l);
    virtual ~basic_debugbuf();

protected:
    /**
      * Write character in the case of overflow
      * 
      * For the purpose of the streambuf class, overflows happen when a new
      * character is to be written at the put pointer pptr position, but this
      * has reached the end pointer epptr, indicating that apparently no room
      * is currently available at the internal output array.
      *
      * This function is expected either to modify the pbase, pptr and epptr
      * pointers that define the internal output array in such a way that room
      * is made available for more characters, or otherwise fail. It is also
      * responsibility of the function to write the character passed as
      * argument.
      *
      * The specific behavior depends on each derived class, but it normally
      * attempts to write some of the characters to the controlled output
      * sequence to make room for more characters in the internal output array.
      * Its default behavior in streambuf is to do nothing and return EOF (or
      * traits::eof() for other traits), although it is overloaded in 
      *
      * http://www.cplusplus.com/reference/iostream/streambuf/overflow/
      */
    int overflow(int c);

    int sync();

    std::streamsize xsputn(const char* s, std::streamsize n);
};

/**
  * Derive std::ostream and connect to our basic_debugbuf class.
  */
class logstream : public std::ostream
{
public:
    logstream(basic_debugbuf* buf);
    virtual ~logstream();
};


/**
 * typedef streams to allow selective overloading of operator<< based on
 * logging level
 */

typedef logstream debugstream;
typedef logstream infostream;
typedef logstream noticestream;
typedef logstream warningstream;
typedef logstream errstream;
typedef logstream critstream;
typedef logstream alertstream;
typedef logstream emergstream;

/**
  * The actual logging stream objects
  */
extern debugstream debug;
extern infostream info;
extern noticestream notice;
extern warningstream warning;
extern errstream err;
extern critstream crit;
extern alertstream alert;
extern emergstream emerg;


} // namespace slugger

#endif
// vim: set ts=4 sw=4 et :
