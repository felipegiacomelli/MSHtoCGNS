#include <FileMend/WellGenerator.hpp>
#include <Utilities/Print.hpp>

WellGenerator::WellGenerator(GridDataShared gridData, std::string wellGeneratorScript) : gridData(gridData), wellGeneratorScript(wellGeneratorScript) {
	this->checkGridData();
	this->readScript();
	this->buildElementConnectivities();
	this->generateWells();
}

void WellGenerator::checkGridData() {
	if (this->gridData->wells.size() != 0u)
		throw std::runtime_error("WellGenerator: Number of wells in gridData must be 0 and not " + std::to_string(this->gridData->wells.size()));

	if (this->gridData->lineConnectivity.size() != 0u)
		throw std::runtime_error("WellGenerator: Number of lines in gridData must be 0 and not " + std::to_string(this->gridData->lineConnectivity.size()));
}

void WellGenerator::readScript() {
	boost::property_tree::ptree iroot;
	boost::property_tree::read_json(this->wellGeneratorScript, iroot);

	for (const auto& wellRegion : iroot.get_child("wellRegions")) {
		this->wellGeneratorDatum.emplace_back();

		this->wellGeneratorDatum.back().regionName = wellRegion.second.get<std::string>("regionName");

		int index = 0;
		for (const auto& coordinate : wellRegion.second.get_child("wellStart")) {
    		this->wellGeneratorDatum.back().wellStart[index] = coordinate.second.get_value<double>();
    		index++;
		}

		this->wellGeneratorDatum.back().wellDirection = wellRegion.second.get<int>("wellDirection");

		this->wellGeneratorDatum.back().wellName = wellRegion.second.get<std::string>("wellName");
    }
}

void WellGenerator::buildElementConnectivities() {
	for (auto i = this->gridData->tetrahedronConnectivity.cbegin(); i != this->gridData->tetrahedronConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x;});
	}
	for (auto i = this->gridData->hexahedronConnectivity.cbegin(); i != this->gridData->hexahedronConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x;});
	}
	for (auto i = this->gridData->prismConnectivity.cbegin(); i != this->gridData->prismConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x;});
	}
	for (auto i = this->gridData->pyramidConnectivity.cbegin(); i != this->gridData->pyramidConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x;});
	}

	for (auto i = this->gridData->triangleConnectivity.cbegin(); i != this->gridData->triangleConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x;});
	}
	for (auto i = this->gridData->quadrangleConnectivity.cbegin(); i != this->gridData->quadrangleConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x;});
	}

	std::stable_sort(this->elementConnectivities.begin(), this->elementConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

	for (unsigned i = 0; i < this->elementConnectivities.size(); i++)
		this->elementConnectivities[i].pop_back();
}

