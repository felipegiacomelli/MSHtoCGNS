#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <iostream>

#include <boost/filesystem.hpp>

bool exists(const std::string&);
void createDirectory(const std::string&);
void deleteDirectory(const std::string&);

#endif