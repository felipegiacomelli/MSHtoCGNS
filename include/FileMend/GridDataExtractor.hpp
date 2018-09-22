#ifndef GRID_DATA_EXTRACTOR
#define GRID_DATA_EXTRACTOR

#include <set>
#include <algorithm>
#include <numeric>

#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>

struct GridDataExtractorData {
	std::vector<std::string> regions;
	std::vector<std::string> boundaries;
	std::vector<std::string> wells;
};

class GridDataExtractor {
	public:
		GridDataExtractor(GridDataShared original, std::string gridDataExtractorScript);

		~GridDataExtractor() = default;

		GridDataShared extract;

	private:
		void checkGridData();
		void readScript();
		void buildElementConnectivities();
		void extractRegions();
		void extractBoundaries();
		void extractWells();
		void extractVertices();
		void fixIndices();

		GridDataShared original;
		std::string gridDataExtractorScript;
		std::vector<GridDataExtractorData> gridDataExtractorDatum;
		std::vector<std::vector<int>> elementConnectivities;
		std::set<int> vertices;
		int localIndex = 0;
};

#endif