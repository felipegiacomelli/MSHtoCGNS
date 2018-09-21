#include <chrono>

#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>
#include <IO/Output.hpp>
#include <IO/MshReader2D.hpp>
#include <IO/MshReader3D.hpp>
#include <Utilities/Print.hpp>

#include <MultipleBasesCgnsCreator3D.hpp>

void printGridDataInformation(GridDataShared gridData);
GridDataShared extractGridDataEntities(GridDataShared gridData, boost::property_tree::ptree script);
std::vector<std::vector<int>> buildElementConnectivities(GridDataShared gridData);

int main() {
	boost::property_tree::ptree script;
	boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMultipleBases.json", script);
	std::string inputPath(script.get<std::string>("path.input"));
	std::string outputPath(script.get<std::string>("path.output"));

	auto start = std::chrono::steady_clock::now();
	MshReader3D reader(inputPath);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;
	std::cout << std::endl << "\tGrid path: " << inputPath;
	std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

	printGridDataInformation(reader.gridData);
	printf("#############################\n");

	std::vector<GridDataShared> gridDatas{reader.gridData, extractGridDataEntities(reader.gridData, script)};
	std::vector<std::string> zoneNames{"Rock", "Reservoir"};

	for (auto i = 0 ; i < gridDatas.back()->coordinates.size(); i++) {
		printf("\n");
		for (int j = 0; j < 3; j++) {
			printf("checkClose(extract->coordinates[%i][%i], %.4e, TOLERANCE);\t", i, j, gridDatas.back()->coordinates[i][j]);
		}
	}

	printf("\n\n");
	printGridDataInformation(gridDatas.front());
	printf("#############################\n");
	printGridDataInformation(gridDatas.back());

	MultipleBasesCgnsCreator3D creator(gridDatas, zoneNames, outputPath);
	end = std::chrono::steady_clock::now();
	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;

	return 0;
}

