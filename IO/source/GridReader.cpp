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
	this->readPhysicalEntities();
	this->readNodes();
	this->readElements();
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

	//print(entitiesTypes  , "types"  ); std::cout << std::endl;
	//print(entitiesNumbers, "numbers"); std::cout << std::endl;
	//print(entitiesNames  , "names"  ); std::cout << std::endl;
	
	std::vector<int> boundaryNumbers;
	std::vector<int> geometryNumbers;
	for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
		if (entitiesTypes[i] == 1) {
			boundaryNumbers.push_back(entitiesNumbers[i]);
		}
		else if (entitiesTypes[i] == 2) {
			geometryNumbers.push_back(entitiesTypes[i]);
		}
		else {
			throw std::runtime_error("Non supported physical entity found");
		}
	}
	if (geometryNumbers.size() != 1) throw std::runtime_error("One and only one geometry supported");

	this->gridData.boundaries.resize(boundaryNumbers.size());
	for (int i = 0; i < boundaryNumbers.size(); i++) {
		this->gridData.boundaries[i].name = entitiesNames[boundaryNumbers[i]-1];
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

	std::vector<std::vector<int>> physicalEntitiesElements(this->numberOfPhysicalEntities, std::vector<int>());
	for (int i = 0; i < elements.size(); i++) {
		physicalEntitiesElements[elements[i][2]-1].push_back(i);
	}
	//print(lineIndices, "lineIndices"); std::cout << std::endl;
	//print(triangleIndices, "triangleIndices"); std::cout << std::endl;
	print(elements, "elements"); std::cout << std::endl;
	print(physicalEntitiesElements, "physicalEntitiesElements"); std::cout << std::endl;
}


GridReader::~GridReader() {
	//print(this->gridData.coordinates, "coordinates"); std::cout << std::endl;
	//printGridData(this->gridData);
	delete this->buffer;
}

