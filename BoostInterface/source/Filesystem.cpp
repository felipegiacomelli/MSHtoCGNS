#include <BoostInterface/Filesystem.hpp>

void createDirectory(const std::string& Path) {
	if (!boost::filesystem::exists(Path)) {
		boost::filesystem::path Directory(Path);
		boost::filesystem::create_directory(Directory);
	}
}

void deleteDirectory(const std::string& Path) {
	if (boost::filesystem::exists(Path)) {
		boost::filesystem::path Directory(Path);
		boost::filesystem::remove_all(Directory);
	}
}