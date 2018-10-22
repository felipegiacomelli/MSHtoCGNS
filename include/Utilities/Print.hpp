#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include <Grid/GridData.hpp>

template<typename T>
void print(const std::vector<T>& a, std::string&& message) {
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "\t" << message << std::endl << "\t";
	for (auto i = a.cbegin(); i != a.cend(); i++)
		std::cout << "\t" << *i;
	std::cout << std::endl;
}

template<typename InputIt>
void print(InputIt cbegin, InputIt cend, std::string&& message) {
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "\t" << message << std::endl << std::endl;
	for (auto i = cbegin; i != cend; i++) {
		for (auto j = i->cbegin(); j != i->cend(); j++) {
			std::cout << "\t" << std::setw(3) << std::right <<*j;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void printGridDataInformation(boost::shared_ptr<GridData> gD, std::string message) {
	std::cout << std::endl << "\t" << message << std::endl;

	int numberOfElements = gD->tetrahedronConnectivity.size() + gD->hexahedronConnectivity.size() + gD->prismConnectivity.size() + gD->pyramidConnectivity.size();
	std::cout << std::endl << "\t\tnumberOfElements: " << numberOfElements;
	std::cout << std::endl << "\t\t\ttetrahedra: " << std::setw(6) << std::setfill(' ') << std::right << gD->tetrahedronConnectivity.size();
	std::cout << std::endl << "\t\t\thexahedra : " << std::setw(6) << std::setfill(' ') << std::right << gD->hexahedronConnectivity.size();
	std::cout << std::endl << "\t\t\tprisms    : " << std::setw(6) << std::setfill(' ') << std::right << gD->prismConnectivity.size();
	std::cout << std::endl << "\t\t\tpyramids  : " << std::setw(6) << std::setfill(' ') << std::right << gD->pyramidConnectivity.size() << std::endl;

	int numberOfFacets = gD->triangleConnectivity.size() + gD->quadrangleConnectivity.size();
	std::cout << std::endl << "\t\tnumberOfFacets  : " << gD->triangleConnectivity.size() + gD->quadrangleConnectivity.size();
	std::cout << std::endl << "\t\t\ttriangle  : " << std::setw(6) << std::setfill(' ') << std::right << gD->triangleConnectivity.size();
	std::cout << std::endl << "\t\t\tquadrangle: " << std::setw(6) << std::setfill(' ') << std::right << gD->quadrangleConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\tnumberOfLines: " << gD->lineConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\ttotal: " << numberOfElements + numberOfFacets + gD->lineConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\tnumberOfRegions: " << gD->regions.size();
	for (auto r : gD->regions)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << r.name << ": " << std::setw(6) << std::right << r.elementBegin << " - " << r.elementEnd << "\t|" << std::setw(6) << std::right << r.elementEnd - r.elementBegin;
	std::cout << std::endl;

	std::cout << std::endl << "\t\tnumberOfBoundaries: " << gD->boundaries.size();
	for (auto b : gD->boundaries)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << b.name << ": " << std::setw(6) << std::right << b.facetBegin << " - " << b.facetEnd << "\t|" << std::setw(6) << std::right << b.facetEnd - b.facetBegin;
	std::cout << std::endl;

	std::cout << std::endl << "\t\tnumberOfWells: " << gD->wells.size();
	for (auto w : gD->wells)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << w.name << ": " << std::setw(6) << std::right << w.lineBegin << " - " << w.lineEnd << "\t|" << std::setw(6) << std::right << w.lineEnd - w.lineBegin;

	std::cout << std::endl << std::endl;
}

#endif