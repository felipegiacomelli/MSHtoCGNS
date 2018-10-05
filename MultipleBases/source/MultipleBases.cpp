#include <chrono>

#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>
#include <MshInterface/MshReader/MshReader3D.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <FileMend/GridDataExtractor.hpp>
#include <Utilities/Print.hpp>

#include <FileMend/MultipleBasesCgnsCreator3D.hpp>

void printGridDataInformation(GridDataShared gridData);

int main() {
	boost::property_tree::ptree script;
	boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMultipleBases.json", script);
	boost::filesystem::path inputPath(script.get<std::string>("path.input"));
	boost::filesystem::path outputPath(script.get<std::string>("path.output"));

	auto start = std::chrono::steady_clock::now();

	GridDataShared gridData;
	if (inputPath.extension() == std::string(".msh")) {
		MshReader3D reader(inputPath.string());
		gridData = reader.gridData;
	}
	else if (inputPath.extension() == std::string(".cgns")) {
		CgnsReader3D reader(inputPath.string());
		gridData = reader.gridData;
	}
	else
		throw std::runtime_error("MultipleBases: file extension " + inputPath.extension().string() + " not supported");

	auto end = std::chrono::steady_clock::now();

	std::chrono::duration<double> elapsedSeconds = end - start;
	std::cout << std::endl << "\tGrid path: " << inputPath.string();
	std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

	printGridDataInformation(gridData);
	printf("\t#############################\n\n");

	GridDataExtractor gridDataExtractor(gridData, std::string(SCRIPT_DIRECTORY) + "ScriptGridDataExtractor.json");

	std::vector<GridDataShared> gridDatas{gridData, gridDataExtractor.extract};
	std::vector<std::string> baseNames{"Rock", "Reservoir"};

	printGridDataInformation(gridDatas.back());
	printf("\t#############################\n");

	start = std::chrono::steady_clock::now();
	CgnsCreator3D creator(gridDataExtractor.extract, outputPath.string());
	// MultipleBasesCgnsCreator3D(gridDatas, baseNames, outputPath.string());
	end = std::chrono::steady_clock::now();

	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << outputPath.string() << std::endl << std::endl;

	return 0;
}

void printGridDataInformation(GridDataShared gridData) {
	std::cout << std::endl << "\t\tnumberOfVertices: " << gridData->coordinates.size() << std::endl;

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
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << region.name  << ": "  << std::setw(6) << std::right << region.elementBegin   << " - " << region.elementsOnRegion.back()   << " | " << region.elementsOnRegion.size();
	std::cout << std::endl;

	std::cout << std::endl << "\t\tnumberOfBoundaries: " << gridData->boundaries.size();
	for (auto boundary : gridData->boundaries)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << boundary.name << ": " << std::setw(6) << std::right << boundary.facetBegin << " - " << boundary.facetsOnBoundary.back() << " | " << boundary.facetsOnBoundary.size();
	std::cout << std::endl;

	std::cout << std::endl << "\t\tnumberOfWells: " << gridData->wells.size();
	for (auto well : gridData->wells)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << well.name     << ": " << std::setw(6) << std::right << well.lineBegin       << " - " << well.linesOnWell.back()       << " | " << well.linesOnWell.size();

	std::cout << std::endl << std::endl;
}