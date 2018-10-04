#include <MshInterface/MshReader/MshReader3D.hpp>
#include <Utilities/Print.hpp>

MshReader3D::MshReader3D(std::string filePath) : MshReader(filePath) {
	this->gridData->dimension = 3;
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

void MshReader3D::readPhysicalEntities() {
	this->file.seekg(0, std::ios::beg);
	while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof())
		this->file >> this->buffer;
	if (this->file.eof())
		throw std::runtime_error("MshReader3D: There is no Physical Entities data in the grid file");

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
			case 1: {
				boundaryIndices.push_back(entitiesIndices[i]);
				break;
			}
			case 2: {
				regionsIndices.push_back(entitiesIndices[i]);
				break;
			}
			default:
				throw std::runtime_error("MshReader3Ds: Physical entity " + std::to_string(entitiesTypes[i]) + " not supported");
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
}

void MshReader3D::determineNumberOfFacets() {
	this->numberOfFacets = 0;
	for (unsigned i = 0; i < this->connectivities.size(); i++) {
		if (this->connectivities[i][0] != 1 && this->connectivities[i][0] != 2)
			break;
		else
			this->numberOfFacets++;
	}
}

void MshReader3D::addRegions() {
	for (unsigned i = 0; i < this->regionElements.size(); i++) {
		this->gridData->regions[i].elementBegin = this->regionElements[i].front();
		this->gridData->regions[i].elementEnd   = this->regionElements[i].back() + 1;
		for (unsigned j = 0; j < this->regionElements[i].size(); j++) {
			int index = this->regionElements[i][j];
			int type  = this->elements[index][0];
			std::vector<int> connectivity(this->elements[index].cbegin() + 2, this->elements[index].cend());
			switch (type) {
				case 3: {
					std::array<int, 5> tetrahedron;
					std::copy_n(std::begin(connectivity), 5, std::begin(tetrahedron));
					this->gridData->tetrahedronConnectivity.emplace_back(std::move(tetrahedron));
					break;
				}
				case 4: {
					std::array<int, 9> hexahedron;
					std::copy_n(std::begin(connectivity), 9, std::begin(hexahedron));
					this->gridData->hexahedronConnectivity.emplace_back(std::move(hexahedron));
					break;
				}
				default:
					throw std::runtime_error("MshReader3D: Non supported element found");
			}
		}
	}
}

void MshReader3D::addBoundaries() {
	for (unsigned i = 0; i < this->boundaryFacets.size(); i++) {
		this->gridData->boundaries[i].facetBegin = this->boundaryFacets[i].front();
		this->gridData->boundaries[i].facetEnd   = this->boundaryFacets[i].back() + 1;
		for (unsigned j = 0; j < this->boundaryFacets[i].size(); j++) {
			int index = this->boundaryFacets[i][j];
			int type  = this->facets[index][0];
			std::vector<int> connectivity(this->facets[index].cbegin() + 2, this->facets[index].cend());
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
					throw std::runtime_error("MshReader3D: Non supported element found");
			}
		}
	}
}

void MshReader3D::defineBoundaryVertices() {
	for (auto boundary = this->gridData->boundaries.begin(); boundary != this->gridData->boundaries.end(); boundary++) {
		std::set<int> vertices;
		if (this->gridData->triangleConnectivity.size() > 0) {
			std::vector<std::array<int, 4>> boundaryConnectivity(this->gridData->triangleConnectivity.cbegin() + boundary->facetBegin - this->elements.size(), this->gridData->triangleConnectivity.cbegin() + boundary->facetEnd - this->elements.size());
			for (unsigned j = 0; j < boundaryConnectivity.size(); j++)
				for (unsigned k = 0; k != 3u; k++)
					vertices.insert(boundaryConnectivity[j][k]);
			boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
		}
		else {
			std::vector<std::array<int, 5>> boundaryConnectivity(this->gridData->quadrangleConnectivity.cbegin() + boundary->facetBegin - this->elements.size(), this->gridData->quadrangleConnectivity.cbegin() + boundary->facetEnd - this->elements.size());
			for (unsigned j = 0; j < boundaryConnectivity.size(); j++)
				for (unsigned k = 0; k != 4u; k++)
					vertices.insert(boundaryConnectivity[j][k]);
			boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
		}
	}
}