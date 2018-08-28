#ifndef WELL_GENERATOR_HPP
#define WELL_GENERATOR_HPP

#include <set>
#include <algorithm>
#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Utilities/Vector.hpp>
#include <Grid/GridData.hpp>

#define TOLERANCE 1e-3

struct WellGeneratorData {
	std::string regionName;
	std::array<double, 3> wellStart;
	int wellDirection;
};

class WellGenerator {
	public:
		WellGenerator(GridDataShared gridData, std::string wellGeneratorScript);

		~WellGenerator() = default;

	private:
		void checkGridData();
		void readScript();
		void buildElementConnectivities();
		bool isClose(const std::array<double, 3>& coordinate, const std::array<double, 3>& wellStart, int wellDirection);
		void generateWells();

		GridDataShared gridData;
		std::string wellGeneratorScript;
		std::vector<std::vector<int>> elementConnectivities;
};

// WellData generateWell(const std::vector<std::vector<int>>& elementConnectivities, GridDataShared gridData, const std::string& regionName, const std::array<double, 3>& wellStart, int wellDirection) {
// 	std::cout << std::endl << regionName << std::endl;

// 	RegionData radialGridRegion;
// 	for (auto region : gridData->regions)
// 		if (region.name == regionName)
// 			radialGridRegion = region;

// 	auto regionBegin = elementConnectivities.cbegin() + radialGridRegion.elementsOnRegion.front();
// 	auto regionEnd = elementConnectivities.cbegin() + radialGridRegion.elementsOnRegion.back() + 1;

// 	std::vector<std::vector<int>> prismsOnRegion;
// 	for (auto element = regionBegin; element != regionEnd; element++)
// 		if (element->size() == 6u)
// 			prismsOnRegion.emplace_back(*element);

// 	std::cout << std::endl << "\tprismsOnRegion: " << prismsOnRegion.size() << std::endl;

// 	std::set<int> wellIndices;
// 	for (auto& prism : prismsOnRegion)
// 		for (auto& index : prism)
// 			if (isClose(gridData->coordinates[index], wellStart, wellDirection))
// 				wellIndices.insert(index);

// 	std::cout << std::endl << "\twellIndices: " << wellIndices.size() << std::endl;

// 	std::vector<std::pair<int, std::array<double, 3>>> wellVertices;
// 	for (auto index = wellIndices.cbegin(); index != wellIndices.cend(); index++)
// 		wellVertices.emplace_back(std::make_pair(*index, gridData->coordinates[*index]));

// 	std::cout << std::endl << "\tunsorted well vertices" << std::endl;

// 	for (auto& vertex : wellVertices) {
// 		std::cout << "\t" << vertex.first;
// 		print(vertex.second);
// 		std::cout << std::endl;
// 	}

// 	std::stable_sort(wellVertices.begin(), wellVertices.end(), [=](const auto& a, const auto& b) {return a.second[wellDirection] < b.second[wellDirection];});

// 	std::cout << std::endl << "\tsorted well vertices" << std::endl;

// 	for (auto& vertex : wellVertices) {
// 		std::cout << "\t" << vertex.first;
// 		print(vertex.second);
// 		std::cout << std::endl;
// 	}

// 	std::vector<std::array<int, 3>> lineConnectivity;
// 	for (unsigned i = 0u; i < wellVertices.size() - 1u; i++)
// 		lineConnectivity.emplace_back(std::array<int, 3>{wellVertices[i].first, wellVertices[i+1].first, int(i)});

// 	std::cout << std::endl << "\twell line connectivity" << std::endl;

// 	for (auto& line : lineConnectivity) {
// 		print(line);
// 		std::cout << std::endl;
// 	}

// 	WellData wellData;
// 	return wellData;
// }

#endif