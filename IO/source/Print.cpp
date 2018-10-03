#include <IO/Print.hpp>

void printGridDataInformation(GridDataShared gridData) {
	std::cout << std::endl << "\t\tnumberOfElements: " << gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size();
	std::cout << std::endl << "\t\t\ttetrahedra: " << gridData->tetrahedronConnectivity.size();
	std::cout << std::endl << "\t\t\thexahedra:  " << gridData->hexahedronConnectivity.size();
	std::cout << std::endl << "\t\t\tprisms:     " << gridData->prismConnectivity.size();
	std::cout << std::endl << "\t\t\tpyramids:   " << gridData->pyramidConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\tnumberOfFacets  : " << gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size();
	std::cout << std::endl << "\t\t\ttriangle: " << gridData->triangleConnectivity.size();
	std::cout << std::endl << "\t\t\tquadrangle:  " << gridData->quadrangleConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\tnumberOfLines   : " << gridData->lineConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\ttotal           : " << gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size() + gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size() + gridData->lineConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\tnumberOfRegions: " << gridData->regions.size();
	for (auto region : gridData->regions)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << region.name  << ": "  << std::setw(6) << std::right << region.elementsOnRegion.front()   << " - " << region.elementsOnRegion.back()   << " | " << region.elementsOnRegion.size();
	std::cout << std::endl;

	std::cout << std::endl << "\t\tnumberOfBoundaries: " << gridData->boundaries.size();
	for (auto boundary : gridData->boundaries)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << boundary.name << ": " << std::setw(6) << std::right << boundary.facetsOnBoundary.front() << " - " << boundary.facetsOnBoundary.back() << " | " << boundary.facetsOnBoundary.size() << " | " << boundary.vertices.size();
	std::cout << std::endl;

	std::cout << std::endl << "\t\tnumberOfWells: " << gridData->wells.size();
	for (auto well : gridData->wells)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << well.name     << ": " << std::setw(6) << std::right << well.linesOnWell.front()       << " - " << well.linesOnWell.back()       << " | " << well.linesOnWell.size();

	std::cout << std::endl << std::endl;
}