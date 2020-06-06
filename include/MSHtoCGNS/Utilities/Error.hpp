#ifndef __UTILITIES_ERROR_HPP__
#define __UTILITIES_ERROR_HPP__

#include <string>

std::string error(const char* prettyFunction, std::string message, const char* cgError);
std::string error(const char* prettyFunction, std::string message);

#endif
