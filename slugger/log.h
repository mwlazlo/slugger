#ifndef SLUGGER_LOG_H
#define SLUGGER_LOG_H

#include <slugger/types.h>
#include <slugger/destination.h>
#include <iostream>

// enable per thread logging levels (disabled due to efficiency concerns)
#undef PER_THREAD_LEVEL

namespace slugger {


/**
 * User defined prefix stream operator. Useful for multi-threaded apps.
 */
typedef const std::string& (*stream_prefix_func)();
void set_stream_prefix(stream_prefix_func);
stream_prefix_func get_stream_prefix();

const char* level_to_string(logging_level level);
logging_level string_to_level(const std::string&);

std::istream& operator>>(std::istream& i, logging_level& l);
std::ostream& operator<<(std::ostream& o, const logging_level& ll);


/**
* The register_stream method is used to redirect output into a different
* logger stream.
*/
void register_stream(const destination::pointer ls);

/**
* The set_facility method is used to set the (global) syslog(3) facility.
*/
void set_facility(syslog_facility fac);
syslog_facility get_facility();

/**
* The set_program_name method is used to set the program name used by
* syslog(3)
*/
void set_program_name(const std::string &program_name);
const std::string& get_program_name();

/**
 * The set_logging_level_default method is used to control which message levels will be
 * sent to a logger stream target.
*/
void set_logging_level_default(logging_level level);

/**
 * The set_logging_level method is used to control which message levels will be
 * sent to a logger stream target.
*/
void set_logging_level(logging_level level);

/**
 * The get_logging_level method is used to return the logging level configured
 * by this thread. If no level is set for this thread, the default is used.
 */
logging_level get_logging_level();

/**
  * The get_singleton class method is used to obtain a reference to
  * the sole logger instance for the system.
  */
destination &get_singleton();

} // namespace slugger

#endif
// vim: set ts=4 sw=4 et :
