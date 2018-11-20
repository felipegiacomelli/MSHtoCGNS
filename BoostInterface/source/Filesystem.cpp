#include <BoostInterface/Filesystem.hpp>

void createDirectory(std::string name) {
    if (!boost::filesystem::exists(name)) {
        boost::filesystem::path directory(name);
        boost::filesystem::create_directory(directory);
    }
}

void deleteDirectory(std::string name) {
    if (boost::filesystem::exists(name)) {
        boost::filesystem::path directory(name);
        boost::filesystem::remove_all(directory);
    }
}
