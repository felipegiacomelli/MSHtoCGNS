#ifndef WELL_GENERATOR_HPP
#define WELL_GENERATOR_HPP

#include <set>
#include <algorithm>
#include <numeric>

#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Utilities/Vector.hpp>
#include <Grid/GridData.hpp>

#define TOLERANCE 1e-3

template<typename T>
void print(const std::array<T, 3>& coordinate) {
	for (int i = 0; i < 3; i++)
		std::cout << "\t" << coordinate[i];
}

struct WellGeneratorData {
	std::string regionName;
	std::array<double, 3> wellStart;
	int wellDirection;
	std::string wellName;
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
		std::vector<WellGeneratorData> wellGeneratorDatum;
		std::vector<std::vector<int>> elementConnectivities;
		int numberOfElements;
		int numberOfFacets;
		int lineConnectivityShift;
};

#endif