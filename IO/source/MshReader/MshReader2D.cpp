#include <IO/MshReader2D.hpp>

MshReader2D::MshReader2D(const std::string& filePath) : MshReader(filePath) {
	this->gridData->dimension = 2;
	this->readNodes();
	this->readPhysicalEntities();
	this->readConnectivities();
	this->determineNumberOfFacets();
	this->divideConnectivities();
	this->assignElementsToRegions();
	this->assignFacetsToBoundaries();
	this->addRegions();
	this->addBoundaries();
	this->defineBoundaryVertices();
}

void MshReader2D::readPhysicalEntities() {
	this->file.seekg(0, std::ios::beg);
	while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof())
		this->file >> this->buffer;
	if (this->file.eof())
		throw std::runtime_error("MshReader2D: There is no Physical Entities data in the grid file");

	this->file >> this->numberOfPhysicalEntities;
	std::vector<int> entitiesTypes;
	std::vector<int> entitiesIndices;
	std::vector<std::string> entitiesNames;
	for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
		int type, index;
		std::string name;
		this->file >> type >> index >> name;
		type--;
		index--;
		name.erase(name.begin()); name.erase(name.end()-1);
 		entitiesTypes.push_back(type);
		entitiesIndices.push_back(index);
		entitiesNames.push_back(name);
	}

	std::vector<int> regionsIndices, boundaryIndices;
	for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
		switch(entitiesTypes[i]) {
			case 0: {
				boundaryIndices.push_back(entitiesIndices[i]);
				break;
			}
			case 1: {
				regionsIndices.push_back(entitiesIndices[i]);
				break;
			}
			default:
				throw std::runtime_error("MshReader2D: Non supported physical entity found");
		}
	}

	this->numberOfBoundaries = boundaryIndices.size();
	this->gridData->boundaries.resize(boundaryIndices.size());
	for (unsigned i = 0; i < boundaryIndices.size(); i++)
		this->gridData->boundaries[i].name = entitiesNames[boundaryIndices[i]];

	this->numberOfRegions = regionsIndices.size();
	this->gridData->regions.resize(regionsIndices.size());
	for (unsigned i = 0; i < regionsIndices.size(); i++)
		this->gridData->regions[i].name = entitiesNames[regionsIndices[i]];

	std::iota(boundaryIndices.begin(), boundaryIndices.end(), 0);
	std::iota(regionsIndices.begin(), regionsIndices.end(), 0);
}

void MshReader2D::determineNumberOfFacets() {
	this->numberOfFacets = 0;
	for (unsigned i = 0; i < this->connectivities.size(); i++) {
		if (this->connectivities[i][0] != 0)
			break;
		else
			this->numberOfFacets++;
	}
}

void MshReader2D::addRegions() {
	for (unsigned i = 0; i < this->regionElements.size(); i++) {
		this->gridData->regions[i].elementsOnRegion = this->regionElements[i];
		for (unsigned j = 0; j < this->regionElements[i].size(); j++) {
			int index = this->regionElements[i][j];
			int type  = this->elements[index][0];
			std::vector<int> connectivity(this->elements[index].cbegin() + 2, this->elements[index].cend());
			switch (type) {
				case 1: {
					std::array<int, 4> triangle;
					std::copy_n(std::begin(connectivity), 4, std::begin(triangle));
					this->gridData->triangleConnectivity.emplace_back(std::move(triangle));
					break;
				}
				case 2: {
					std::array<int, 5> quadrangle;
					std::copy_n(std::begin(connectivity), 5, std::begin(quadrangle));
					this->gridData->quadrangleConnectivity.emplace_back(std::move(quadrangle));
					break;
				}
				default:
					throw std::runtime_error("MshReader2D: Non supported element found");
			}
		}
	}
}

void MshReader2D::addBoundaries() {
	for (unsigned i = 0; i < this->boundaryFacets.size(); i++) {
		for (unsigned j = 0; j < this->boundaryFacets[i].size(); j++) {
			int index = this->boundaryFacets[i][j];
			int type  = this->facets[index][0];
			this->gridData->boundaries[i].facetsOnBoundary.emplace_back(this->facets[index].back());
			std::vector<int> connectivity(this->facets[index].cbegin() + 2, this->facets[index].cend());
			switch (type) {
				case 0: {
					std::array<int, 3> line;
					std::copy_n(std::begin(connectivity), 3, std::begin(line));
					this->gridData->lineConnectivity.emplace_back(std::move(line));
					break;
				}
				default:
					throw std::runtime_error("MshReader2D: Non supported facet found");
			}
		}
	}
}

void MshReader2D::defineBoundaryVertices() {
	for (auto boundary = this->gridData->boundaries.begin(); boundary < this->gridData->boundaries.end(); boundary++) {
		std::set<int> vertices;
		std::vector<std::array<int, 3>> facets(this->gridData->lineConnectivity.cbegin() + boundary->facetsOnBoundary.front() - this->elements.size(),
												this->gridData->lineConnectivity.cbegin() + boundary->facetsOnBoundary.back() + 1 - this->elements.size());
		for (auto j = facets.cbegin(); j != facets.cend(); j++)
			for (auto k = j->cbegin(); k != j->cend()-1; k++)
				vertices.insert(*k);
		boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
	}
}