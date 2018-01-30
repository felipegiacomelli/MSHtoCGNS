#include <IO/CgnsReader.hpp>

CgnsReader::CgnsReader(const std::string& filePath) {
	this->filePath = filePath;
	this->checkFile();
	this->buffer = new char[800];
	this->zoneSizes.resize(3);
	this->readBase();
	this->readZone();
	this->readSection();
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
		throw std::runtime_error("The CGNS file has no base");
		cg_error_exit();
	}
	if (this->cgnsBase != 1) { 
		throw std::runtime_error("The CGNS file has more than one base");
		cg_error_exit(); 
	}
	if (cg_base_read(this->cgnsFile, this->cgnsBase, this->buffer, &this->cellDimension, &this->physicalDimension)) {
		throw std::runtime_error("Could not read base");
		cg_error_exit();
	}
}

void CgnsReader::readZone() {
	if (cg_nzones(this->cgnsFile, this->cgnsBase, &this->cgnsZone)) { 
		throw std::runtime_error("The CGNS file has no zone");
		cg_error_exit(); 
	}
	if (this->cgnsZone != 1) { 
		throw std::runtime_error("The CGNS file has more than one zone");
		cg_error_exit(); 
	}
	if (cg_zone_type(this->cgnsFile, this->cgnsBase, this->cgnsZone, &this->zoneType)) {
		throw std::runtime_error("Could not read zone type");
		cg_error_exit();
	}
	if (this->zoneType != Unstructured) { 
		throw std::runtime_error("Only unstructured zones are supported");
		cg_error_exit(); 
	}
	if (cg_zone_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, this->buffer, &this->zoneSizes[0])) {
		throw std::runtime_error("Could not read zone");
		cg_error_exit();
	}
}

void CgnsReader::readSection() {
	if (cg_nsections(this->cgnsFile, this->cgnsBase, this->cgnsZone, &this->numberOfSections)) {
		throw std::runtime_error("Could not read number of sections");
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