GridDataShared extractGridDataEntities(GridDataShared gridData, boost::property_tree::ptree script) {
	auto extract(MakeShared<GridData>());
	extract->dimension = gridData->dimension;

	auto elementConnectivities = buildElementConnectivities(gridData);
	std::set<int> vertices;
	int localIndex = 0;

	std::vector<std::string> regions;
	for (auto region : script.get_child("regions"))
		regions.emplace_back(region.second.get_value<std::string>());

	for (auto name : regions) {
		auto iterator(std::find_if(gridData->regions.cbegin(),gridData->regions.cend(), [=](auto r){return r.name == name;}));
		if (iterator == gridData->regions.cend())
			throw std::runtime_error("extractGridDataEntities: There is no region " + name + " in gridData");
		auto region(*iterator);

		auto regionBegin = elementConnectivities.begin() + region.elementsOnRegion.front();
		auto regionEnd = elementConnectivities.begin() + region.elementsOnRegion.back() + 1;

		std::iota(region.elementsOnRegion.begin(), region.elementsOnRegion.end(), localIndex);

		// printf("\n\n"); print(region.elementsOnRegion, "elementsOnRegion");

		for (auto element = regionBegin; element != regionEnd; element++) {

			for (auto vertex  : *element)
				vertices.insert(vertex);

			element->push_back(localIndex);

			switch (element->size()) {
				case 5: {
					std::array<int, 5> tetrahedron;
					std::copy_n(element->cbegin(), 5, std::begin(tetrahedron));
					extract->tetrahedronConnectivity.emplace_back(std::move(tetrahedron));
					break;
				}
				case 9: {
					std::array<int, 9> hexahedron;
					std::copy_n(element->cbegin(), 9, std::begin(hexahedron));
					extract->hexahedronConnectivity.emplace_back(std::move(hexahedron));
					break;
				}
				case 7: {
					std::array<int, 7> prism;
					std::copy_n(element->cbegin(), 7, std::begin(prism));
					extract->prismConnectivity.emplace_back(std::move(prism));
					break;
				}
				case 6: {
					std::array<int, 6> pyramid;
					std::copy_n(element->cbegin(), 6, std::begin(pyramid));
					extract->pyramidConnectivity.emplace_back(std::move(pyramid));
					break;
				}
			}

			localIndex++;

		}

		extract->regions.emplace_back(region);
	}

	std::vector<std::string> boundaries;
	for (auto boundary : script.get_child("boundaries"))
		boundaries.emplace_back(boundary.second.get_value<std::string>());

	for (auto name : boundaries) {
		auto iterator(std::find_if(gridData->boundaries.cbegin(),gridData->boundaries.cend(), [=](auto b){return b.name == name;}));
		if (iterator == gridData->boundaries.cend())
			throw std::runtime_error("extractGridDataEntities: There is no boundary " + name + " in gridData");
		auto boundary(*iterator);
		gridData->boundaries.erase(iterator);

		auto boundaryBegin = elementConnectivities.begin() + boundary.facetsOnBoundary.front();
		auto boundaryEnd = elementConnectivities.begin() + boundary.facetsOnBoundary.back() + 1;

		std::iota(boundary.facetsOnBoundary.begin(), boundary.facetsOnBoundary.end(), localIndex);

		// printf("\n\n"); print(boundary.facetsOnBoundary, "facetsOnBoundary");

		for (auto facet = boundaryBegin; facet != boundaryEnd; facet++) {

			facet->push_back(localIndex);

			switch (facet->size()) {
				case 4: {
					std::array<int, 4> triangle;
					std::copy_n(facet->cbegin(), 4, std::begin(triangle));
					extract->triangleConnectivity.emplace_back(std::move(triangle));
					break;
				}
				case 5: {
					std::array<int, 5> quadrangle;
					std::copy_n(facet->cbegin(), 5, std::begin(quadrangle));
					extract->quadrangleConnectivity.emplace_back(std::move(quadrangle));
					break;
				}
			}

			localIndex++;

		}

		extract->boundaries.emplace_back(boundary);
	}

	for (auto vertex : vertices)
		extract->coordinates.push_back(gridData->coordinates[vertex]);

	printf("\n"); for (auto vertex : vertices) std::cout << "\t" << vertex;  printf("\n\n");

	printf("\n"); print(extract->hexahedronConnectivity.cbegin(), extract->hexahedronConnectivity.cend(), "hexas"); printf("\n");
	printf("\n"); print(extract->quadrangleConnectivity.cbegin(), extract->quadrangleConnectivity.cend(), "quads"); printf("\n");

	std::unordered_map<int, int> globalToLocal;
	int index = 0;
	for (auto vertex : vertices) {
		globalToLocal[vertex] = index;
		index++;
	}

	// printf("\n"); for (auto map : globalToLocal) std::cout << "\t" << map.first << " - " << map.second << std::endl;  printf("\n\n");

	for (auto& hexahedron : extract->hexahedronConnectivity)
		for (auto vertex = hexahedron.begin(); vertex != hexahedron.end() - 1; vertex++)
			*vertex = globalToLocal[*vertex];

	for (auto& quadrangle : extract->quadrangleConnectivity)
		for (auto vertex = quadrangle.begin(); vertex != quadrangle.end() - 1; vertex++)
			*vertex = globalToLocal[*vertex];

	printf("\n"); print(extract->hexahedronConnectivity.cbegin(), extract->hexahedronConnectivity.cend(), "hexas"); printf("\n");
	printf("\n"); print(extract->quadrangleConnectivity.cbegin(), extract->quadrangleConnectivity.cend(), "quads"); printf("\n");

	return extract;
}



std::vector<std::vector<int>> buildElementConnectivities(GridDataShared gridData) {
	std::vector<std::vector<int>> elementConnectivities;

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

	for (auto i = gridData->triangleConnectivity.cbegin(); i != gridData->triangleConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x;});
	}
	for (auto i = gridData->quadrangleConnectivity.cbegin(); i != gridData->quadrangleConnectivity.cend(); i++) {
		elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(elementConnectivities.back()), [](auto x){return x;});
	}

	std::stable_sort(elementConnectivities.begin(), elementConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

	for (unsigned i = 0; i < elementConnectivities.size(); i++)
		elementConnectivities[i].pop_back();

	return elementConnectivities;
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