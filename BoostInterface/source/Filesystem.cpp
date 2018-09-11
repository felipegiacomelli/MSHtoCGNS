#include <BoostInterface/Filesystem.hpp>

void createDirectory(std::string Path) {
	if (!boost::filesystem::exists(Path)) {
		boost::filesystem::path Directory(Path);
		boost::filesystem::create_directory(Directory);
	}
}

void deleteDirectory(std::string Path) {
	if (boost::filesystem::exists(Path)) {
		boost::filesystem::path Directory(Path);
		boost::filesystem::remove_all(Directory);
	}
}