#include "Logger.hpp"

Logger::Logger() : _DEBUG(false)
{
}

Logger::~Logger()
{
}

void Logger::setDebug(bool value)
{
    this->_DEBUG = value;
}

void Logger::log(std::string type, std::string msg)
{
    if (_DEBUG && type == "DEBUG")
        std::cout << GREEN << "[" << type << "] " << RESET << msg << std::endl;
    else if (type == "ERROR")
    {
        std::cerr << RED << "[" << type << "] " << RESET << msg << std::endl;
        exit(1);
    }
    else if (type == "WARNING")
        std::cerr << YELLOW << "[" << type << "] " << RESET << msg << std::endl;
    else if (type == "INFO")
        std::cout << MAGENTA << "[" << type << "] " << RESET << msg << std::endl;
}
