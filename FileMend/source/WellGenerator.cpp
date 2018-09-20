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

	std::stable_sort(this->elementConnectivities.begin(), this->elementConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

	for (unsigned i = 0; i < this->elementConnectivities.size(); i++)
		this->elementConnectivities[i].pop_back();
}

void WellGenerator::generateWells() {
	this->lineConnectivityShift = this->elementConnectivities.size() + this->gridData->triangleConnectivity.size() + this->gridData->quadrangleConnectivity.size();

	for (auto wellGeneratorData : this->wellGeneratorDatum) {

		auto wellRegion = std::find_if(this->gridData->regions.cbegin(), this->gridData->regions.cend(), [=](auto r){return r.name == wellGeneratorData.regionName;});

		auto regionBegin = this->elementConnectivities.cbegin() + wellRegion->elementsOnRegion.front();
		auto regionEnd = this->elementConnectivities.cbegin() + wellRegion->elementsOnRegion.back() + 1;

		std::vector<std::vector<int>> prisms;
		for (auto element = regionBegin; element != regionEnd; element++)
			if (element->size() == 6u)
				prisms.emplace_back(*element);
		this->numberOfPrisms = prisms.size();

		for (const auto& prism : prisms)
			for (const auto& index : prism)
				if (isClose(this->gridData->coordinates[index], wellGeneratorData.wellStart))
					this->currentIndex = index;

		this->defineQuantities(prisms);

		std::vector<int> vertices;
		vertices.push_back(this->currentIndex);
		for (int k = 0; k < numberOfSegments; k++) {
			std::set<int> wellStartPrisms;
			for (auto i = 0u; i < prisms.size(); i++)
				if (std::find(prisms[i].cbegin(), prisms[i].cend(), this->currentIndex) != prisms[i].cend())
					wellStartPrisms.insert(i);

			std::set<int> wellStartVertices;
			for (const auto& prismIndex : wellStartPrisms)
				wellStartVertices.insert(prisms[prismIndex].cbegin(), prisms[prismIndex].cend());

			std::unordered_map<int, int> map;
			for (const auto& vertexIndex : wellStartVertices)
				for (const auto& prismIndex : wellStartPrisms)
					if (std::find(prisms[prismIndex].cbegin(), prisms[prismIndex].cend(), vertexIndex) != prisms[prismIndex].cend())
						map[vertexIndex]++;

			this->currentIndex = std::find_if(map.cbegin(), map.cend(), [=](auto entry){return entry.first != this->currentIndex && entry.second == numberOfElementsPerSection;})->first;

			for (auto rit = wellStartPrisms.crbegin(); rit != wellStartPrisms.crend(); rit++)
				prisms.erase(prisms.begin() + *rit);

			vertices.push_back(this->currentIndex);
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

void WellGenerator::defineQuantities(std::vector<std::vector<int>> prisms) {
	std::set<int> wellStartPrisms;
	for (auto i = 0u; i < prisms.size(); i++)
		if (std::find(prisms[i].cbegin(), prisms[i].cend(), currentIndex) != prisms[i].cend())
			wellStartPrisms.insert(i);

	this->numberOfElementsPerSection = wellStartPrisms.size();
	this->numberOfSegments = this->numberOfPrisms / this->numberOfElementsPerSection;
}

bool WellGenerator::isClose(const std::array<double, 3>& coordinate, const std::array<double, 3>& referencePoint) {
	bool close = true;

	for (int i = 0; i < 3; i++)
		close &= std::abs(coordinate[i] - referencePoint[i]) < TOLERANCE;

	return close;
}