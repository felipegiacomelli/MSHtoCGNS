#include "BoostInterface/Filesystem.hpp"

bool exists(const std::string& Path) {
	if (boost::filesystem::exists(Path)) {
		return true;
	}
	else {
		return false;
	}	
}

void createDirectory(const std::string& Path) {
	if (!exists(Path)) {		
		boost::filesystem::path Directory(Path);
		boost::filesystem::create_directory(Directory);
	}	
}

void deleteDirectory(const std::string& Path) {
	if (exists(Path)) {		
		boost::filesystem::path Directory(Path);
		boost::filesystem::remove_all(Directory);
	}	
}
