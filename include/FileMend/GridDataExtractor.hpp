#ifndef GRID_DATA_EXTRACTOR
#define GRID_DATA_EXTRACTOR

#include <Grid/GridData.hpp>

struct GridDataExtractorData {
	std::string regionName;
	std::array<double, 3> wellStart;
	int wellDirection;
	std::string wellName;
};

class GridDataExtractor {
	GridDataExtractor() = default;

	~GridDataExtractor() = default;
};

#endif