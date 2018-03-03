#include <IO/MshReader.hpp>

MshReader::MshReader(const std::string& filePath) :  
	filePath(filePath), buffer(new char[800]), gridData(MakeShared<GridData>()) {
	this->checkFile();
}

void MshReader::checkFile() {
    boost::filesystem::path input(this->filePath);
	if (!boost::filesystem::exists(input.parent_path())) throw std::runtime_error("MshReader: The parent path does not exist");
	if (!boost::filesystem::exists(this->filePath)) throw std::runtime_error("MshReader: There is no file in the given path");
	if (input.extension() != ".msh") throw std::runtime_error("MshReader: The file extension is not .msh");
	this->file = std::ifstream(this->filePath.c_str());
}

void MshReader::readNodes() {
	int numberOfNodes, temporary;
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$Nodes") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) throw std::runtime_error("MshReader: There is no Node data in the grid file");
	this->file >> numberOfNodes;
	this->gridData->coordinates.resize(numberOfNodes, std::vector<double>(3));
	for (int i = 0; i < numberOfNodes; i++) {
		this->file >> temporary >> this->gridData->coordinates[i][0] >> this->gridData->coordinates[i][1] >> this->gridData->coordinates[i][2];
	}
}

void MshReader::readConnectivities() {
	int numberOfElements;
	this->file.seekg(0, std::ios::beg);
	while (strcmp(this->buffer, "$Elements") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) this->file.clear(); 
	else {
		this->file >> numberOfElements;
		for (int i = 0; i < numberOfElements+1; i++) {
			std::string line;
			std::getline(this->file, line);
			std::istringstream stream(line);
			std::vector<int> connectivity;
			int value;
			while (stream >> value) {
				value--;
				connectivity.push_back(value);
			}
			this->connectivities.emplace_back(std::move(connectivity));
		}
	}
	this->connectivities.erase(this->connectivities.begin());
	
	if (connectivities[0][2] != 1) throw std::runtime_error("MshReader: Elements must have exactly 2 tags");

	for (auto i = this->connectivities.begin(); i < this->connectivities.end(); i++) {
		i->erase(i->begin());
		i->erase(i->begin()+1);
		i->erase(i->begin()+2);
	}

	if (this->gridData->dimension == 2) {
		int facetQuantity = 0;
		for (unsigned i = 0; i < connectivities.size(); i++) {
			if (connectivities[i][0] != 0) break;
			else facetQuantity++;
		}
		this->facets   = std::vector<std::vector<int>>(connectivities.begin()                 , connectivities.begin() + facetQuantity);
		this->elements = std::vector<std::vector<int>>(connectivities.begin() + facetQuantity, connectivities.end());
		
		print(elements, "elements");

		int counter = 0;
		std::vector<unsigned> regionStart;
		regionStart.emplace_back(0);
		for (unsigned i = 0; i < elements.size()-1; i++) {
			if (elements[i][1] == elements[i+1][1]) counter++;
			else {
				counter++;
				regionStart.push_back(counter);
			}
		}
		regionStart.push_back(elements.size());
		for (unsigned i = 0; i < regionStart.size()-1; i++) {
			for (unsigned j = regionStart[i]; j < regionStart[i+1]; j++) {
				elements[j][1] = i;
			}
		}

		print(regionStart, "regionStart");
		print(elements, "elements");

		this->facetsOnBoundary.resize(this->numberOfBoundaries, std::vector<int>());
		for (unsigned i = 0; i < this->facets.size(); i++) {
			facetsOnBoundary[facets[i][1]].push_back(i);
		}
		print(facetsOnBoundary, "facetsOnBoundary");
		
		this->elementsOnRegion.resize(this->numberOfRegions, std::vector<int>());
		for (unsigned i = 0; i < this->elements.size(); i++) {
			elementsOnRegion[elements[i][1]].push_back(i);
		}
		print(elementsOnRegion, "elementsOnRegion");
	}

	this->physicalEntitiesElementIndices.resize(this->numberOfPhysicalEntities, std::vector<int>());
	for (unsigned i = 0; i < this->connectivities.size(); i++) {
		this->physicalEntitiesElementIndices[this->connectivities[i][1]].push_back(i);
	}
}

GridDataShared MshReader::getGridData() const {
	return this->gridData;
}

MshReader::~MshReader() {
	delete this->buffer;
}