void WellGenerator::generateWells() {
	this->lineConnectivityShift = this->elementConnectivities.size();

	for (auto wellGeneratorData : this->wellGeneratorDatum) {

		auto wellRegion = std::find_if(this->gridData->regions.cbegin(), this->gridData->regions.cend(), [=](auto r){return r.name == wellGeneratorData.regionName;});

		auto regionBegin = this->elementConnectivities.cbegin() + wellRegion->elementsOnRegion.front();
		auto regionEnd = this->elementConnectivities.cbegin() + wellRegion->elementsOnRegion.back() + 1;

		std::vector<int> vertices;

		std::vector<std::vector<int>> prisms;
		for (auto element = regionBegin; element != regionEnd; element++)
			if (element->size() == 6u)
				prisms.emplace_back(*element);

		int currentIndex = -1;
		for (const auto& prism : prisms)
			for (const auto& index : prism)
				if (isClose(this->gridData->coordinates[index], wellGeneratorData.wellStart))
					currentIndex = index;

		vertices.push_back(currentIndex);

		int numberOfElementsPerSection;
		int numberOfSegments;
		int numberOfVerticesPerSection;
		int numberOfPrisms = prisms.size();
		{
			std::set<int> wellStartPrisms;
			for (auto i = 0u; i < prisms.size(); i++)
				if (std::find(prisms[i].cbegin(), prisms[i].cend(), currentIndex) != prisms[i].cend())
					wellStartPrisms.insert(i);

			numberOfElementsPerSection = wellStartPrisms.size();

			numberOfSegments = numberOfPrisms / numberOfElementsPerSection;

			std::set<int> wellStartVertices;
			for (const auto& prismIndex : wellStartPrisms)
				wellStartVertices.insert(prisms[prismIndex].cbegin(), prisms[prismIndex].cend());

			numberOfVerticesPerSection = wellStartVertices.size();
		}
		// printf("\n\n\tnumberOfPrisms: %i", numberOfPrisms);
		// printf("\n\n\tnumberOfElementsPerSection: %i", numberOfElementsPerSection);
		// printf("\n\n\tnumberOfSegments: %i", numberOfSegments);
		// printf("\n\n\tnumberOfVerticesPerSection: %i\n", numberOfVerticesPerSection);

		for (int k = 0; k < numberOfSegments; k++) {

			std::set<int> wellStartPrisms;
			for (auto i = 0u; i < prisms.size(); i++)
				if (std::find(prisms[i].cbegin(), prisms[i].cend(), currentIndex) != prisms[i].cend())
					wellStartPrisms.insert(i);

			std::set<int> wellStartVertices;
			for (const auto& prismIndex : wellStartPrisms)
				wellStartVertices.insert(prisms[prismIndex].cbegin(), prisms[prismIndex].cend());

			std::unordered_map<int, int> map;
			for (const auto& vertexIndex : wellStartVertices)
				for (const auto& prismIndex : wellStartPrisms)
					if (std::find(prisms[prismIndex].cbegin(), prisms[prismIndex].cend(), vertexIndex) != prisms[prismIndex].cend())
						map[vertexIndex]++;

			int nextIndex = std::find_if(map.cbegin(), map.cend(), [=](auto entry){return entry.first != currentIndex && entry.second == numberOfElementsPerSection;})->first;

			// printf("\n\n\tPrisms on section\n");
			// for (const auto& prismIndex : wellStartPrisms)
			// 	std::cout << "\t" << prismIndex;

			// printf("\n\n\tcurrentIndex: \033[1;32m %i \033[0m - %i - [%3.f, %3.f, %3.f,]"  , currentIndex, map[currentIndex], gridData->coordinates[currentIndex][0], gridData->coordinates[currentIndex][1], gridData->coordinates[currentIndex][2]);
			// printf("\n\n\tnextIndex   : \033[1;33m %i \033[0m - %i - [%3.f, %3.f, %3.f,]\n", nextIndex, map[nextIndex], gridData->coordinates[nextIndex][0], gridData->coordinates[nextIndex][1], gridData->coordinates[nextIndex][2]);

			// for (const auto& prismIndex : wellStartPrisms) {
			// 	printf("\n");
			// 	for (const auto& index : prisms[prismIndex])
			// 		if (index == currentIndex)
			// 			std::cout << "\t\033[1;32m" << index <<"\033[0m";
			// 		else if (index == nextIndex)
			// 			std::cout << "\t\033[1;33m" << index <<"\033[0m";
			// 		else
			// 			std::cout << "\t" << index;
			// }

			currentIndex = nextIndex;

			for (auto rit = wellStartPrisms.crbegin(); rit != wellStartPrisms.crend(); rit++)
				prisms.erase(prisms.begin() + *rit);

			vertices.push_back(currentIndex);
		}

    	unsigned numberOfLines = vertices.size() - 1;

		for (unsigned i = 0; i < numberOfLines; i++)
			this->gridData->lineConnectivity.emplace_back(std::array<int, 3>{vertices[i], vertices[i+1], int(i) + this->lineConnectivityShift});

		std::stable_sort(vertices.begin(), vertices.end());

		WellData well;
		well.name = wellGeneratorData.wellName;
		well.linesOnWell.resize(numberOfLines);
		std::iota(well.linesOnWell.begin(), well.linesOnWell.end(), this->lineConnectivityShift);
		well.vertices = std::move(vertices);
		this->gridData->wells.emplace_back(std::move(well));

		this->lineConnectivityShift +=  numberOfLines;
    }
}

bool WellGenerator::isClose(const std::array<double, 3>& coordinate, const std::array<double, 3>& referencePoint) {
	bool close = true;

	for (int i = 0; i < 3; i++)
		close &= std::abs(coordinate[i] - referencePoint[i]) < TOLERANCE;

	return close;
}