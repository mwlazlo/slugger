#ifndef SLUGGER_FILE_H
#define SLUGGER_FILE_H

#include <cstdio>
#include <ctime>
#include <string>
#include <fstream>
#include <time.h>

#include <slugger/log.h>

#include <pthread.h>

namespace slugger { namespace stream {

/**
  * The file class is used to represent a logging destination
  * which is a file.
  *
  * The file will be closed and reopened periodically to facilitate
  * logrotate(8) and friends.
  */
class file: 
    public destination
{
public:
    /**
      * The destructor.
      */
    virtual ~file();

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param filename
      *     the name of the file which is to receive the logging data.
      */
    file(const std::string &filename);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     the name of the file which is to receive the logging data.
      */
    static pointer create(const std::string &filename);

protected:
    // See base class fopr documentation.
    void message(logging_level level, const std::string &txt);

private:
    /**
      * The filename isntance variable is sued to rememeber the name of
      * the file which is to receive the logging data.
      */
    std::string filename;

    /**
      * The vfp instance variable is used to remember the open file. 
      */
    std::ofstream fp;

    /**
      * The since instance variable is used to remember the time we last
      * opened the file.  This is used when deciding whether or not to
      * close and reopen the file, to make logrotate(8) easy.
      */
    time_t since;

    /**
      * The default constructor.  Do not use.
      */
    file();

    /**
      * The copy constructor.  Do not use.
      */
    file(const file &);

    /**
      * The assignment operator.  Do not use.
      */
    file &operator=(const file &);

    /**
      * Locking is required for multi-threaded applications
      */
    pthread_mutex_t mux;
};

}} // namespace slugger::stream

#endif // SLUGGER_FILE_H
// vim: set ts=4 sw=4 et :
