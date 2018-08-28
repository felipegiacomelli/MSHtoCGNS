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

auto buildElementConnectivities(GridDataShared gridData);

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

	// auto elementConnectivities = buildElementConnectivities(gridData);
	// auto regionBegin = elementConnectivities.cbegin() + region.elementsOnRegion.front();
	// auto regionEnd = elementConnectivities.cbegin() + region.elementsOnRegion.back() + 1;

	// for (auto prism : gridData->prismConnectivity) {

	// }

	start = std::chrono::steady_clock::now();
	SpecialCgnsCreator3D creator(gridData, outputPath);
	end = std::chrono::steady_clock::now();
	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;

	return 0;
}

auto buildElementConnectivities(GridDataShared gridData) {
	std::vector<std::vector<int>> elementConnectivities;

	// 3D
	for (auto i = gridData->tetrahedronConnectivity.cbegin(); i != gridData->tetrahedronConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x;});
	}
	for (auto i = gridData->hexahedronConnectivity.cbegin(); i != gridData->hexahedronConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x;});
	}
	for (auto i = gridData->prismConnectivity.cbegin(); i != gridData->prismConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x;});
	}
	for (auto i = gridData->pyramidConnectivity.cbegin(); i != gridData->pyramidConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x;});
	}

	// 2D
	for (auto i = gridData->triangleConnectivity.cbegin(); i != gridData->triangleConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x;});
	}
	for (auto i = gridData->quadrangleConnectivity.cbegin(); i != gridData->quadrangleConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x;});
	}

	// 1D
	for (auto i = gridData->lineConnectivity.cbegin(); i != gridData->lineConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x;});
	}

	std::stable_sort(elementConnectivities.begin(), elementConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

	for (unsigned i = 0; i < elementConnectivities.size(); i++)
		elementConnectivities[i].pop_back();

	return elementConnectivities;
}