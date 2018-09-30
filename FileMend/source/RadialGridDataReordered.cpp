#include <FileMend/RadialGridDataReordered.hpp>
#include <cgnslib.h>

RadialGridDataReordered::RadialGridDataReordered(GridDataShared gridData) : gridData(gridData) {
	this->checkGridData();
	this->defineQuantities();
	this->copyElements();
	this->final = MakeShared<GridData>();
	this->final->dimension = 3;
	this->reorder();
	this->fixIndices();
}

void RadialGridDataReordered::checkGridData() {
	if (this->gridData->dimension != 3)
		throw std::runtime_error("WellGenerator: gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

	if (this->gridData->tetrahedronConnectivity.size() != 0u)
		throw std::runtime_error("WellGenerator: gridData tetrahedronConnectivity size must be 0 and not " + std::to_string(this->gridData->tetrahedronConnectivity.size()));

	if (this->gridData->hexahedronConnectivity.size() == 0u)
		throw std::runtime_error("WellGenerator: gridData hexahedronConnectivity size must not be 0");

	if (this->gridData->prismConnectivity.size() == 0u)
		throw std::runtime_error("WellGenerator: gridData prismConnectivity size must not be 0");

	if (this->gridData->pyramidConnectivity.size() != 0u)
		throw std::runtime_error("WellGenerator: gridData pyramidConnectivity size must be 0 and not " + std::to_string(this->gridData->pyramidConnectivity.size()));

	if (this->gridData->lineConnectivity.size() == 0u)
		throw std::runtime_error("WellGenerator: gridData lineConnectivity size must not be 0");
}

void RadialGridDataReordered::defineQuantities() {
	this->numberOfSegments = this->gridData->lineConnectivity.size();

	this->numberOfPrismsPerSegment = this->gridData->prismConnectivity.size() / this->numberOfSegments;
	this->numberOfHexahedronsPerSegment = this->gridData->hexahedronConnectivity.size() / this->numberOfSegments;
	this->numberOfHexahedronsPerRadius = this->numberOfHexahedronsPerSegment / this->numberOfPrismsPerSegment;

	printf("\n\tnumberOfSegments             : %i", numberOfSegments);
	printf("\n\tnumberOfPrisms               : %i", int(this->gridData->prismConnectivity.size()));
	printf("\n\tnumberOfHexahedrons          : %i", int(this->gridData->hexahedronConnectivity.size()));
	printf("\n\tnumberOfVerticesPerSegment   : %i", int(this->gridData->coordinates.size() / this->numberOfSegments));
	printf("\n\tnumberOfPrismsPerSegment     : %i", numberOfPrismsPerSegment);
	printf("\n\tnumberOfHexahedronsPerSegment: %i", numberOfHexahedronsPerSegment);
	printf("\n\tnumberOfHexahedronsPerRadius : %i", numberOfHexahedronsPerRadius);
	printf("\n\n");
}

void RadialGridDataReordered::copyElements() {
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

		for (const auto& prism : this->prisms)
			if (hasElement(prism.cbegin(), prism.cend()-1, this->final->lineConnectivity[s].front()))
				this->final->prismConnectivity.push_back(prism);

		for (auto rit = this->final->prismConnectivity.crbegin(); rit != this->final->prismConnectivity.crend()-s*this->numberOfPrismsPerSegment; rit++)
			this->prisms.erase(this->prisms.begin() + rit->back());
		for (auto i = 0u; i < this->prisms.size(); i++)
			this->prisms[i].back() = i;

		for (int p = 0; p < this->numberOfPrismsPerSegment; p++) {
			std::vector<int> firstFacet(4);
			std::copy_if(this->final->prismConnectivity[s*this->numberOfPrismsPerSegment+p].cbegin(), this->final->prismConnectivity[s*this->numberOfPrismsPerSegment+p].cend()-1, firstFacet.begin(), [=](auto& p){return p != this->final->lineConnectivity[s][0] && p != this->final->lineConnectivity[s][1];});

			for (const auto& hexa : this->hexahedra)
				if (hasElement(hexa.cbegin(), hexa.cend()-1, firstFacet[0]))
					if (hasElement(hexa.cbegin(), hexa.cend()-1, firstFacet[1]))
						if (hasElement(hexa.cbegin(), hexa.cend()-1, firstFacet[2]))
							if (hasElement(hexa.cbegin(), hexa.cend()-1, firstFacet[3]))
								this->final->hexahedronConnectivity.push_back(hexa);

			this->hexahedra.erase(this->hexahedra.begin() + this->final->hexahedronConnectivity.back().back());
			for (auto i = 0u; i < this->hexahedra.size(); i++)
				this->hexahedra[i].back() = i;

			for (int h = 0; h < this->numberOfHexahedronsPerRadius-1; h++) {
				std::vector<int> lastFacet(4);
				std::copy_if(this->final->hexahedronConnectivity.back().cbegin(), this->final->hexahedronConnectivity.back().cend()-1, lastFacet.begin(), [=](auto& p){return !hasElement(firstFacet.cbegin(), firstFacet.cend(), p);});

				for (const auto& hexa : this->hexahedra)
					if (hasElement(hexa.cbegin(), hexa.cend()-1, lastFacet[0]))
						if (hasElement(hexa.cbegin(), hexa.cend()-1, lastFacet[1]))
							if (hasElement(hexa.cbegin(), hexa.cend()-1, lastFacet[2]))
								if (hasElement(hexa.cbegin(), hexa.cend()-1, lastFacet[3]))
									this->final->hexahedronConnectivity.push_back(hexa);

				this->hexahedra.erase(this->hexahedra.begin() + this->final->hexahedronConnectivity.back().back());
				for (auto i = 0u; i < this->hexahedra.size(); i++)
					this->hexahedra[i].back() = i;

				firstFacet = lastFacet;
			}
		}
		this->addVertices(s);

		std::cout << "\tsegment \033[1;31m" << s << "\033[0m: " << this->final->lineConnectivity[s][0] << " - " << this->final->lineConnectivity[s][1] << std::endl;

		printf("\n\t\tprismIndices (%i): ", int(this->final->prismConnectivity.size() / (s+1)));
		for (int i = 0; i < this->numberOfPrismsPerSegment; i++)
			std::cout << this->final->prismConnectivity[s*this->numberOfPrismsPerSegment+i].back() << ", ";;

		printf("\n\t\thexahIndices (%i): ", int(this->final->hexahedronConnectivity.size() / (s+1)));
		for (int i = 0; i < this->numberOfHexahedronsPerSegment; i++)
			std::cout << this->final->hexahedronConnectivity[s*this->numberOfHexahedronsPerSegment+i].back() << ", ";

		printf("\n\n");

	}
}

