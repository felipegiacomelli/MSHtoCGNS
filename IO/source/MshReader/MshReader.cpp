#include <IO/MshReader.hpp>

MshReader::MshReader(const std::string& filePath) {
	if (!boost::filesystem::exists(filePath)) throw std::runtime_error("There is no .msh file in the given path");
	this->filePath = filePath;
	this->file = std::ifstream(this->filePath.c_str());
	this->buffer = new char[800];
}

void MshReader::readNodes() {
	int numberOfNodes, temporary;
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$Nodes") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) throw std::runtime_error("There is no Node data in the grid file");
	this->file >> numberOfNodes;
	this->gridData.coordinates.resize(numberOfNodes, std::vector<double>(3));
	for (int i = 0; i < numberOfNodes; i++) {
		this->file >> temporary >> this->gridData.coordinates[i][0] >> this->gridData.coordinates[i][1] >> this->gridData.coordinates[i][2];
	}
}

void MshReader::readElements() {
	int numberOfElements;
	this->file.seekg(0, std::ios::beg);
	while (strcmp(this->buffer, "$Elements") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) { 
		this->file.clear(); 
	}
	else {
		this->file >> numberOfElements;
		for (int i = 0; i < numberOfElements+1; i++) {
			std::string line;
			std::getline(this->file, line);
			std::istringstream stream(line);
			std::vector<int> element;
			int value;
			while (stream >> value) element.push_back(value);
			this->elements.push_back(element);
		}
	}
	this->elements.erase(this->elements.begin());
	for (auto i = this->elements.begin(); i < this->elements.end(); i++) {
		i->erase(i->begin());		
	}

	this->physicalEntitiesElementIndices.resize(this->numberOfPhysicalEntities, std::vector<int>());
	for (unsigned int i = 0; i < this->elements.size(); i++) {
		this->physicalEntitiesElementIndices[this->elements[i][2]-1].push_back(i);
	}
}

GridData MshReader::getGridData() const {
	return this->gridData;
}

MshReader::~MshReader() {
	delete this->buffer;
}

