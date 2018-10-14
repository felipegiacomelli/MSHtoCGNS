#include <FileMend/RadialGridDataReordered.hpp>
#include <cgnslib.h>

RadialGridDataReordered::RadialGridDataReordered(GridDataShared gridData) : gridData(gridData) {
	this->checkGridData();
	this->defineQuantities();
	this->copyData();
	this->final = MakeShared<GridData>();
	this->final->dimension = 3;
	this->reorder();
	this->addVertices();
	this->copyVertices();
	this->copyRegions();
	this->copyWells();
	this->fixIndices();
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

	if (this->gridData->lineConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData lineConnectivity size must not be 0");

	if (this->gridData->regions.size() != 1u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData regions size must be 1 and not " + std::to_string(this->gridData->regions.size()));

	if (this->gridData->wells.size() != 1u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData wells size must be 1 and not " + std::to_string(this->gridData->wells.size()));
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
	for (auto i = 0u; i < this->gridData->coordinates.size(); i++)
		this->coordinates.emplace_back(std::make_pair(i, this->gridData->coordinates[i]));

	this->hexahedra = this->gridData->hexahedronConnectivity;
	for (auto i = 0u; i < this->hexahedra.size(); i++)
		this->hexahedra[i].back() = i;

	this->prisms = this->gridData->prismConnectivity;
	for (auto i = 0u; i < this->prisms.size(); i++)
		this->prisms[i].back() = i;
}

void RadialGridDataReordered::reorder() {
	for (int s = 0; s < this->numberOfSegments; s++) {
		this->final->lineConnectivity.push_back(this->gridData->lineConnectivity[s]);

		for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
			if (hasElement(prism->cbegin(), prism->cend()-1, this->final->lineConnectivity[s].front()))
				this->copyPrism(prism);
			else
				prism++;

		for (int p = 0; p < this->numberOfPrismsPerSegment; p++) {
			auto prism = this->final->prismConnectivity[s*this->numberOfPrismsPerSegment+p];
			auto line = this->final->lineConnectivity[s];
			std::array<int, 4> firstFacet;
			std::copy_if(prism.cbegin(), prism.cend()-1, firstFacet.begin(), [=](auto& p){return p != line[0] && p != line[1];});

			for (auto hexa = this->hexahedra.begin(); hexa != this->hexahedra.end();)
				if (hasElements(hexa->cbegin(), hexa->cend()-1, firstFacet))
					this->copyHexahedron(hexa);
				else
					hexa++;

			for (int h = 0; h < this->numberOfHexahedronsPerRadius-1; h++) {
				auto hexahedron = this->final->hexahedronConnectivity.back();
				std::array<int, 4> lastFacet;
				std::copy_if(hexahedron.cbegin(), hexahedron.cend()-1, lastFacet.begin(), [=](auto& p){return !hasElement(firstFacet.cbegin(), firstFacet.cend(), p);});

				for (auto hexa = this->hexahedra.begin(); hexa != this->hexahedra.end();)
					if (hasElements(hexa->cbegin(), hexa->cend()-1, lastFacet))
						this->copyHexahedron(hexa);
					else
						hexa++;

				firstFacet = lastFacet;
			}
		}
	}
}

void RadialGridDataReordered::copyPrism(std::vector<std::array<int, 7>>::iterator prism) {
	this->final->prismConnectivity.push_back(this->gridData->prismConnectivity[prism->back()]);
	prism = this->prisms.erase(prism);
}

void RadialGridDataReordered::copyHexahedron(std::vector<std::array<int, 9>>::iterator hexahedron) {
	this->final->hexahedronConnectivity.push_back(this->gridData->hexahedronConnectivity[hexahedron->back()]);
	hexahedron = this->hexahedra.erase(hexahedron);
}

void RadialGridDataReordered::addVertices() {
	{
		this->addVertex(std::find_if(this->coordinates.begin(),this->coordinates.end(), [=](const auto& a){return a.first == this->gridData->lineConnectivity.front()[0];}));

		auto first = this->gridData->coordinates[this->gridData->lineConnectivity.front()[0]];
		auto last  = this->gridData->coordinates[this->gridData->lineConnectivity.front()[1]];

		std::array<double, 3> normal;
		std::transform(first.cbegin(), first.cend(), last.cbegin(), normal.begin(), std::minus<>());

		this->findVerticesOnPlane(normal, -1.0 * std::inner_product(first.cbegin(), first.cend(), normal.begin(), 0.0));
		this->segmentNumber++;
	}

	for (int s = 1; s < this->numberOfSegments; s++) {
		this->addVertex(std::find_if(this->coordinates.begin(),this->coordinates.end(), [=](const auto& a){return a.first == this->gridData->lineConnectivity[s][0];}));

		auto first = this->gridData->coordinates[this->gridData->lineConnectivity[s-1][0]];
		auto last  = this->gridData->coordinates[this->gridData->lineConnectivity[s-1][1]];
		this->segmentLength = std::sqrt(std::inner_product(first.cbegin(), first.cend(), last.cbegin(), 0.0, std::plus<>(), [](const auto& a, const auto& b){return (a-b)*(a-b);}));

		std::array<double, 3> normal;
		std::transform(first.cbegin(), first.cend(), last.cbegin(), normal.begin(), std::minus<>());

		first = this->gridData->coordinates[this->gridData->lineConnectivity[s][0]];
		last  = this->gridData->coordinates[this->gridData->lineConnectivity[s][1]];

		std::transform(first.cbegin(), first.cend(), last.cbegin(), normal.begin(), std::minus<>());

		this->findVerticesOnPlane(normal, -1.0 * std::inner_product(first.cbegin(), first.cend(), normal.begin(), 0.0));
		this->segmentNumber++;
	}

	{
		this->addVertex(std::find_if(this->coordinates.begin(),this->coordinates.end(), [=](const auto& a){return a.first == this->gridData->lineConnectivity.back()[1];}));

		auto first = this->gridData->coordinates[this->gridData->lineConnectivity.back()[0]];
		auto last  = this->gridData->coordinates[this->gridData->lineConnectivity.back()[1]];
		this->segmentLength = std::sqrt(std::inner_product(first.cbegin(), first.cend(), last.cbegin(), 0.0, std::plus<>(), [](const auto& a, const auto& b){return (a-b)*(a-b);}));

		std::array<double, 3> normal;
		std::transform(first.cbegin(), first.cend(), last.cbegin(), normal.begin(), std::minus<>());

		this->findVerticesOnPlane(normal, -1.0 * std::inner_product(last.cbegin(), last.cend(), normal.begin(), 0.0));
		this->segmentNumber++;
	}
}

void RadialGridDataReordered::findVerticesOnPlane(const std::array<double, 3>& normal, const double& d) {
	for (auto coordinate = this->coordinates.begin(); coordinate != this->coordinates.end();)
		if (this->isCoordinateOnPlane(coordinate->second, normal, d))
			if (this->checkDistance(coordinate->second))
				this->addVertex(coordinate);
			else
				coordinate++;
		else
			coordinate++;
}

bool RadialGridDataReordered::isCoordinateOnPlane(const std::array<double, 3>& coordinate, const std::array<double, 3>& normal, const double& d) {
	return std::abs(std::inner_product(coordinate.cbegin(), coordinate.cend(), normal.begin(), d)) < this->tolerance;
}

bool RadialGridDataReordered::checkDistance(const std::array<double, 3>& a) {
	if (this->vertices.size() < unsigned(this->numberOfVerticesPerSection))
		return true;

	int shift = this->vertices.size() - this->segmentNumber * this->numberOfVerticesPerSection;
	auto b = this->gridData->coordinates[this->vertices[shift + (this->segmentNumber-1)*this->numberOfVerticesPerSection]];

	return calculateDistance(a, b) - this->segmentLength < this->tolerance;
}

void RadialGridDataReordered::addVertex(std::vector<std::pair<int, std::array<double, 3>>>::iterator vertex) {
	this->vertices.push_back(vertex->first);
	vertex = this->coordinates.erase(vertex);
}

void RadialGridDataReordered::copyVertices() {
	for (auto vertex : this->vertices)
		this->final->coordinates.emplace_back(this->gridData->coordinates[vertex]);
}

void RadialGridDataReordered::copyRegions() {
	this->final->regions = this->gridData->regions;
}

void RadialGridDataReordered::copyWells() {
	this->final->wells = this->gridData->wells;
}

void RadialGridDataReordered::fixIndices() {
	std::unordered_map<int, int> originalToFinal;
	int index = 0;
	for (auto vertex : this->vertices)
		originalToFinal[vertex] = index++;

	for (auto& hexahedron : this->final->hexahedronConnectivity)
		for (auto vertex = hexahedron.begin(); vertex != hexahedron.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& prism : this->final->prismConnectivity)
		for (auto vertex = prism.begin(); vertex != prism.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& line : this->final->lineConnectivity)
		for (auto vertex = line.begin(); vertex != line.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& vertex : this->final->wells[0].vertices)
		vertex = originalToFinal[vertex];

	for (int s = 0; s < this->numberOfSegments; s++) {
		for (int h = 0; h < this->numberOfHexahedronsPerSegment; h++)
			this->final->hexahedronConnectivity[s * this->numberOfHexahedronsPerSegment + h].back() = this->elementShift++;

		for (int p = 0; p < this->numberOfPrismsPerSegment; p++)
			this->final->prismConnectivity[s * this->numberOfPrismsPerSegment + p].back() = this->elementShift++;
	}

	for (int s = 0; s < this->numberOfSegments; s++)
		this->final->lineConnectivity[s].back() = this->elementShift++;
}