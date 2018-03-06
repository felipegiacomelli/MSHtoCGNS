#include <IO/CgnsReader.hpp>

CgnsReader::CgnsReader(const std::string& filePath) : 
	filePath(filePath), buffer(new char[800]), zoneSizes(std::vector<cgsize_t>(3)), gridData(MakeShared<GridData>()) {
	this->checkFile();
	this->readBase();
	this->readZone();
	this->readNumberOfSections();
	this->readNumberOfBoundaries();
}

void CgnsReader::checkFile() {
    boost::filesystem::path input(this->filePath);
	if (!boost::filesystem::exists(input.parent_path())) { 
		throw std::runtime_error("CgnsReader: The parent path does not exist");
	}
	if (!boost::filesystem::exists(this->filePath)) { 
		throw std::runtime_error("CgnsReader: There is no file in the given path");
	}
	if (input.extension() != ".cgns") { 
		throw std::runtime_error("CgnsReader: The file extension is not .cgns");
	}
	if (cg_open(this->filePath.c_str(), CG_MODE_READ, &this->cgnsFile)) { 
		throw std::runtime_error("CgnsReader: Could not open the the file " + this->filePath);		
	}
}

void CgnsReader::readBase() {
	if (cg_nbases(this->cgnsFile, &this->cgnsBase)) {
		throw std::runtime_error("CgnsReader: The CGNS file has no base");
	}
	if (this->cgnsBase != 1) {
		throw std::runtime_error("CgnsReader: The CGNS file has more than one base");
	}
	if (cg_base_read(this->cgnsFile, this->cgnsBase, this->buffer, &this->cellDimension, &this->physicalDimension)) {
		throw std::runtime_error("CgnsReader: Could not read base");		
	}
}

void CgnsReader::readZone() {
	if (cg_nzones(this->cgnsFile, this->cgnsBase, &this->cgnsZone)) {
		throw std::runtime_error("CgnsReader: The CGNS file has no zone"); 
	}
	if (this->cgnsZone != 1) {
		throw std::runtime_error("CgnsReader: The CGNS file has more than one zone"); 
	}
	if (cg_zone_type(this->cgnsFile, this->cgnsBase, this->cgnsZone, &this->zoneType)) {
		throw std::runtime_error("CgnsReader: Could not read zone type");
	}
	if (this->zoneType != Unstructured) {
		throw std::runtime_error("CgnsReader: Only unstructured zones are supported"); 
	}
	if (cg_zone_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, this->buffer, &this->zoneSizes[0])) {
		throw std::runtime_error("CgnsReader: Could not read zone");
	}
}

void CgnsReader::readNumberOfSections() {
	int numberOfSections;
	if (cg_nsections(this->cgnsFile, this->cgnsBase, this->cgnsZone, &numberOfSections)) { 
		throw std::runtime_error("CgnsReader: Could not read number of sections");
	}
	this->sectionIndices = std::vector<int>(numberOfSections);
	std::iota(this->sectionIndices.begin(), this->sectionIndices.end(), 1);
}

void CgnsReader::readNumberOfBoundaries() {
	int numberOfBoundaries;
	if (cg_nbocos(this->cgnsFile, this->cgnsBase, this->cgnsZone, &numberOfBoundaries)) {
		throw std::runtime_error("CgnsReader: Could not read number of boundaries");
	}
	this->boundaryIndices = std::vector<int>(numberOfBoundaries);
	std::iota(this->boundaryIndices.begin(), this->boundaryIndices.end(), 1);
}

void CgnsReader::readBoundaries() {
	if (this->boundaryIndices.size() != this->gridData->boundaries.size()) {
		throw std::runtime_error("CgnsReader: mismatch between number of boundary conditions and boundary connectivities");
	}
	for (auto boundary = this->boundaryIndices.cbegin(); boundary != this->boundaryIndices.cend(); boundary++) {
		BCType_t bocotype;
		PointSetType_t ptset_type;
		cgsize_t numberOfVertices, NormalListSize;
		int NormalIndex, ndataset;
		DataType_t NormalDataType;
		if (cg_boco_info(this->cgnsFile, this->cgnsBase, this->cgnsZone, *boundary, this->buffer, &bocotype, &ptset_type, &numberOfVertices, &NormalIndex, &NormalListSize, &NormalDataType, &ndataset)) {
			throw std::runtime_error("CgnsReader: Could not read boundary information");
		}
		std::vector<cgsize_t> vertices(numberOfVertices);
		if (cg_boco_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *boundary, &vertices[0], nullptr)) {
			throw std::runtime_error("CgnsReader: Could not read boundary");
		}
		std::transform(vertices.begin(), vertices.end(), vertices.begin(), [](const cgsize_t& x){return x - 1;});
		this->gridData->boundaries[*boundary - 1].vertices = std::move(vertices);
	}
}

GridDataShared CgnsReader::getGridData() const {
	return this->gridData;
}

CgnsReader::~CgnsReader() {
	cg_close(this->cgnsFile);
	delete this->buffer;
}