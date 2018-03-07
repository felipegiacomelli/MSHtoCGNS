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
	if (cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex)) { 
		throw std::runtime_error("CgnsReader: Could not open the file " + this->filePath);		
	}
}

void CgnsReader::readBase() {
	if (cg_nbases(this->fileIndex, &this->baseIndex)) {
		throw std::runtime_error("CgnsReader: Could not read number of bases");
	}
	if (this->baseIndex != 1) {
		throw std::runtime_error("CgnsReader: The CGNS file has more than one base");
	}
	if (cg_base_read(this->fileIndex, this->baseIndex, this->buffer, &this->cellDimension, &this->physicalDimension)) {
		throw std::runtime_error("CgnsReader: Could not read base");		
	}
}

void CgnsReader::readZone() {
	if (cg_nzones(this->fileIndex, this->baseIndex, &this->zoneIndex)) {
		throw std::runtime_error("CgnsReader: Could not read number of zones"); 
	}
	if (this->zoneIndex != 1) {
		throw std::runtime_error("CgnsReader: The CGNS file has more than one zone"); 
	}
	if (cg_zone_type(this->fileIndex, this->baseIndex, this->zoneIndex, &this->zoneType)) {
		throw std::runtime_error("CgnsReader: Could not read zone type");
	}
	if (this->zoneType != Unstructured) {
		throw std::runtime_error("CgnsReader: Only unstructured zones are supported"); 
	}
	if (cg_zone_read(this->fileIndex, this->baseIndex, this->zoneIndex, this->buffer, &this->zoneSizes[0])) {
		throw std::runtime_error("CgnsReader: Could not read zone");
	}
}

void CgnsReader::readNumberOfSections() {
	int numberOfSections;
	if (cg_nsections(this->fileIndex, this->baseIndex, this->zoneIndex, &numberOfSections)) { 
		throw std::runtime_error("CgnsReader: Could not read number of sections");
	}
	this->sectionIndices = std::vector<int>(numberOfSections);
	std::iota(this->sectionIndices.begin(), this->sectionIndices.end(), 1);
}

void CgnsReader::readNumberOfBoundaries() {
	int numberOfBoundaries;
	if (cg_nbocos(this->fileIndex, this->baseIndex, this->zoneIndex, &numberOfBoundaries)) {
		throw std::runtime_error("CgnsReader: Could not read number of boundaries");
	}
	this->boundaryIndices = std::vector<int>(numberOfBoundaries);
	std::iota(this->boundaryIndices.begin(), this->boundaryIndices.end(), 1);
}

void CgnsReader::readBoundaries() {
	if (this->boundaryIndices.size() != this->gridData->boundaries.size()) {
		throw std::runtime_error("CgnsReader: mismatch between number of boundary conditions and boundary connectivities");
	}
	for (auto boundaryIndex = this->boundaryIndices.cbegin(); boundaryIndex != this->boundaryIndices.cend(); boundaryIndex++) {
		BCType_t bocotype;
		PointSetType_t ptset_type;
		cgsize_t numberOfVertices, NormalListSize;
		int NormalIndex, ndataset;
		DataType_t NormalDataType;
		if (cg_boco_info(this->fileIndex, this->baseIndex, this->zoneIndex, *boundaryIndex, this->buffer, &bocotype, &ptset_type, &numberOfVertices, &NormalIndex, &NormalListSize, &NormalDataType, &ndataset)) {
			throw std::runtime_error("CgnsReader: Could not read boundary information");
		}
		std::vector<cgsize_t> vertices(numberOfVertices);
		if (cg_boco_read(this->fileIndex, this->baseIndex, this->zoneIndex, *boundaryIndex, &vertices[0], nullptr)) {
			throw std::runtime_error("CgnsReader: Could not read boundary");
		}
		for (unsigned i = 0; i < vertices.size(); i++) vertices[i]--;
		this->gridData->boundaries[*boundaryIndex - 1].vertices = std::move(vertices);
	}
}

GridDataShared CgnsReader::getGridData() const {
	return this->gridData;
}

CgnsReader::~CgnsReader() {
	cg_close(this->fileIndex);
	delete this->buffer;
}