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
	this->readNodes();
	this->readElements();
}

void GridReader::readPhysicalEntities() {
	int numberOfPhysicalEntities;
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) throw std::runtime_error("There is no Physical Entities data in the grid file\n");
	this->file >> numberOfPhysicalEntities;
}

void GridReader::readNodes() {
	int numberOfNodes, temporary;
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$Nodes") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) throw std::runtime_error("There is no Node data in the grid file\n");
	this->file >> numberOfNodes;
	this->gridData.coordinates.resize(numberOfNodes, std::vector<double>(3));
	for (int i = 0; i < numberOfNodes; i++) {
		this->file >> temporary >> this->gridData.coordinates[i][0] >> this->gridData.coordinates[i][1] >> this->gridData.coordinates[i][2];
	}
}

void GridReader::readElements() {
	int numberOfElements;
	std::vector<std::vector<int>> elements;
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
			elements.push_back(element);
		}
	}
	elements.erase(elements.begin());
	for (auto i = elements.begin(); i < elements.end(); i++) {
		i->erase(i->begin());		
	}

	std::vector<int> lineIndices;
	std::vector<int> triangleIndices;
	std::vector<int> quadrangleIndices;
	std::vector<int> tetrahedraIndices;
	std::vector<int> hexahedraIndices;
	std::vector<int> pyramidsIndices;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i][0] == 1) {
			lineIndices.push_back(i);
			continue;
		}
		if (elements[i][0] == 2) {
			triangleIndices.push_back(i);
			continue;
		}
		if (elements[i][0] == 3) {
			quadrangleIndices.push_back(i);
			continue;
		} 
		if (elements[i][0] == 4) {
			tetrahedraIndices.push_back(i);
			continue;
		} 
		if (elements[i][0] == 5) {
			hexahedraIndices.push_back(i);
			continue;
		}
		if (elements[i][0] == 7) {
			pyramidsIndices.push_back(i);
			continue;
		}
	}
	print(lineIndices, "lineIndices"); std::cout << std::endl;
	print(triangleIndices, "triangleIndices"); std::cout << std::endl;
	//print(elements, "elements"); std::cout << std::endl;
}


GridReader::~GridReader() {
	//print(this->gridData.coordinates, "coordinates"); std::cout << std::endl;
	delete this->buffer;
}

