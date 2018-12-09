#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <iostream>

#include <boost/filesystem.hpp>

void createDirectory(std::string path);
void createDirectories(std::string path);
void deleteDirectory(std::string path);

#endif
