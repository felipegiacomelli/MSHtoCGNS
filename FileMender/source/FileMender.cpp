#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>
#include <SpecialCgnsReader3D.hpp>
#include <SpecialCgnsCreator3D.hpp>
#include <Output.hpp>

void buildElementConnectivities(GridDataShared gridData);

int main() {
	boost::property_tree::ptree iroot;
	boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMender.json", iroot);
	std::string inputPath  = iroot.get<std::string>("path.input");
	std::string outputPath = iroot.get<std::string>("path.output");

	auto start = std::chrono::steady_clock::now();
	SpecialCgnsReader3D reader(inputPath);
	GridDataShared  gridData = reader.gridData;
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;
	std::cout << std::endl << "\tGrid path: " << inputPath;
	std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

	std::cout << std::endl << "numberOfElements: " << gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size();
	std::cout << std::endl << "numberOfFacets  : " << gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size();
	std::cout << std::endl << "numberOfLines   : " << gridData->lineConnectivity.size();
	std::cout << std::endl << "total           : " << gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size() + gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size() + gridData->lineConnectivity.size() << std::endl;

	for (auto region : gridData->regions)
		std::cout << std::endl << std::setw(15) << std::left << region.name  << ": "  << std::setw(6) << std::right << region.elementsOnRegion.front()   << " - " << region.elementsOnRegion.back()   << " | " << region.elementsOnRegion.size();

	std::cout << std::endl;

	for (auto boundary : gridData->boundaries)
		std::cout << std::endl << std::setw(15) << std::left << boundary.name << ": " << std::setw(6) << std::right << boundary.facetsOnBoundary.front() << " - " << boundary.facetsOnBoundary.back() << " | " << boundary.facetsOnBoundary.size();

	std::cout << std::endl;

	for (auto well : gridData->wells)
		std::cout << std::endl << std::setw(15) << std::left << well.name     << ": " << std::setw(6) << std::right << well.elementsOnWell.front()       << " - " << well.elementsOnWell.back()       << " | " << well.elementsOnWell.size();

	std::cout << std::endl << std::endl;

	start = std::chrono::steady_clock::now();
	SpecialCgnsCreator3D creator(gridData, outputPath);
	end = std::chrono::steady_clock::now();
	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;

	return 0;
}

void buildElementConnectivities(GridDataShared gridData) {
	std::vector<std::vector<int>> elementConnectivities;

	// 3D
	for (auto i = gridData->tetrahedronConnectivity.cbegin(); i != gridData->tetrahedronConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = gridData->hexahedronConnectivity.cbegin(); i != gridData->hexahedronConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = gridData->prismConnectivity.cbegin(); i != gridData->prismConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = gridData->pyramidConnectivity.cbegin(); i != gridData->pyramidConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x + 1;});
	}

	// 2D
	for (auto i = gridData->triangleConnectivity.cbegin(); i != gridData->triangleConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = gridData->quadrangleConnectivity.cbegin(); i != gridData->quadrangleConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x + 1;});
	}

	// 1D
	for (auto i = gridData->lineConnectivity.cbegin(); i != gridData->lineConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x + 1;});
	}

	std::stable_sort(elementConnectivities.begin(), elementConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

	// for (unsigned i = 0; i < elementConnectivities.size(); i++)
	// 	elementConnectivities[i].back() = i;
}