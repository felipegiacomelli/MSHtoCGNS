#include <chrono>

#include <Utilities/Output.hpp>
#include <Utilities/Print.hpp>
#include <Grid/GridData.hpp>
#include <FileMend/CgnsReader/SpecialCgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <FileMend/WellGenerator.hpp>
#include <FileMend/GridDataExtractor.hpp>
#include <FileMend/RadialGridDataReordered.hpp>
#include <FileMend/MultipleBasesCgnsCreator3D.hpp>

void runWithScript();

void renameZones(GridDataShared gridData, boost::property_tree::ptree) {

}

void createSingleRegion(GridDataShared gridData, std::string regionName) {
	RegionData region;
	region.name = regionName;
	region.elementBegin = 0;
	region.elementEnd = gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size();
	gridData->regions.clear();
	gridData->regions.emplace_back(std::move(region));
}

void applyRatio(GridDataShared gridData, const double& ratio) {
	for (auto& coordinate: gridData->coordinates)
		for (auto& position : coordinate)
			position *= ratio;
}

int main(int argc, char** argv) {
	if (argc > 1) {
		if (std::stoi(argv[1]) == 1)
			runWithScript();
		return 0;
	}

	boost::property_tree::ptree propertyTree;
	boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMender.json", propertyTree);
	// std::string inputPath  = propertyTree.get<std::string>("path.input");
	// std::string outputPath = propertyTree.get<std::string>("path.output");
	std::string inputPath  = std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-Mixed/12523v_57072e.cgns";
	std::string outputPath = "/home/felipe/Downloads/msh_to_cgns/mender/";

	auto start = std::chrono::steady_clock::now();
	SpecialCgnsReader3D reader(inputPath);
	GridDataShared gridData = reader.gridData;
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;
	std::cout << std::endl << "\tGrid path: " << inputPath;
	std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

	printGridDataInformation(gridData);

	{
		propertyTree.clear();
		std::string regionName("WELL_BODY");

		boost::property_tree::ptree wellStart;
		boost::property_tree::ptree x, y, z;
		x.put("",  20.0); wellStart.push_back(std::make_pair("", x));
		y.put("", 125.0); wellStart.push_back(std::make_pair("", y));
		z.put("",  50.0); wellStart.push_back(std::make_pair("", z));

		std::string wellName("LINE_WELL");

		boost::property_tree::ptree region;
		region.put("regionName", regionName);
		region.add_child("wellStart", wellStart);
		region.put("wellName", wellName);

		boost::property_tree::ptree wellRegions;
		wellRegions.push_back(std::make_pair("", region));

		propertyTree.add_child("wellRegions", wellRegions);

		WellGenerator wellGenerator(gridData, propertyTree);
	}
	// WellGenerator wellGenerator(gridData, std::string(SCRIPT_DIRECTORY) + "ScriptWellGenerator.json");

	GridDataShared radialGridData;
	{
		propertyTree.clear();

		boost::property_tree::ptree entities;

		std::vector<std::string> names{"WELL_BODY"};
		for (auto name : names) {
			boost::property_tree::ptree entity;
			entity.put("", name);
			entities.push_back(std::make_pair("", entity));
		}
		propertyTree.add_child("regions", entities);

		entities.clear();

		names = std::vector<std::string>{};
		for (auto name : names) {
			boost::property_tree::ptree entity;
			entity.put("", name);
			entities.push_back(std::make_pair("", entity));
		}
		propertyTree.add_child("boundaries", entities);

		entities.clear();

		names = std::vector<std::string>{"LINE_WELL"};
		for (auto name : names) {
			boost::property_tree::ptree entity;
			entity.put("", name);
			entities.push_back(std::make_pair("", entity));
		}
		propertyTree.add_child("wells", entities);

		GridDataExtractor gridDataExtractor(gridData, propertyTree);
		radialGridData = gridDataExtractor.extract;
	}
	// GridDataExtractor gridDataExtractor(gridData, std::string(SCRIPT_DIRECTORY) + "ScriptGridDataExtractor.json")
	// radialGridData = gridDataExtractor.extract;

	printGridDataInformation(radialGridData);

	RadialGridDataReordered radialGridDataReordered(radialGridData);
	radialGridData = radialGridData;

	start = std::chrono::steady_clock::now();
	MultipleBasesCgnsCreator3D creator({gridData, radialGridData}, {"Reservoir", "Well"}, outputPath);
	end = std::chrono::steady_clock::now();
	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;

	printGridDataInformation(gridData);

	return 0;
}

void runWithScript() {
	boost::property_tree::ptree propertyTree;
	boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMender.json", propertyTree);
	std::string inputPath  = propertyTree.get<std::string>("path.input");
	std::string outputPath = propertyTree.get<std::string>("path.output");

	auto start = std::chrono::steady_clock::now();
	SpecialCgnsReader3D reader(inputPath);
	GridDataShared gridData = reader.gridData;
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;
	std::cout << std::endl << "\tGrid path: " << inputPath;
	std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

	printGridDataInformation(gridData);

	propertyTree = propertyTree.get_child("ScriptWellGenerator");

	WellGenerator wellGenerator(gridData, propertyTree);

	RadialGridDataReordered radialGridDataReordered(gridData);
	gridData = radialGridDataReordered.final;

	printGridDataInformation(gridData);

	start = std::chrono::steady_clock::now();
	CgnsCreator3D creator(gridData, outputPath);
	end = std::chrono::steady_clock::now();
	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;
}