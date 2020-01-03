#include "logger.hpp"


CLogger* CLogger::instance;
std::string CLogger::file_name = LOG_DEFAULT_FILE_NAME;
log4cpp::Priority::Value CLogger::priority = log4cpp::Priority::INFO;


CLogger::CLogger()
{
    auto pl = new log4cpp::PatternLayout();
    pl->setConversionPattern("%d{%Y-%m-%d %H:%M:%S:%l} %R %p %c : %m %n");

    appender_console = new log4cpp::OstreamAppender("console", &std::cout);
    appender_console->setLayout(pl);


    appender_file = new log4cpp::FileAppender("default", file_name);
    appender_file->setLayout(pl);

    log4cpp::Category& root = log4cpp::Category::getRoot();
    root.setPriority(priority);
    root.addAppender(appender_file);
    root.addAppender(appender_console);
}


CLogger::~CLogger()
{
    // empty on purpose
}


CLogger* CLogger::get_instance()
{
    if(instance == nullptr)
    {
        instance = new CLogger();
    }

    return instance;
}


void CLogger::set_file_name(std::string file_name)
{
    CLogger::file_name = file_name;
}


void CLogger::set_priority(std::string priority)
{
    std::transform(priority.begin(), priority.end(), priority.begin(), ::tolower);

    if(priority == "emerg")
    {
        CLogger::priority = log4cpp::Priority::EMERG;
    }
    else if(priority == "fatal")
    {
        CLogger::priority = log4cpp::Priority::EMERG;
    }
    else if(priority == "alert")
    {
        CLogger::priority = log4cpp::Priority::ALERT;
    }
    else if(priority == "crit")
    {
        CLogger::priority = log4cpp::Priority::CRIT;
    }
    else if(priority == "error")
    {
         CLogger::priority = log4cpp::Priority::ERROR;
    }
    else if(priority == "warn")
    {
         CLogger::priority = log4cpp::Priority::WARN;
    }
    else if(priority == "notice")
    {
         CLogger::priority = log4cpp::Priority::NOTICE;
    }
    else if(priority == "info")
    {
         CLogger::priority = log4cpp::Priority::INFO;
    }
    else if(priority == "debug")
    {
         CLogger::priority = log4cpp::Priority::DEBUG;
    }
    else if(priority == "notset")
    {
         CLogger::priority = log4cpp::Priority::NOTSET;
    }
}


void CLogger::set_priority(log4cpp::Priority::Value priority)
{
    CLogger::priority = priority;
}


int CLogger::log(log4cpp::Priority::Value priority, std::string name, std::string message)
{
    log4cpp::Category& root = log4cpp::Category::getInstance(std::string(name));
    root << priority<< message;

    return 0;
}


int CLogger::log_debug(std::string name, std::string message)
{
    return log(log4cpp::Priority::DEBUG, name, message);
}


int CLogger::log_info(std::string name, std::string message)
{
    return log(log4cpp::Priority::INFO, name, message);
}


int CLogger::log_warning(std::string name, std::string message)
{
    return log(log4cpp::Priority::WARN, name, message);
}


int CLogger::log_error(std::string name, std::string message)
{
    return log(log4cpp::Priority::ERROR, name, message);
}
