#define BOOST_TEST_MODULE FileMendTestModule

#include <BoostInterface/Test.hpp>

#include <chrono>

#include <Grid/GridData.hpp>
#include <FileMend/SpecialCgnsReader3D.hpp>
#include <FileMend/SpecialCgnsCreator3D.hpp>
#include <FileMend/WellGenerator.hpp>

void printGridDataInformation(GridDataShared gridData) {
	std::cout << std::endl << "\t\tnumberOfElements: " << gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size();
	std::cout << std::endl << "\t\t\ttetrahedra: " << gridData->tetrahedronConnectivity.size();
	std::cout << std::endl << "\t\t\thexahedra:  " << gridData->hexahedronConnectivity.size();
	std::cout << std::endl << "\t\t\tprisms:     " << gridData->prismConnectivity.size();
	std::cout << std::endl << "\t\t\tpyramids:   " << gridData->pyramidConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\tnumberOfFacets  : " << gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size();
	std::cout << std::endl << "\t\t\ttriangle: " << gridData->tetrahedronConnectivity.size();
	std::cout << std::endl << "\t\t\tquadrangle:  " << gridData->hexahedronConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\tnumberOfLines   : " << gridData->lineConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\ttotal           : " << gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size() + gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size() + gridData->lineConnectivity.size() << std::endl;

	std::cout << std::endl << "\t\tnumberOfRegions: " << gridData->regions.size();
	for (auto region : gridData->regions)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << region.name  << ": "  << std::setw(6) << std::right << region.elementsOnRegion.front()   << " - " << region.elementsOnRegion.back()   << " | " << region.elementsOnRegion.size();
	std::cout << std::endl;

	std::cout << std::endl << "\t\tnumberOfBoundaries: " << gridData->boundaries.size();
	for (auto boundary : gridData->boundaries)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << boundary.name << ": " << std::setw(6) << std::right << boundary.facetsOnBoundary.front() << " - " << boundary.facetsOnBoundary.back() << " | " << boundary.facetsOnBoundary.size();
	std::cout << std::endl;

	std::cout << std::endl << "\t\tnumberOfWells: " << gridData->wells.size();
	for (auto well : gridData->wells)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << well.name     << ": " << std::setw(6) << std::right << well.linesOnWell.front()       << " - " << well.linesOnWell.back()       << " | " << well.linesOnWell.size();

	std::cout << std::endl << std::endl;
}

struct FileMendFixture {
	FileMendFixture() {
		boost::property_tree::ptree iroot;
		boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMender.json", iroot);
		this->inputPath  = iroot.get<std::string>("path.input");
		this->outputPath = iroot.get<std::string>("path.output");
		this->wellGeneratorScript = std::string(SCRIPT_DIRECTORY) + "ScriptWellGenerator.json";
	}

	~FileMendFixture() = default;

	std::string inputPath;
	std::string outputPath;
	std::string wellGeneratorScript;
};

FixtureTestSuite(FileMendSuite, FileMendFixture)

TestCase(FileMendTest) {
	auto start = std::chrono::steady_clock::now();
	SpecialCgnsReader3D reader(this->inputPath);
	GridDataShared  gridData = reader.gridData;
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;
	std::cout << std::endl << "\tGrid path: " << this->inputPath;
	std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

	// printGridDataInformation(gridData);

	WellGenerator wellGenerator(gridData, this->wellGeneratorScript);

	// printGridDataInformation(gridData);

	start = std::chrono::steady_clock::now();
	SpecialCgnsCreator3D creator(gridData, this->outputPath);
	end = std::chrono::steady_clock::now();
	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;
}

TestSuiteEnd()