#include <IO/GridReader.hpp>

// $Elements
// number-of-elements
// elm-number elm-type number-of-tags < tag > … node-number-list
// …
// $EndElements

GridReader::GridReader(std::string&& filePath) {
	this->filePath = filePath;
	this->file = std::ifstream(this->filePath.c_str());
	this->buffer = new char[800];
	this->gridData.dimension = 2;
	this->readPhysicalEntities();
	this->readNodes();
	this->readElements();
	this->addElements();
}

void GridReader::readPhysicalEntities() {
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) throw std::runtime_error("There is no Physical Entities data in the grid file");
	this->file >> this->numberOfPhysicalEntities;
	std::vector<int> entitiesTypes;			int type;
	std::vector<int> entitiesNumbers;		int number;
	std::vector<std::string> entitiesNames; std::string name;
	for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
		file >> type >> number >> name;
		number--;
		name.erase(name.begin()); name.erase(name.end()-1);
 		entitiesTypes.push_back(type);
		entitiesNumbers.push_back(number);
		entitiesNames.push_back(name);
	}

	std::vector<int> geometryNumbers;
	for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
		if (entitiesTypes[i] == 1) {
			this->boundaryNumbers.push_back(entitiesNumbers[i]);
		}
		else if (entitiesTypes[i] == 2) {
			geometryNumbers.push_back(entitiesTypes[i]);
		}
		else {
			throw std::runtime_error("Non supported physical entity found");
		}
	}
	if (geometryNumbers.size() != 1) throw std::runtime_error("One and only one geometry supported");
	this->geometryNumber = geometryNumbers[0];

	this->gridData.boundaries.resize(boundaryNumbers.size());
	for (int i = 0; i < boundaryNumbers.size(); i++) {
		this->gridData.boundaries[i].name = entitiesNames[boundaryNumbers[i]];
	}
}

void GridReader::readNodes() {
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

void GridReader::readElements() {
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
	for (int i = 0; i < this->elements.size(); i++) {
		this->physicalEntitiesElementIndices[this->elements[i][2]-1].push_back(i);
	}
}

void GridReader::addElements() {
	for (int i = 0; i < this->physicalEntitiesElementIndices.size(); i++) {
		for (int j = 0; j < this->physicalEntitiesElementIndices[i].size(); j++) {
			int index = this->physicalEntitiesElementIndices[i][j];
			int type  = this->elements[index][0];
			if (type == 1) {
				std::vector<int>::const_iterator first = this->elements[index].cbegin() + 4;
				std::vector<int>::const_iterator last  = this->elements[index].cend();
				std::vector<int> line(first, last);
				this->gridData.boundaries[i].lineConnectivity.push_back(line);
			}
			else if (type == 2) {
				std::vector<int>::const_iterator first = this->elements[index].cbegin() + 4;
				std::vector<int>::const_iterator last  = this->elements[index].cend();
				std::vector<int> triangle(first, last);
				this->gridData.triangleConnectivity.push_back(triangle);
			}
			else if (type == 3) {
				std::vector<int>::const_iterator first = this->elements[index].cbegin() + 4;
				std::vector<int>::const_iterator last  = this->elements[index].cend();
				std::vector<int> quadrangle(first, last);
				this->gridData.quadrangleConnectivity.push_back(quadrangle);
			}
			else {
				throw std::runtime_error("Boundary element must be a line");
			}
		}
	
	}
}

GridData GridReader::getGridData() const {
	return this->gridData;
}

GridReader::~GridReader() {
	delete this->buffer;
}

