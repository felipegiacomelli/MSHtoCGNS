#ifndef RADIAL_GRID_DATA_REORDERED
#define RADIAL_GRID_DATA_REORDERED

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>

class RadialGridDataReordered {
	public:
		RadialGridDataReordered(GridDataShared gridData, std::string radialGridDataReorderedScript);

		~RadialGridDataReordered() = default;

		GridDataShared extract;

	private:
		void checkGridData();
		// void readScript();
		// void buildElementConnectivities();
		// void extractRegions();
		// void extractBoundaries();
		// void extractWells();
		// void extractVertices();
		// void fixIndices();

		GridDataShared gridData;
		std::string radialGridDataReorderedScript;
		std::vector<std::vector<int>> elementConnectivities;
		std::set<int> vertices;
		int localIndex = 0;
};

#endif