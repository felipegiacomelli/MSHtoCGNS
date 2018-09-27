#include <FileMend/RadialGridDataReordered.hpp>
#include <cgnslib.h>

RadialGridDataReordered::RadialGridDataReordered(GridDataShared gridData) : gridData(gridData) {
	this->checkGridData();
	this->defineQuantities();
	this->copyElements();
	this->final = MakeShared<GridData>();
	this->final->dimension = 3;
	this->reorder();
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

		std::cout << "\tsegment \033[1;31m" << s << "\033[0m: " << this->final->lineConnectivity[s][0] << " - " << this->final->lineConnectivity[s][1] << std::endl;

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
		this->fixIndices(s);

		printf("\n\t\tprismIndices (%i): ", int(this->final->prismConnectivity.size() / (s+1)));
		for (int i = 0; i < this->numberOfPrismsPerSegment; i++)
			std::cout << this->final->prismConnectivity[s*this->numberOfPrismsPerSegment+i].back() << ", ";;

		printf("\n\t\thexahIndices (%i): ", int(this->final->hexahedronConnectivity.size() / (s+1)));
		for (int i = 0; i < this->numberOfHexahedronsPerSegment; i++)
			std::cout << this->final->hexahedronConnectivity[s*this->numberOfHexahedronsPerSegment+i].back() << ", ";

		printf("\n\n");

	}
}

void RadialGridDataReordered::fixIndices(int segment) {
	std::set<int> vertices;
	vertices.insert(this->final->lineConnectivity[segment][0]);
	vertices.insert(this->final->lineConnectivity[segment][1]);
	for (int h = 0; h < this->numberOfHexahedronsPerSegment; h++)
		for (int v = 0; v < 8; v++)
			vertices.insert(this->final->hexahedronConnectivity[segment * this->numberOfHexahedronsPerSegment + h][v]);

	std::unordered_map<int, int> originalToFinal;
	int index = 0;
	for (auto vertex : vertices)
		originalToFinal[vertex] = index++;

	for (int h = 0; h < this->numberOfHexahedronsPerSegment; h++)
		for (int v = 0; v < 8; v++)
			this->final->hexahedronConnectivity[segment * this->numberOfHexahedronsPerSegment + h][v] = originalToFinal[this->final->hexahedronConnectivity[segment * this->numberOfHexahedronsPerSegment + h][v]];

	for (int p = 0; p < this->numberOfPrismsPerSegment; p++)
		for (int v = 0; v < 6; v++)
			this->final->prismConnectivity[segment * this->numberOfPrismsPerSegment + p][v] = originalToFinal[this->final->prismConnectivity[segment * this->numberOfPrismsPerSegment + p][v]];

	this->final->lineConnectivity[segment][0] = originalToFinal[this->final->lineConnectivity[segment][0]]; this->final->lineConnectivity[segment][1] = originalToFinal[this->final->lineConnectivity[segment][1]];
}

RadialGridDataReordered::~RadialGridDataReordered() {
	printf("\n\n");
	printf("\tprisms: %i\n", int(this->prisms.size()));
	printf("\thexahedra: %i\n", int(this->hexahedra.size()));
}
