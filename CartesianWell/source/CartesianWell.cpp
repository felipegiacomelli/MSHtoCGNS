#include <chrono>
#include <iomanip>

#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>

void printGridDataInformation(GridDataShared gridData);
void printWellsInformation(GridDataShared gridData);

bool isClose(const std::array<double, 3>& coordinate, const std::array<double, 3>& wellStart, int wellDirection);

int main() {
	boost::property_tree::ptree propertyTree;
	boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptCartesianWell.json", propertyTree);
	std::string inputPath  = propertyTree.get<std::string>("path.input");
	std::string outputPath = propertyTree.get<std::string>("path.output");

	auto start = std::chrono::steady_clock::now();
	CgnsReader3D reader(inputPath);
	GridDataShared gridData = reader.gridData;
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;
	std::cout << std::endl << "\tGrid path: " << inputPath;
	std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

	printGridDataInformation(gridData);

		std::array<double, 3> wellStart{0.5, 0.5, 0.0};
		int wellDirection = 2;
		std::string wellName("Well");

		std::set<int> indices;
		for (auto hexahedron : gridData->hexahedronConnectivity)
			for (auto index : hexahedron)
				if (isClose(gridData->coordinates[index], wellStart, wellDirection))
					indices.insert(index);

		std::vector<std::pair<int, std::array<double, 3>>> vertices;
		for (auto index = indices.cbegin(); index != indices.cend(); index++)
			vertices.emplace_back(std::make_pair(*index, gridData->coordinates[*index]));

		std::stable_sort(vertices.begin(), vertices.end(), [=](auto a, auto b) {return a.second[wellDirection] < b.second[wellDirection];});

		unsigned numberOfLines = vertices.size() - 1;
		int lineConnectivityShift = gridData->hexahedronConnectivity.size() + gridData->quadrangleConnectivity.size();

		for (unsigned i = 0; i < numberOfLines; i++)
			gridData->lineConnectivity.emplace_back(std::array<int, 3>{vertices[i].first, vertices[i+1].first, int(i) + lineConnectivityShift});

		WellData well;
		well.name = wellName;
		well.linesOnWell.resize(numberOfLines);
		std::iota(well.linesOnWell.begin(), well.linesOnWell.end(), lineConnectivityShift);
		for (auto vertex : vertices)
			well.vertices.emplace_back(vertex.first);
		gridData->wells.emplace_back(std::move(well));

	start = std::chrono::steady_clock::now();
	CgnsCreator3D creator(gridData, outputPath);
	end = std::chrono::steady_clock::now();
	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;

	printGridDataInformation(gridData);
	printWellsInformation(gridData);

	return 0;
}

bool isClose(const std::array<double, 3>& coordinate, const std::array<double, 3>& wellStart, int wellDirection) {
	bool close = true;

	for (int i = 0; i < 3; i++)
		if (i != wellDirection)
			close &= std::abs(coordinate[i] - wellStart[i]) < 1e-8;

	return close;
}

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
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << region.name  << ": "  << std::setw(6) << std::right << region.elementBegin << " - " << region.elementsOnRegion.back()   << " | " << region.elementsOnRegion.size();
	std::cout << std::endl;

	std::cout << std::endl << "\t\tnumberOfBoundaries: " << gridData->boundaries.size();
	for (auto boundary : gridData->boundaries)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << boundary.name << ": " << std::setw(6) << std::right << boundary.facetBegin << " - " << std::setw(6) << std::right << boundary.facetsOnBoundary.back() << " | " << boundary.facetsOnBoundary.size();
	std::cout << std::endl;

	std::cout << std::endl << "\t\tnumberOfWells: " << gridData->wells.size();
	for (auto well : gridData->wells)
		std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << well.name << ": " << std::setw(6) << std::right << well.lineBegin << " - " << well.linesOnWell.back() << " | " << well.linesOnWell.size();

	std::cout << std::endl << std::endl;
}

void printWellsInformation(GridDataShared gridData) {
	std::cout << std::endl << "\tWell: " << gridData->wells.front().name << std::endl;

	std::cout << std::endl << "\t\tLines " << std::endl;
	for (auto line : gridData->lineConnectivity) {
		std::cout << "\t";
		for (auto index : line)
			std::cout << "\t" << index;
		std::cout << std::endl;
	}

	std::cout << std::endl << "\t\tVertices " << std::endl;
	for (auto vertex : gridData->wells.front().vertices) {
		std::cout << "\t\t" << vertex << ":";
		for (auto i = 0; i < 3; i++)
			std::cout << "\t" << gridData->coordinates[vertex][i];
		std::cout <<std::endl;
	}

	std::cout << std::endl << std::endl;
}