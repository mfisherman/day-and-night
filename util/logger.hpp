#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#define LOG_DEFAULT_FILE_NAME "program.log"

#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/PatternLayout.hh"

#include <algorithm>
#include <string>

class CLogger
{
public:
    ~CLogger();

    static CLogger* get_instance();
    static void set_file_name(std::string file_name);
    static void set_priority(std::string priority);
    static void set_priority(log4cpp::Priority::Value priority);

    int log(log4cpp::Priority::Value priority, std::string name, std::string message);
    int log_debug(std::string name, std::string message);
    int log_info(std::string name, std::string message);
    int log_error(std::string name, std::string message);
    int log_warning(std::string name, std::string message);

private:
    CLogger();

    static CLogger* instance;
    static std::string file_name;
    static log4cpp::Priority::Value priority;

    log4cpp::Appender* appender_file;
    log4cpp::Appender* appender_console;
};

#endif
