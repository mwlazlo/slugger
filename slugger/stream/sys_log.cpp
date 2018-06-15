#include <cstring>
#include <syslog.h>

#include <slugger/stream/sys_log.h>

using namespace slugger;
using namespace slugger::stream;

sys_log::~sys_log()
{
    if(is_open) {
        closelog();
        is_open = false;
    }
}


sys_log::sys_log() :
    destination(),
    is_open(false)
{
}


destination::pointer
sys_log::create()
{
    return pointer(new sys_log());
}


static int
syslog_level(logging_level level)
{
    switch(level) {
        case debug_t:
            return LOG_DEBUG;
        case info_t:
            return LOG_INFO;
        case notice_t:
            return LOG_NOTICE;
        case warning_t:
            return LOG_WARNING;
        case err_t:
            return LOG_ERR;
        case crit_t:
            return LOG_CRIT;
        case alert_t:
            return LOG_ALERT;
        case emerg_t:
            return LOG_EMERG;
        default:
            return LOG_ERR;
    }
}


static int
get_syslog_facility(syslog_facility facility)
{
    switch(facility) {
        case auth_t:
            return LOG_AUTH;
        case cron_t:
            return LOG_CRON;
        case daemon_t:
            return LOG_DAEMON;
        case kern_t:
            return LOG_KERN;
        case local0_t:
            return LOG_LOCAL0;
        case local1_t:
            return LOG_LOCAL1;
        case local2_t:
            return LOG_LOCAL2;
        case local3_t:
            return LOG_LOCAL3;
        case local4_t:
            return LOG_LOCAL4;
        case local5_t:
            return LOG_LOCAL5;
        case local6_t:
            return LOG_LOCAL6;
        case local7_t:
            return LOG_LOCAL7;
        case lpr_t:
            return LOG_LPR;
        case mail_t:
            return LOG_MAIL;
        case news_t:
            return LOG_NEWS;
        case syslog_t:
            return LOG_SYSLOG;
        case user_t:
            return LOG_USER;
        case uucp_t:
            return LOG_UUCP;
        default:
            return LOG_LOCAL0;
    }
}


void
sys_log::message(logging_level level, const std::string &txt)
{
    int sfacility = get_syslog_facility(get_facility());

    if(!is_open) {
        int option = LOG_PID;
        openlog(get_program_name().c_str(), option, sfacility);
    }
    int slevel = syslog_level(level);
    int priority = sfacility | slevel;
    syslog(priority, "%s", txt.c_str());
}

// vim: set ts=4 sw=4 et :
