#include <FileMend/WellGenerator.hpp>

WellGenerator::WellGenerator(GridDataShared gridData, std::string wellGeneratorScript) : gridData(gridData), wellGeneratorScript(wellGeneratorScript) {
	this->checkGridData();
	this->readScript();
	this->numberOfElements = this->gridData->tetrahedronConnectivity.size() + this->gridData->hexahedronConnectivity.size() + this->gridData->prismConnectivity.size() + this->gridData->pyramidConnectivity.size();
	this->numberOfFacets = this->gridData->triangleConnectivity.size() + this->gridData->quadrangleConnectivity.size();
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

    //
    	this->wellGeneratorDatum.pop_back();
    	this->wellGeneratorDatum.pop_back();
    //
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

		std::vector<std::vector<int>> prisms;
		for (auto element = regionBegin; element != regionEnd; element++)
			if (element->size() == 6u)
				prisms.emplace_back(*element);

		int numberOfPrisms = prisms.size();
		printf("\n\n\tnumberOfPrisms: %i\n", numberOfPrisms);

		int wellStartIndex = -1;
		for (const auto& prism : prisms)
			for (const auto& index : prism)
				if (isClose(this->gridData->coordinates[index], wellGeneratorData.wellStart))
					wellStartIndex = index;

		printf("\n\n\twellStartIndex: %i\n", wellStartIndex);
		for (auto coordinate : this->gridData->coordinates[wellStartIndex])
			std::cout << "\t" << coordinate;

		std::set<int> wellStartPrisms; int k = 0;
		for (const auto& prism : prisms) {
			if (std::find(prism.cbegin(), prism.cend(), wellStartIndex) != prism.cend())
				wellStartPrisms.insert(k);
			k++;
		}

		int numberOfElementsPerSection = wellStartPrisms.size();

		printf("\n\n\tnumberOfElementsPerSection: %i\n", numberOfElementsPerSection);
		for (const auto& prismIndex : wellStartPrisms)
			std::cout << "\t" << prismIndex;

		printf("\n");

		for (const auto& prismIndex : wellStartPrisms) {
			printf("\n");
			for (const auto& index : prisms[prismIndex])
				if (index == wellStartIndex)
					std::cout << "\t\033[1;31m" << index <<"\033[0m";
				else
					std::cout << "\t" << index;
		}

		std::set<int> wellStartVertices;
		for (const auto& prismIndex : wellStartPrisms)
			wellStartVertices.insert(prisms[prismIndex].cbegin(), prisms[prismIndex].cend());

		int numberOfVerticesPerSection = wellStartVertices.size();

		printf("\n\n\tnumberOfVerticesPerSection: %i\n", numberOfVerticesPerSection);

		std::unordered_map<int, int> map;
		// for (const auto& vertexIndex : wellStartVertices)
		// 	map[vertexIndex] = 0;

		for (const auto& vertexIndex : wellStartVertices) {
			int l = 0;
			for (const auto& prismIndex : wellStartPrisms)
				if (std::find(prisms[prismIndex].cbegin(), prisms[prismIndex].cend(), vertexIndex) != prisms[prismIndex].cend())
					l++;
			map[vertexIndex] = l;
		}

		for (auto value : map)
			std::cout << "\t" << value.first << " : " << value.second << std::endl;

		printf("\n\t##########################################\n");

		// std::vector<std::pair<int, std::array<double, 3>>> vertices;
		// for (auto index = indices.cbegin(); index != indices.cend(); index++)
		// 	vertices.emplace_back(std::make_pair(*index, this->gridData->coordinates[*index]));

		// std::stable_sort(vertices.begin(), vertices.end(), [=](const auto& a, const auto& b) {return a.second[wellGeneratorData.wellDirection] < b.second[wellGeneratorData.wellDirection];});
		// unsigned numberOfLines = vertices.size() - 1;

		// for (unsigned i = 0; i < numberOfLines; i++)
		// 	this->gridData->lineConnectivity.emplace_back(std::array<int, 3>{vertices[i].first, vertices[i+1].first, int(i) + this->lineConnectivityShift});

		// WellData well;
		// well.name = wellGeneratorData.wellName;
		// well.linesOnWell.resize(numberOfLines);
		// std::iota(well.linesOnWell.begin(), well.linesOnWell.end(), this->lineConnectivityShift);
		// for (auto index : indices)
		// 	well.vertices.emplace_back(index);
		// this->gridData->wells.emplace_back(std::move(well));

		// this->lineConnectivityShift +=  numberOfLines;
    }
}

void WellGenerator::generateWells(int i) {
	this->lineConnectivityShift = this->elementConnectivities.size();

	for (auto wellGeneratorData : this->wellGeneratorDatum) {

		auto wellRegion = std::find_if(this->gridData->regions.cbegin(), this->gridData->regions.cend(), [=](auto r){return r.name == wellGeneratorData.regionName;});

		auto regionBegin = this->elementConnectivities.cbegin() + wellRegion->elementsOnRegion.front();
		auto regionEnd = this->elementConnectivities.cbegin() + wellRegion->elementsOnRegion.back() + 1;

		std::vector<std::vector<int>> prisms;
		for (auto element = regionBegin; element != regionEnd; element++)
			if (element->size() == 6u)
				prisms.emplace_back(*element);

		std::set<int> indices;
		for (const auto& prism : prisms)
			for (const auto& index : prism)
				if (isClose(this->gridData->coordinates[index], wellGeneratorData.wellStart, wellGeneratorData.wellDirection))
					indices.insert(index);

		std::vector<std::pair<int, std::array<double, 3>>> vertices;
		for (auto index = indices.cbegin(); index != indices.cend(); index++)
			vertices.emplace_back(std::make_pair(*index, this->gridData->coordinates[*index]));

		std::stable_sort(vertices.begin(), vertices.end(), [=](const auto& a, const auto& b) {return a.second[wellGeneratorData.wellDirection] < b.second[wellGeneratorData.wellDirection];});
		unsigned numberOfLines = vertices.size() - 1;

		for (unsigned i = 0; i < numberOfLines; i++)
			this->gridData->lineConnectivity.emplace_back(std::array<int, 3>{vertices[i].first, vertices[i+1].first, int(i) + this->lineConnectivityShift});

		WellData well;
		well.name = wellGeneratorData.wellName;
		well.linesOnWell.resize(numberOfLines);
		std::iota(well.linesOnWell.begin(), well.linesOnWell.end(), this->lineConnectivityShift);
		for (auto index : indices)
			well.vertices.emplace_back(index);
		this->gridData->wells.emplace_back(std::move(well));

		this->lineConnectivityShift +=  numberOfLines;
    }
}

bool WellGenerator::isClose(const std::array<double, 3>& coordinate, const std::array<double, 3>& wellStart, int wellDirection) {
	bool close = true;

	for (int i = 0; i < 3; i++)
		if (i != wellDirection)
			close &= std::abs(coordinate[i] - wellStart[i]) < TOLERANCE;

	return close;
}

bool WellGenerator::isClose(const std::array<double, 3>& coordinate, const std::array<double, 3>& referencePoint) {
	bool close = true;

	for (int i = 0; i < 3; i++)
		close &= std::abs(coordinate[i] - referencePoint[i]) < TOLERANCE;

	return close;
}