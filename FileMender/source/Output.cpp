#include <Output.hpp>

void output(GridDataShared gridData, std::string fileName) {
	std::ofstream file(fileName, std::ofstream::out);

	file << std::endl << "### Coordinates - " << gridData->coordinates.size() << std::endl;
	for (auto i = gridData->coordinates.cbegin(); i != gridData->coordinates.cend(); i++) {
		for (auto j = i->cbegin(); j != i->cend(); j++)
			file << "\t" << std::fixed << std::setprecision(8) << std::setw(10) << std::right << *j;
		file << std::endl;
	}

	int numberOfElements;
	int numberOfFacets;
	switch (gridData->dimension) {
		case 2: {
			if (gridData->triangleConnectivity.size() > 0) {
				file << std::endl << "### Triangle connectivity - " << gridData->triangleConnectivity.size() << std::endl;
				output(gridData->triangleConnectivity.cbegin(), gridData->triangleConnectivity.cend(), file);
			}
			if (gridData->quadrangleConnectivity.size() > 0) {
				file << std::endl << "### Quadrangle connectivity - " << gridData->quadrangleConnectivity.size() << std::endl;
				output(gridData->quadrangleConnectivity.cbegin(), gridData->quadrangleConnectivity.cend(), file);
			}
			if (gridData->lineConnectivity.size() > 0) {
				file << std::endl << "### Line connectivity - " << gridData->lineConnectivity.size() << std::endl;
				output(gridData->lineConnectivity.cbegin(), gridData->lineConnectivity.cend(), file);
			}
			numberOfElements = gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size();
			numberOfFacets = gridData->lineConnectivity.size();
			break;
		}
		case 3: {
			if (gridData->tetrahedronConnectivity.size() > 0) {
				file << std::endl << "### Tetrahedron connectivity - " << gridData->tetrahedronConnectivity.size() << std::endl;
				output(gridData->tetrahedronConnectivity.cbegin(), gridData->tetrahedronConnectivity.cend(), file);
			}
			if (gridData->hexahedronConnectivity.size() > 0) {
				file << std::endl << "### Hexahedron connectivity - " << gridData->hexahedronConnectivity.size() << std::endl;
				output(gridData->hexahedronConnectivity.cbegin(), gridData->hexahedronConnectivity.cend(), file);
			}
			if (gridData->triangleConnectivity.size() > 0) {
				file << std::endl << "### Triangle connectivity - " << gridData->triangleConnectivity.size() << std::endl;
				output(gridData->triangleConnectivity.cbegin(), gridData->triangleConnectivity.cend(), file);
			}
			if (gridData->quadrangleConnectivity.size() > 0) {
				file << std::endl << "### Quadrangle connectivity - " << gridData->quadrangleConnectivity.size() << std::endl;
				output(gridData->quadrangleConnectivity.cbegin(), gridData->quadrangleConnectivity.cend(), file);
			}
			numberOfElements = gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size();
			numberOfFacets = gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size();
			break;
		}
		default:
			throw std::runtime_error("IO/Output: GridData dimension must be either 2 or 3");
	}


	file << std::endl << "### Boundaries - " << gridData->boundaries.size() << std::endl;
	for (auto& boundary : gridData->boundaries) {
		file << std::endl << "\t" << boundary.name << std::endl;
		for (auto j = boundary.facetsOnBoundary.cbegin(); j != boundary.facetsOnBoundary.cend(); j++)
			file << "\t" << *j;
		file << std::endl;
	}

	file << std::endl << "### Regions - " << gridData->regions.size() << std::endl;
	for (auto& region : gridData->regions) {
		file << std::endl << "\t" << region.name << std::endl;
		for (auto j = region.elementsOnRegion.cbegin(); j != region.elementsOnRegion.cend(); j++)
			file << "\t" << *j;
		file << std::endl;
	}

	file << std::endl;
	file << "GridData ###"        << std::endl;
	file << "Number of element: " << std::setw(3) << std::right << numberOfElements << std::endl;
	file << "Number of facets: "  << std::setw(3) << std::right << numberOfFacets   << std::endl;

	file.close();
}

void printGridDataInformation(GridDataShared gridData) {
	std::cout << std::endl << "numberOfElements: " << gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size();
	std::cout << std::endl << "\ttetrahedra: " << gridData->tetrahedronConnectivity.size();
	std::cout << std::endl << "\thexahedra:  " << gridData->hexahedronConnectivity.size();
	std::cout << std::endl << "\tprisms:     " << gridData->prismConnectivity.size();
	std::cout << std::endl << "\tpyramids:   " << gridData->pyramidConnectivity.size() << std::endl;

	std::cout << std::endl << "numberOfFacets  : " << gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size();
	std::cout << std::endl << "\ttriangle: " << gridData->tetrahedronConnectivity.size();
	std::cout << std::endl << "\tquadrangle:  " << gridData->hexahedronConnectivity.size() << std::endl;

	std::cout << std::endl << "numberOfLines   : " << gridData->lineConnectivity.size() << std::endl;

	std::cout << std::endl << "total           : " << gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size() + gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size() + gridData->lineConnectivity.size() << std::endl;

	std::cout << std::endl << "numberOfRegions: " << gridData->regions.size();
	for (auto region : gridData->regions)
		std::cout << std::endl << std::setw(20) << std::left << region.name  << ": "  << std::setw(6) << std::right << region.elementsOnRegion.front()   << " - " << region.elementsOnRegion.back()   << " | " << region.elementsOnRegion.size();
	std::cout << std::endl;

	std::cout << std::endl << "numberOfBoundaries: " << gridData->boundaries.size();
	for (auto boundary : gridData->boundaries)
		std::cout << std::endl << std::setw(20) << std::left << boundary.name << ": " << std::setw(6) << std::right << boundary.facetsOnBoundary.front() << " - " << boundary.facetsOnBoundary.back() << " | " << boundary.facetsOnBoundary.size();
	std::cout << std::endl;

	std::cout << std::endl << "numberOfWells: " << gridData->wells.size();
	for (auto well : gridData->wells)
		std::cout << std::endl << std::setw(20) << std::left << well.name     << ": " << std::setw(6) << std::right << well.linesOnWell.front()       << " - " << well.linesOnWell.back()       << " | " << well.linesOnWell.size();

	std::cout << std::endl << std::endl;
}