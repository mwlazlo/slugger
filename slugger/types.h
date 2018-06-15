#ifndef SLUGGER_TYPES_H
#define SLUGGER_TYPES_H

namespace slugger {

/**
  * Enumerate the various logging levels
  */
typedef enum {
    debug_t,
    info_t,
    notice_t,
    warning_t,
    err_t,
    crit_t,
    alert_t,
    emerg_t
} logging_level;

/**
  * Enumerate the various syslog facilities (the default is local0)
  */
typedef enum {
    auth_t,
    cron_t,
    daemon_t,
    kern_t,
    local0_t,
    local1_t,
    local2_t,
    local3_t,
    local4_t,
    local5_t,
    local6_t,
    local7_t,
    lpr_t,
    mail_t,
    news_t,
    syslog_t,
    user_t,
    uucp_t
} syslog_facility;

}; // namespace slugger

#endif
// vim: set ts=4 sw=4 et :
