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

bool WellGenerator::isClose(const std::array<double, 3>& coordinate, const std::array<double, 3>& wellStart, int wellDirection) {
	bool close = true;

	for (int i = 0; i < 3; i++)
		if (i != wellDirection)
			close &= std::abs(coordinate[i] - wellStart[i]) < TOLERANCE;

	return close;
}

void WellGenerator::generateWells() {
	this->lineConnectivityShift = this->elementConnectivities.size();

	for (auto wellGeneratorData : this->wellGeneratorDatum) {

    	std::cout << "\t" << wellGeneratorData.regionName << std::endl;
    	for (int i = 0; i < 3; i++)
    		std::cout << "\t" << wellGeneratorData.wellStart[i];
    	std::cout << std::endl << "\t" << wellGeneratorData.wellDirection << std::endl;

		RegionData radialGridRegion;
		for (auto region : this->gridData->regions)
			if (region.name == wellGeneratorData.regionName)
				radialGridRegion = region;

		auto regionBegin = this->elementConnectivities.cbegin() + radialGridRegion.elementsOnRegion.front();
		auto regionEnd = this->elementConnectivities.cbegin() + radialGridRegion.elementsOnRegion.back() + 1;

		std::vector<std::vector<int>> prismsOnRegion;
		for (auto element = regionBegin; element != regionEnd; element++)
			if (element->size() == 6u)
				prismsOnRegion.emplace_back(*element);

		// std::cout << std::endl << "\tprismsOnRegion: " << prismsOnRegion.size() << std::endl;

		std::set<int> wellIndices;
		for (auto& prism : prismsOnRegion)
			for (auto& index : prism)
				if (isClose(this->gridData->coordinates[index], wellGeneratorData.wellStart, wellGeneratorData.wellDirection))
					wellIndices.insert(index);

		std::vector<std::pair<int, std::array<double, 3>>> wellVertices;
		for (auto index = wellIndices.cbegin(); index != wellIndices.cend(); index++)
			wellVertices.emplace_back(std::make_pair(*index, this->gridData->coordinates[*index]));

		std::stable_sort(wellVertices.begin(), wellVertices.end(), [=](const auto& a, const auto& b) {return a.second[wellGeneratorData.wellDirection] < b.second[wellGeneratorData.wellDirection];});
		unsigned numberOfLine = wellVertices.size() - 1;

		// std::cout << std::endl << "\tsorted well vertices: " << wellIndices.size() << std::endl;
		// for (auto& vertex : wellVertices) {
		// 	std::cout << "\t" << vertex.first;
		// 	print(vertex.second);
		// 	std::cout << std::endl;
		// }

		for (unsigned i = 0; i < numberOfLine; i++)
			this->gridData->lineConnectivity.emplace_back(std::array<int, 3>{wellVertices[i].first, wellVertices[i+1].first, int(i) + this->lineConnectivityShift});

		// std::cout << std::endl << "\twell line connectivity - " << lineConnectivity.size() << std::endl;
		// for (auto& line : lineConnectivity) {
		// 	print(line);
		// 	std::cout << std::endl;
		// }

		WellData well;
		well.name = wellGeneratorData.wellName;
		well.linesOnWell.resize(numberOfLine);
		std::iota(well.linesOnWell.begin(), well.linesOnWell.end(), this->lineConnectivityShift);
		this->gridData->wells.emplace_back(std::move(well));

		this->lineConnectivityShift +=  numberOfLine;

		for (auto i : well.linesOnWell)
			std::cout << "\t" << i;
		std::cout << std::endl << std::endl;
    }
}