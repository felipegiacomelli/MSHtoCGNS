#include <IO/CgnsReader.hpp>

CgnsReader::CgnsReader(const std::string& filePath) {
	this->filePath = filePath;
	this->checkFile();
	this->buffer = new char[800];
	this->zoneSizes.resize(3);
	this->readBase();
	this->readZone();
	this->readDimension();
}

void CgnsReader::checkFile() {
	if (!boost::filesystem::exists(this->filePath)) throw std::runtime_error("There is no .cgns file in the given path");
	if (cg_open(this->filePath.c_str(), CG_MODE_READ, &this->cgnsFile)) {
		std::cerr << "\nThere is a problem opening the file " << this->filePath << std::endl; 
		cg_error_exit();
	}
}

void CgnsReader::readBase() {
	if (cg_nbases(this->cgnsFile, &this->cgnsBase)) {
		std::cerr << "\nThere is no base" << std::endl; 
		cg_error_exit();
	}
	if (this->cgnsBase != 1) { 
		std::cerr << "\nMore than one base is not supported" << std::endl; 
		cg_error_exit(); 
	}
}

void CgnsReader::readZone() {
	if (cg_nzones(this->cgnsFile, this->cgnsBase, &this->cgnsZone)) { 
		std::cerr << "\nThere is no zone" << std::endl; 
		cg_error_exit(); 
	}
	if (this->cgnsZone != 1) { 
		std::cout << "\nMore than one zone is not supported" << std::endl; 
		cg_error_exit(); 
	}
	if (cg_zone_type(this->cgnsFile, this->cgnsBase, this->cgnsZone, &this->zoneType)) {
		std::cerr << "\nError reading zone type" << std::endl;	
		cg_error_exit();
	}
	if (this->zoneType != Unstructured) { 
		std::cout << "\nUnstructured zone expected" << std::endl; 
		cg_error_exit(); 
	}
	if (cg_zone_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, this->buffer, &this->zoneSizes[0])) {
		std::cerr << "\nThere was a problem when reading zone" << std::endl;	
		cg_error_exit();
	}
}

void CgnsReader::readSection() {
	if (cg_nsections(this->cgnsFile, this->cgnsBase, this->cgnsZone, &this->numberOfSections)) {
		std::cerr << "\nThere was a problem when reading sections" << std::endl;	
		cg_error_exit();
	}
}

void CgnsReader::readDimension() {
	if (cg_base_read(this->cgnsFile, this->cgnsBase, this->buffer, &this->cellDimension, &this->physicalDimension)) {
		std::cerr << "\nThere was a problem when reading base" << std::endl;	
		cg_error_exit();
	}
}


GridData CgnsReader::getGridData() const {
	return this->gridData;
}

CgnsReader::~CgnsReader() {
	cg_close(this->cgnsFile);
	delete this->buffer;
}
