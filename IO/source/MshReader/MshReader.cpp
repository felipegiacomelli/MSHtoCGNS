#include <IO/MshReader.hpp>

MshReader::MshReader(const std::string& filePath) : filePath(filePath) {
	this->checkFile();
	this->gridData = MakeShared<GridData>();
}

void MshReader::checkFile() {
    boost::filesystem::path input(this->filePath);
	if (!boost::filesystem::exists(input.parent_path()))
		throw std::runtime_error("MshReader: The parent path does not exist");

	if (!boost::filesystem::exists(this->filePath))
		throw std::runtime_error("MshReader: There is no file in the given path");

	if (input.extension() != ".msh")
		throw std::runtime_error("MshReader: The file extension is not .msh");

	this->file = std::ifstream(this->filePath.c_str());
}

void MshReader::readNodes() {
	int numberOfVertices, temporary;
	this->file.seekg(0, std::ios::beg);
	while (strcmp(this->buffer, "$Nodes") && !this->file.eof())
		this->file >> this->buffer;
	if (this->file.eof())
		throw std::runtime_error("MshReader: There is no Node data in the grid file");

	this->file >> numberOfVertices;
	this->gridData->coordinates.resize(numberOfVertices, std::vector<double>(3));
	for (int i = 0; i < numberOfVertices; i++)
		this->file >> temporary >> this->gridData->coordinates[i][0] >> this->gridData->coordinates[i][1] >> this->gridData->coordinates[i][2];
}

void MshReader::readConnectivities() {
	int numberOfElements;
	this->file.seekg(0, std::ios::beg);
	while (strcmp(this->buffer, "$Elements") && !this->file.eof())
		this->file >> this->buffer;
	if (this->file.eof())
		this->file.clear();
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

	if (connectivities[0][2] != 1)
		throw std::runtime_error("MshReader: Elements must have exactly 2 tags");

	for (unsigned i = 0; i < this->connectivities.size(); i++) {
		this->connectivities[i].erase(this->connectivities[i].begin()    );
		this->connectivities[i].erase(this->connectivities[i].begin() + 1);
		this->connectivities[i].erase(this->connectivities[i].begin() + 2);
	}
}

void MshReader::divideConnectivities() {
	this->elements = std::vector<std::vector<int>>(this->connectivities.begin() + this->numberOfFacets, this->connectivities.end());
	for (unsigned i = 0; i < this->elements.size(); i++)
		this->elements[i].push_back(i);
	int numberOfElements = elements.size();

	this->facets   = std::vector<std::vector<int>>(this->connectivities.begin(), this->connectivities.begin() + this->numberOfFacets);
	unsigned boundaryIndex = 1;
	std::stable_sort(this->facets.begin(), this->facets.end(), [boundaryIndex](const auto& a, const auto& b) {return a[boundaryIndex] < b[boundaryIndex];});
	for (unsigned i = 0; i < this->facets.size(); i++)
		this->facets[i].push_back(numberOfElements + i);
}

void MshReader::assignElementsToRegions() {
	int counter = 0;
	std::vector<unsigned> regionStart;
	regionStart.emplace_back(0);
	for (unsigned i = 0; i < this->elements.size()-1; i++) {
		if (this->elements[i][1] == this->elements[i+1][1])
			counter++;
		else {
			counter++;
			regionStart.push_back(counter);
		}
	}
	regionStart.push_back(this->elements.size());

	for (unsigned i = 0; i < regionStart.size()-1; i++)
		for (unsigned j = regionStart[i]; j < regionStart[i+1]; j++)
			this->elements[j][1] = i;

	this->regionElements.resize(this->numberOfRegions, std::vector<int>());
	for (unsigned i = 0; i < this->elements.size(); i++)
		this->regionElements[this->elements[i][1]].push_back(i);
}

void MshReader::assignFacetsToBoundaries() {
	this->boundaryFacets.resize(this->numberOfBoundaries, std::vector<int>());
	for (unsigned i = 0; i < this->facets.size(); i++)
		this->boundaryFacets[this->facets[i][1]].push_back(i);
}