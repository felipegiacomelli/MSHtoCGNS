#include <IO/Output.hpp>

void outputGridData2D(GridDataShared gridData, std::string fileName) {
	std::ofstream file(fileName, std::ofstream::out);
	int numberOfNodes = gridData->coordinates.size();
	int numberOfElements = gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size();
	int numberOfFacets = gridData->lineConnectivity.size();

	file << "GridData ###"        << std::endl;
	file << "Number of nodes:   " << std::setw(3) << std::right << numberOfNodes    << std::endl;
	file << "Number of element: " << std::setw(3) << std::right << numberOfElements << std::endl;
	file << "Number of facets: "  << std::setw(3) << std::right << numberOfFacets   << std::endl;

	file << std::endl << "Coordinates" << std::endl;
	for (auto i = gridData->coordinates.cbegin(); i != gridData->coordinates.cend(); i++) {
		for (auto j = i->cbegin(); j != i->cend(); j++)
			file << "\t" << std::fixed << std::setprecision(8) << std::setw(10) << std::right << *j;
		file << std::endl;
	}

	if (gridData->triangleConnectivity.size() > 0) {
		file << std::endl << "Triangle connectivity" << std::endl;
		for (auto i = gridData->triangleConnectivity.cbegin(); i != gridData->triangleConnectivity.cend(); i++) {
			for (auto j = i->cbegin(); j != i->cend(); j++)
				file << "\t" << std::setw(3) << std::right << *j;
			file << std::endl;
		}
	}
	if (gridData->quadrangleConnectivity.size() > 0) {
		file << std::endl << "Quadrangle connectivity" << std::endl;
		for (auto i = gridData->quadrangleConnectivity.cbegin(); i != gridData->quadrangleConnectivity.cend(); i++) {
			for (auto j = i->cbegin(); j != i->cend(); j++)
				file << "\t" << std::setw(3) << std::right << *j;
			file << std::endl;
		}
	}
	if (gridData->lineConnectivity.size() > 0) {
		file << std::endl << "Line connectivity" << std::endl;
		for (auto i = gridData->lineConnectivity.cbegin(); i != gridData->lineConnectivity.cend(); i++) {
			for (auto j = i->cbegin(); j != i->cend(); j++)
				file << "\t" << std::setw(3) << std::right << *j;
			file << std::endl;
		}
	}

	file << std::endl << "Boundaries ###" << std::endl;
	for (auto i = gridData->boundaries.cbegin(); i != gridData->boundaries.cend(); i ++) {
		file << std::endl << "\t" << i->name << std::endl;
		for (auto j = i->facetsOnBoundary.cbegin(); j != i->facetsOnBoundary.cend(); j++)
			file << "\t" << *j;
		file << std::endl;

	}

	file << std::endl << "Regions ###" << std::endl;
	for (auto i = gridData->regions.cbegin(); i != gridData->regions.cend(); i ++) {
		file << std::endl << "\t" << i->name << std::endl;
		for (auto j = i->elementsOnRegion.cbegin(); j != i->elementsOnRegion.cend(); j++)
			file << "\t" << *j;
		file << std::endl;
	}

	file.close();
}

void outputGridData3D(GridDataShared gridData, std::string fileName) {
	std::ofstream file(fileName, std::ofstream::out);
	int numberOfNodes = gridData->coordinates.size();
	int numberOfElements = gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size();
	int numberOfFacets = gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size();

	file << "GridData ###"        << std::endl;
	file << "Number of nodes:   " << std::setw(3) << std::right << numberOfNodes    << std::endl;
	file << "Number of element: " << std::setw(3) << std::right << numberOfElements << std::endl;
	file << "Number of facets: "  << std::setw(3) << std::right << numberOfFacets   << std::endl;

	file << std::endl << "Coordinates" << std::endl;
	for (auto i = gridData->coordinates.cbegin(); i != gridData->coordinates.cend(); i++) {
		for (auto j = i->cbegin(); j != i->cend(); j++)
			file << "\t" << std::fixed << std::setprecision(8) << std::setw(10) << std::right << *j;
		file << std::endl;
	}

	if (gridData->tetrahedronConnectivity.size() > 0) {
		file << std::endl << "Tetrahedron connectivity" << std::endl;
		for (auto i = gridData->tetrahedronConnectivity.cbegin(); i != gridData->tetrahedronConnectivity.cend(); i++) {
			for (auto j = i->cbegin(); j != i->cend(); j++)
				file << "\t" << std::setw(3) << std::right << *j;
			file << std::endl;
		}
	}
	if (gridData->hexahedronConnectivity.size() > 0) {
		file << std::endl << "Hexahedron connectivity" << std::endl;
		for (auto i = gridData->hexahedronConnectivity.cbegin(); i != gridData->hexahedronConnectivity.cend(); i++) {
			for (auto j = i->cbegin(); j != i->cend(); j++)
				file << "\t" << std::setw(3) << std::right << *j;
			file << std::endl;
		}
	}
	if (gridData->triangleConnectivity.size() > 0) {
		file << std::endl << "Triangle connectivity" << std::endl;
		for (auto i = gridData->triangleConnectivity.cbegin(); i != gridData->triangleConnectivity.cend(); i++) {
			for (auto j = i->cbegin(); j != i->cend(); j++)
				file << "\t" << std::setw(3) << std::right << *j;
			file << std::endl;
		}
	}
	if (gridData->quadrangleConnectivity.size() > 0) {
		file << std::endl << "Quadrangle connectivity" << std::endl;
		for (auto i = gridData->quadrangleConnectivity.cbegin(); i != gridData->quadrangleConnectivity.cend(); i++) {
			for (auto j = i->cbegin(); j != i->cend(); j++)
				file << "\t" << std::setw(3) << std::right << *j;
			file << std::endl;
		}
	}

	file << std::endl << "Boundaries ###" << std::endl;
	for (auto i = gridData->boundaries.cbegin(); i != gridData->boundaries.cend(); i ++) {
		file << std::endl << "\t" << i->name << std::endl;
		for (auto j = i->facetsOnBoundary.cbegin(); j != i->facetsOnBoundary.cend(); j++)
			file << "\t" << *j;
		file << std::endl;

	}

	file << std::endl << "Regions ###" << std::endl;
	for (auto i = gridData->regions.cbegin(); i != gridData->regions.cend(); i ++) {
		file << std::endl << "\t" << i->name << std::endl;
		for (auto j = i->elementsOnRegion.cbegin(); j != i->elementsOnRegion.cend(); j++)
			file << "\t" << *j;
		file << std::endl;
	}

	file.close();
}