void RadialGridDataReordered::addVertices(int segment) {
	for (int v = 0; v < 2; v++)
		if (!hasElement(this->vertices.cbegin(), this->vertices.cend(), this->final->lineConnectivity[segment][v]))
			this->vertices.push_back(this->final->lineConnectivity[segment][v]);

	for (int h = 0; h < this->numberOfHexahedronsPerSegment; h++)
		for (int v = 0; v < 8; v++)
			if (!hasElement(this->vertices.cbegin(), this->vertices.cend(), this->final->hexahedronConnectivity[segment * this->numberOfHexahedronsPerSegment + h][v]))
				this->vertices.push_back(this->final->hexahedronConnectivity[segment * this->numberOfHexahedronsPerSegment + h][v]);
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

	for (int s = 0; s < this->numberOfSegments; s++) {
		for (int h = 0; h < this->numberOfHexahedronsPerSegment; h++)
			this->final->hexahedronConnectivity[s * this->numberOfHexahedronsPerSegment + h].back() = this->elementShift++;

		for (int p = 0; p < this->numberOfPrismsPerSegment; p++)
			this->final->prismConnectivity[s * this->numberOfPrismsPerSegment + p].back() = this->elementShift++;
	}

	for (int s = 0; s < this->numberOfSegments; s++)
		this->final->lineConnectivity[s].back() = this->elementShift++;
}

RadialGridDataReordered::~RadialGridDataReordered() {
	printf("\n\n");
	printf("\tprisms: %i\n", int(this->prisms.size()));
	printf("\thexahedra: %i\n", int(this->hexahedra.size()));
	printf("\tvertices: %i\n", int(this->vertices.size()));
}
