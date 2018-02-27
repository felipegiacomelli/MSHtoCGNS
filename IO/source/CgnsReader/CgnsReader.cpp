#include <IO/CgnsReader.hpp>

CgnsReader::CgnsReader(const std::string& filePath) : filePath(filePath), buffer(new char[800]), zoneSizes(std::vector<cgsize_t>(3)), gridData(MakeShared<GridData>()) {
	this->checkFile();
	this->readBase();
	this->readZone();
	this->readNumberOfSections();
}

void CgnsReader::checkFile() {
    boost::filesystem::path input(this->filePath);
	if (!boost::filesystem::exists(input.parent_path())) throw std::runtime_error("CgnsReader: The parent path does not exist");
	if (!boost::filesystem::exists(this->filePath)) throw std::runtime_error("There is no file in the given path");
	if (input.extension() != ".cgns") throw std::runtime_error("CgnsReader: The file extension is not .cgns");
	if (cg_open(this->filePath.c_str(), CG_MODE_READ, &this->cgnsFile)) {
		std::cerr << std::endl << "Could not open the the file " << this->filePath << std::endl; 
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

void CgnsReader::readNumberOfSections() {
	int numberOfSections;
	if (cg_nsections(this->cgnsFile, this->cgnsBase, this->cgnsZone, &numberOfSections)) {
		throw std::runtime_error("Could not read number of sections");
		cg_error_exit();
	}
	this->sectionIndices = std::vector<int>(numberOfSections);
	std::iota(this->sectionIndices.begin(), this->sectionIndices.end(), 1);
}

GridDataShared CgnsReader::getGridData() const {
	return this->gridData;
}

CgnsReader::~CgnsReader() {
	cg_close(this->cgnsFile);
	delete this->buffer;
}
