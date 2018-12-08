#include "BoostInterface/Filesystem.hpp"

void createDirectory(std::string path) {
    if (!boost::filesystem::exists(path)) {
        boost::filesystem::path directory(path);
        boost::filesystem::create_directory(directory);
    }
}

void createDirectories(std::string path) {
    if (!boost::filesystem::exists(path)) {
        boost::filesystem::path directory(path);
        boost::filesystem::create_directories(directory);
    }
}

void deleteDirectory(std::string path) {
    if (boost::filesystem::exists(path)) {
        boost::filesystem::path directory(path);
        boost::filesystem::remove_all(directory);
    }
}
