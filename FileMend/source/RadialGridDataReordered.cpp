#include <FileMend/RadialGridDataReordered.hpp>
#include <cgnslib.h>

RadialGridDataReordered::RadialGridDataReordered(GridDataShared gridData, std::string radialGridDataReorderedScript) : gridData(gridData), radialGridDataReorderedScript(radialGridDataReorderedScript) {
	this->checkGridData();
}

void RadialGridDataReordered::checkGridData() {
	if (this->gridData->dimension != 3)
		throw std::runtime_error("WellGenerator: gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

	if (this->gridData->tetrahedronConnectivity.size() != 0u)
		throw std::runtime_error("WellGenerator: gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

	if (this->gridData->hexahedronConnectivity.size() == 0u)
		throw std::runtime_error("WellGenerator: gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

	if (this->gridData->prismConnectivity.size() == 0u)
		throw std::runtime_error("WellGenerator: gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

	if (this->gridData->pyramidConnectivity.size() != 0u)
		throw std::runtime_error("WellGenerator: gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));
}