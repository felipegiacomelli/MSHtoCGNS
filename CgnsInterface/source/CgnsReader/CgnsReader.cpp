#include <CgnsInterface/CgnsReader/CgnsReader.hpp>
#include <cgnslib.h>

CgnsReader::CgnsReader(const std::string& filePath) : filePath(filePath) {
	this->checkFile();
	this->readBase();
	this->readZone();
	this->readNumberOfSections();
	this->readNumberOfBoundaries();
	this->gridData = MakeShared<GridData>();
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
	ZoneType_t zoneType;
	if (cg_zone_type(this->fileIndex, this->baseIndex, this->zoneIndex, &zoneType)) {
		throw std::runtime_error("CgnsReader: Could not read zone type");
	}
	if (zoneType != Unstructured) {
		throw std::runtime_error("CgnsReader: Only unstructured zones are supported"); 
	}
	if (cg_zone_read(this->fileIndex, this->baseIndex, this->zoneIndex, this->buffer, &this->sizes[0])) {
		throw std::runtime_error("CgnsReader: Could not read zone");
	}
}

void CgnsReader::readNumberOfSections() {
	if (cg_nsections(this->fileIndex, this->baseIndex, this->zoneIndex, &this->numberOfSections)) { 
		throw std::runtime_error("CgnsReader: Could not read number of sections");
	}
}

void CgnsReader::readNumberOfBoundaries() {
	if (cg_nbocos(this->fileIndex, this->baseIndex, this->zoneIndex, &this->numberOfBoundaries)) {
		throw std::runtime_error("CgnsReader: Could not read number of boundaries");
	}
}

void CgnsReader::readBoundaries() {
	if (static_cast<unsigned>(this->numberOfBoundaries) != this->gridData->boundaries.size()) {
		throw std::runtime_error("CgnsReader: mismatch between number of boundary conditions and boundary connectivities");
	}
	for (int boundaryIndex = 1; boundaryIndex <= this->numberOfBoundaries; boundaryIndex++) {
		BCType_t bocotype;
		PointSetType_t ptset_type;
		int numberOfVertices, NormalListSize;
		int NormalIndex, ndataset;
		DataType_t NormalDataType;
		if (cg_boco_info(this->fileIndex, this->baseIndex, this->zoneIndex, boundaryIndex, this->buffer, &bocotype, &ptset_type, &numberOfVertices, &NormalIndex, &NormalListSize, &NormalDataType, &ndataset)) {
			throw std::runtime_error("CgnsReader: Could not read boundary information");
		}
		std::vector<int> vertices(numberOfVertices);
		if (cg_boco_read(this->fileIndex, this->baseIndex, this->zoneIndex, boundaryIndex, &vertices[0], nullptr)) {
			throw std::runtime_error("CgnsReader: Could not read boundary");
		}
		for (unsigned i = 0; i < vertices.size(); i++) vertices[i]--;
		this->gridData->boundaries[boundaryIndex - 1].vertices = std::move(vertices);
	}
}

CgnsReader::~CgnsReader() {
	cg_close(this->fileIndex);
}