#include <FileMend/RadialGridDataReordered.hpp>
#include <Utilities/Print.hpp>
#include <cgnslib.h>

RadialGridDataReordered::RadialGridDataReordered(GridDataShared gridData) : gridData(gridData) {
	this->checkGridData();
	this->defineQuantities();
	this->createReordered();
	this->copyData();
	this->reorder();
	this->copyVertices();
	this->fixVerticesIndices();
	this->fixElementIndices();
	this->fixFacetIndices();
}

void RadialGridDataReordered::checkGridData() {
	if (this->gridData->dimension != 3)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

	if (this->gridData->tetrahedronConnectivity.size() != 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData tetrahedronConnectivity size must be 0 and not " + std::to_string(this->gridData->tetrahedronConnectivity.size()));

	if (this->gridData->hexahedronConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData hexahedronConnectivity size must not be 0");

	if (this->gridData->prismConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData prismConnectivity size must not be 0");

	if (this->gridData->pyramidConnectivity.size() != 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData pyramidConnectivity size must be 0 and not " + std::to_string(this->gridData->pyramidConnectivity.size()));

	if (this->gridData->triangleConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData triangleConnectivity size must not be 0");

	if (this->gridData->quadrangleConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData quadrangleConnectivity size must not be 0");

	if (this->gridData->lineConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData lineConnectivity size must not be 0");

	if (this->gridData->boundaries.size() != 3u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData boundaries size must be 1 and not " + std::to_string(this->gridData->boundaries.size()));

	if (this->gridData->regions.size() != 1u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData regions size must be 1 and not " + std::to_string(this->gridData->regions.size()));

	if (this->gridData->wells.size() != 1u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData wells size must be 1 and not " + std::to_string(this->gridData->wells.size()));
}

void RadialGridDataReordered::createReordered() {
	this->reordered = MakeShared<GridData>();
	this->reordered->dimension = this->gridData->dimension;

	this->reordered->triangleConnectivity = this->gridData->triangleConnectivity;
	this->reordered->quadrangleConnectivity = this->gridData->quadrangleConnectivity;
	this->reordered->lineConnectivity = this->gridData->lineConnectivity;

	this->reordered->boundaries = this->gridData->boundaries;

	this->reordered->regions = this->gridData->regions;

	this->reordered->wells = this->gridData->wells;
}

void RadialGridDataReordered::defineQuantities() {
	this->numberOfSegments = this->gridData->lineConnectivity.size();

	this->numberOfPrismsPerSegment = this->gridData->prismConnectivity.size() / this->numberOfSegments;
	this->numberOfHexahedronsPerSegment = this->gridData->hexahedronConnectivity.size() / this->numberOfSegments;
	this->numberOfHexahedronsPerRadius = this->numberOfHexahedronsPerSegment / this->numberOfPrismsPerSegment;
	this->numberOfVerticesPerSection = this->gridData->coordinates.size() / (this->gridData->lineConnectivity.size()+1);

	printf("\n\tnumberOfSegments             : %4i", this->numberOfSegments);
	printf("\n\tnumberOfPrisms               : %4i", int(this->gridData->prismConnectivity.size()));
	printf("\n\tnumberOfHexahedrons          : %4i", int(this->gridData->hexahedronConnectivity.size()));
	printf("\n\tnumberOfPrismsPerSegment     : %4i", this->numberOfPrismsPerSegment);
	printf("\n\tnumberOfHexahedronsPerSegment: %4i", this->numberOfHexahedronsPerSegment);
	printf("\n\tnumberOfHexahedronsPerRadius : %4i", this->numberOfHexahedronsPerRadius);
	printf("\n\tnumberOfVerticesPerSection   : %4i", this->numberOfVerticesPerSection);
	printf("\n\n");
}

void RadialGridDataReordered::copyData() {
	this->hexahedra = this->gridData->hexahedronConnectivity;
	for (auto i = 0u; i < this->hexahedra.size(); i++)
		this->hexahedra[i].back() = i;

	this->prisms = this->gridData->prismConnectivity;
	for (auto i = 0u; i < this->prisms.size(); i++)
		this->prisms[i].back() = i;
}

void RadialGridDataReordered::reorder() {
	auto boundary = *std::find_if(this->gridData->boundaries.cbegin(), this->gridData->boundaries.cend(), [=](auto b){return hasElement(b.vertices.cbegin(), b.vertices.cend(), this->gridData->lineConnectivity.front()[0]);});

	std::vector<std::array<int, 4>> triangles;
	for (auto i = this->gridData->triangleConnectivity.cbegin(); i != this->gridData->triangleConnectivity.cend(); i++)
		if (i->back() >= boundary.facetBegin && i->back() < boundary.facetEnd)
			triangles.emplace_back(*i);

	std::vector<std::array<int, 5>> quadrangles;
	for (auto i = this->gridData->quadrangleConnectivity.cbegin(); i != this->gridData->quadrangleConnectivity.cend(); i++)
		if (i->back() >= boundary.facetBegin && i->back() < boundary.facetEnd)
			quadrangles.emplace_back(*i);

	{
		int s = 0;
		for (auto triangle = triangles.begin(); triangle != triangles.end(); triangle++)
			for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
				if (hasElements(prism->cbegin(), prism->cend()-1, triangle->cbegin(), triangle->cend()-1)) {
					this->addVertexAndHandle(std::make_pair((*prism)[0], s * this->numberOfVerticesPerSection + this->vertexShift), std::make_pair((*prism)[3], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));
					this->addVertexAndHandle(std::make_pair((*prism)[1], s * this->numberOfVerticesPerSection + this->vertexShift), std::make_pair((*prism)[4], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));
					this->addVertexAndHandle(std::make_pair((*prism)[2], s * this->numberOfVerticesPerSection + this->vertexShift), std::make_pair((*prism)[5], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));

					(*triangle)[0] = (*prism)[3];
					(*triangle)[1] = (*prism)[4];
					(*triangle)[2] = (*prism)[5];

					this->copyPrism(prism);
					break;
				}
				else
					prism++;

		for (auto quadrangle = quadrangles.begin(); quadrangle != quadrangles.end(); quadrangle++)
			for (auto hexahedron = this->hexahedra.begin(); hexahedron != this->hexahedra.end();)
				if (hasElements(hexahedron->cbegin(), hexahedron->cend()-1, quadrangle->cbegin(), quadrangle->cend()-1)) {

					this->addVertexAndHandle(std::make_pair((*hexahedron)[0], s * this->numberOfVerticesPerSection + this->vertexShift), std::make_pair((*hexahedron)[3], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));
					this->addVertexAndHandle(std::make_pair((*hexahedron)[1], s * this->numberOfVerticesPerSection + this->vertexShift), std::make_pair((*hexahedron)[2], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));
					this->addVertexAndHandle(std::make_pair((*hexahedron)[5], s * this->numberOfVerticesPerSection + this->vertexShift), std::make_pair((*hexahedron)[6], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));
					this->addVertexAndHandle(std::make_pair((*hexahedron)[4], s * this->numberOfVerticesPerSection + this->vertexShift), std::make_pair((*hexahedron)[7], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));

					(*quadrangle)[0] = (*hexahedron)[2];
					(*quadrangle)[1] = (*hexahedron)[3];
					(*quadrangle)[2] = (*hexahedron)[7];
					(*quadrangle)[3] = (*hexahedron)[6];

					this->copyHexahedron(hexahedron);
					break;
				}
				else
					hexahedron++;
	}

	for (int s = 1; s < this->numberOfSegments; s++) {
		this->vertexShift = 0;

		for (auto triangle = triangles.begin(); triangle != triangles.end(); triangle++)
			for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
				if (hasElements(prism->cbegin(), prism->cend()-1, triangle->cbegin(), triangle->cend()-1)) {
					this->addVertexAndHandle(std::make_pair((*prism)[3], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));
					this->addVertexAndHandle(std::make_pair((*prism)[4], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));
					this->addVertexAndHandle(std::make_pair((*prism)[5], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));

					(*triangle)[0] = (*prism)[3];
					(*triangle)[1] = (*prism)[4];
					(*triangle)[2] = (*prism)[5];

					this->copyPrism(prism);
					break;
				}
				else
					prism++;

		for (auto quadrangle = quadrangles.begin(); quadrangle != quadrangles.end(); quadrangle++)
			for (auto hexahedron = this->hexahedra.begin(); hexahedron != this->hexahedra.end();)
				if (hasElements(hexahedron->cbegin(), hexahedron->cend()-1, quadrangle->cbegin(), quadrangle->cend()-1)) {
					this->addVertexAndHandle(std::make_pair((*hexahedron)[3], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));
					this->addVertexAndHandle(std::make_pair((*hexahedron)[2], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));
					this->addVertexAndHandle(std::make_pair((*hexahedron)[6], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));
					this->addVertexAndHandle(std::make_pair((*hexahedron)[7], (s+1) * this->numberOfVerticesPerSection + this->vertexShift));

					(*quadrangle)[0] = (*hexahedron)[2];
					(*quadrangle)[1] = (*hexahedron)[3];
					(*quadrangle)[2] = (*hexahedron)[7];
					(*quadrangle)[3] = (*hexahedron)[6];

					this->copyHexahedron(hexahedron);
					break;
				}
				else
					hexahedron++;
	}

	printf("\n\n");
	printf("\n\tnumberOfPrisms               : %4i", int(this->reordered->prismConnectivity.size()));
	printf("\n\tnumberOfHexahedrons          : %4i", int(this->reordered->hexahedronConnectivity.size()));
	printf("\n\tnumberOfVertices             : %4i", int(this->verticesAndHandles.size()));
	printf("\n\tvertexShift                  : %4i", int(this->vertexShift));

	printf("\n\n\t###################################\n\n");
}

void RadialGridDataReordered::addVertexAndHandle(std::pair<int, int>&& firstVertexAndHandle, std::pair<int, int>&& secondVertexAndHandle) {
	if (std::find_if(this->verticesAndHandles.cbegin(), this->verticesAndHandles.cend(), [=](auto vh){return vh.first == firstVertexAndHandle.first;}) == this->verticesAndHandles.cend())
		if (std::find_if(this->verticesAndHandles.cbegin(), this->verticesAndHandles.cend(), [=](auto vh){return vh.first == secondVertexAndHandle.first;}) == this->verticesAndHandles.cend()) {
		this->verticesAndHandles.push_back(firstVertexAndHandle);
		this->verticesAndHandles.push_back(secondVertexAndHandle);
		this->vertexShift++;
	}
}

void RadialGridDataReordered::addVertexAndHandle(std::pair<int, int>&& vertexAndHandle) {
	if (std::find_if(this->verticesAndHandles.cbegin(), this->verticesAndHandles.cend(), [=](auto vh){return vh.first == vertexAndHandle.first;}) == this->verticesAndHandles.cend()) {
		this->verticesAndHandles.push_back(vertexAndHandle);
		this->vertexShift++;
	}
}

void RadialGridDataReordered::copyPrism(std::vector<std::array<int, 7>>::iterator prism) {
	this->reordered->prismConnectivity.push_back(this->gridData->prismConnectivity[prism->back()]);
	prism = this->prisms.erase(prism);
}

void RadialGridDataReordered::copyHexahedron(std::vector<std::array<int, 9>>::iterator hexahedron) {
	this->reordered->hexahedronConnectivity.push_back(this->gridData->hexahedronConnectivity[hexahedron->back()]);
	hexahedron = this->hexahedra.erase(hexahedron);
}

void RadialGridDataReordered::copyVertices() {
	std::stable_sort(this->verticesAndHandles.begin(), this->verticesAndHandles.end(), [](auto a, auto b) {return a.second < b.second;});
	for (auto vertex : this->verticesAndHandles)
		this->reordered->coordinates.emplace_back(this->gridData->coordinates[vertex.first]);

	// std::cout << std::endl << "\thandles : " << std::endl;
	// for (auto i = 0; i < 10; i++) {
	// 	int shift = i * this->numberOfVerticesPerSection;
	// 	for (auto j = 0; j < 37; j++) {
	// 		std::cout << "\t" << std::setw(4) << std::right << shift + j << " : " << std::setw(4) << std::right << this->verticesAndHandles[shift + j].first << " - " << std::setw(4) << std::right << this->verticesAndHandles[shift + j].second;
	// 		std::cout << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// std::cout << std::endl;

	// std::vector<std::array<int, 2>> fuck;
	// for (std::size_t i = 0; i < this->verticesAndHandles.size(); ++i)
	// 	for (std::size_t j = i + 1; j < this->verticesAndHandles.size(); ++j)
	// 		if (this->verticesAndHandles[i].first == this->verticesAndHandles[j].first)
	// 			fuck.push_back(std::array<int, 2>{int(i), int(j)});

	// for (auto f : fuck)
	// 	std::cout << "\t" << f[2] << "\t" << f[1] << " : " << this->verticesAndHandles[f[0]].first << "\t" << this->verticesAndHandles[f[1]].first << std::endl << std::endl;
}

void RadialGridDataReordered::fixVerticesIndices() {
	std::unordered_map<int, int> originalToFinal;
	for (auto vertexAndHandle : this->verticesAndHandles)
		originalToFinal[vertexAndHandle.first] = vertexAndHandle.second;

	for (auto& hexahedron : this->reordered->hexahedronConnectivity)
		for (auto vertex = hexahedron.begin(); vertex != hexahedron.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& prism : this->reordered->prismConnectivity)
		for (auto vertex = prism.begin(); vertex != prism.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& triangle : this->reordered->triangleConnectivity)
		for (auto vertex = triangle.begin(); vertex != triangle.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& quadrangle : this->reordered->quadrangleConnectivity)
		for (auto vertex = quadrangle.begin(); vertex != quadrangle.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& line : this->reordered->lineConnectivity)
		for (auto vertex = line.begin(); vertex != line.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& vertex : this->reordered->wells[0].vertices)
		vertex = originalToFinal[vertex];

	for (auto& boundary : this->reordered->boundaries)
		for (auto& vertex : boundary.vertices)
			vertex = originalToFinal[vertex];
}

void RadialGridDataReordered::fixElementIndices() {
	for (int s = 0; s < this->numberOfSegments; s++) {
		for (int p = 0; p < this->numberOfPrismsPerSegment; p++)
			this->reordered->prismConnectivity[s * this->numberOfPrismsPerSegment + p].back() = this->elementShift++;
		for (int h = 0; h < this->numberOfHexahedronsPerSegment; h++)
			this->reordered->hexahedronConnectivity[s * this->numberOfHexahedronsPerSegment + h].back() = this->elementShift++;
	}
	for (int s = 0; s < this->numberOfSegments; s++)
		this->reordered->lineConnectivity[s].back() = this->elementShift++;
}

void RadialGridDataReordered::fixFacetIndices() {
// 	for (auto& triangle : this->reordered->triangleConnectivity)
// 		for (auto vertex = triangle.begin(); vertex != triangle.end() - 1; vertex++)
// 			*vertex = originalToFinal[*vertex];

// 	for (auto& quadrangle : this->reordered->quadrangleConnectivity)
// 		for (auto vertex = quadrangle.begin(); vertex != quadrangle.end() - 1; vertex++)
// 			*vertex = originalToFinal[*vertex];
}