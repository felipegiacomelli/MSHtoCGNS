#include <FileMend/RadialGridDataReordered.hpp>
#include <Utilities/Print.hpp>
#include <cgnslib.h>

RadialGridDataReordered::RadialGridDataReordered(GridDataShared gridData) : gridData(gridData) {
	this->checkGridData();
	this->defineQuantities();
	this->createReordered();
	this->copyData();
	this->reorder();
	this->copyVertices();
	// this->fixIndices();
}

void RadialGridDataReordered::checkGridData() {
	if (this->gridData->dimension != 3)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

	if (this->gridData->tetrahedronConnectivity.size() != 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData tetrahedronConnectivity size must be 0 and not " + std::to_string(this->gridData->tetrahedronConnectivity.size()));

	if (this->gridData->hexahedronConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData hexahedronConnectivity size must not be 0");

	if (this->gridData->prismConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData prismConnectivity size must not be 0");

	if (this->gridData->pyramidConnectivity.size() != 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData pyramidConnectivity size must be 0 and not " + std::to_string(this->gridData->pyramidConnectivity.size()));

	if (this->gridData->triangleConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData triangleConnectivity size must not be 0");

	if (this->gridData->quadrangleConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData quadrangleConnectivity size must not be 0");

	if (this->gridData->lineConnectivity.size() == 0u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData lineConnectivity size must not be 0");

	if (this->gridData->boundaries.size() != 3u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData boundaries size must be 1 and not " + std::to_string(this->gridData->boundaries.size()));

	if (this->gridData->regions.size() != 1u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData regions size must be 1 and not " + std::to_string(this->gridData->regions.size()));

	if (this->gridData->wells.size() != 1u)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData wells size must be 1 and not " + std::to_string(this->gridData->wells.size()));
}

void RadialGridDataReordered::createReordered() {
	this->reordered = MakeShared<GridData>();
	this->reordered->dimension = this->gridData->dimension;

	this->reordered->triangleConnectivity = this->gridData->triangleConnectivity;
	this->reordered->quadrangleConnectivity = this->gridData->quadrangleConnectivity;
	this->reordered->lineConnectivity = this->gridData->lineConnectivity;

	this->reordered->boundaries = this->gridData->boundaries;

	this->reordered->regions = this->gridData->regions;

	this->reordered->wells = this->gridData->wells;
}

void RadialGridDataReordered::defineQuantities() {
	this->numberOfSegments = this->gridData->lineConnectivity.size();

	this->numberOfPrismsPerSegment = this->gridData->prismConnectivity.size() / this->numberOfSegments;
	this->numberOfHexahedronsPerSegment = this->gridData->hexahedronConnectivity.size() / this->numberOfSegments;
	this->numberOfHexahedronsPerRadius = this->numberOfHexahedronsPerSegment / this->numberOfPrismsPerSegment;
	this->numberOfVerticesPerSection = this->gridData->coordinates.size() / (this->gridData->lineConnectivity.size()+1);

	printf("\n\tnumberOfSegments             : %4i", this->numberOfSegments);
	printf("\n\tnumberOfPrisms               : %4i", int(this->gridData->prismConnectivity.size()));
	printf("\n\tnumberOfHexahedrons          : %4i", int(this->gridData->hexahedronConnectivity.size()));
	printf("\n\tnumberOfPrismsPerSegment     : %4i", this->numberOfPrismsPerSegment);
	printf("\n\tnumberOfHexahedronsPerSegment: %4i", this->numberOfHexahedronsPerSegment);
	printf("\n\tnumberOfHexahedronsPerRadius : %4i", this->numberOfHexahedronsPerRadius);
	printf("\n\tnumberOfVerticesPerSection   : %4i", this->numberOfVerticesPerSection);
	printf("\n\n");
}

void RadialGridDataReordered::copyData() {
	for (auto i = 0u; i < this->gridData->coordinates.size(); i++)
		this->coordinates.emplace_back(std::make_pair(i, this->gridData->coordinates[i]));

	this->hexahedra = this->gridData->hexahedronConnectivity;
	for (auto i = 0u; i < this->hexahedra.size(); i++)
		this->hexahedra[i].back() = i;

	this->prisms = this->gridData->prismConnectivity;
	for (auto i = 0u; i < this->prisms.size(); i++)
		this->prisms[i].back() = i;
}

void RadialGridDataReordered::reorder() {
	auto boundary = *std::find_if(this->gridData->boundaries.cbegin(), this->gridData->boundaries.cend(), [=](auto b){return hasElement(b.vertices.cbegin(), b.vertices.cend(), this->gridData->lineConnectivity.front()[0]);});
	std::cout << std::endl << "\t" + boundary.name << std::endl << std::endl;

	std::cout << "\t" << std::setw(4) << std::right << this->gridData->lineConnectivity.front()[0] << std::endl << "\t";

	for (auto vertex : boundary.vertices)
		std::cout << std::setw(4) << std::right << vertex;

	std::vector<std::array<int, 4>> triangles;
	for (auto i = this->gridData->triangleConnectivity.cbegin(); i != this->gridData->triangleConnectivity.cend(); i++)
		if (i->back() >= boundary.facetBegin && i->back() < boundary.facetEnd)
			triangles.emplace_back(*i);

	std::vector<std::array<int, 5>> quadrangles;
	for (auto i = this->gridData->quadrangleConnectivity.cbegin(); i != this->gridData->quadrangleConnectivity.cend(); i++)
		if (i->back() >= boundary.facetBegin && i->back() < boundary.facetEnd)
			quadrangles.emplace_back(*i);

	// printf("\n\n");
	// print(triangles.cbegin(), triangles.cend(), "triangles " + std::to_string(triangles.size()));
	// print(quadrangles.cbegin(), quadrangles.cend(), "quadrangles " + std::to_string(quadrangles.size()));

	for (int s = 0; s < this->numberOfSegments + 1; s++) {

		for (auto triangle = triangles.begin(); triangle != triangles.end(); triangle++) {
			for (auto vertex = triangle->begin(); vertex != triangle->end()-1; vertex++)
				this->addVertex(*vertex);

			for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
				if (hasElements(prism->cbegin(), prism->cend()-1, triangle->cbegin(), triangle->cend()-1)) {

					std::stable_sort(prism->begin(), prism->end()-1);
					std::stable_sort(triangle->begin(), triangle->end()-1);
					std::array<int, 4> difference = *triangle;
					std::set_difference(prism->begin(), prism->end()-1, triangle->begin(), triangle->end()-1, difference.begin());
					*triangle = difference;

					this->reordered->prismConnectivity.push_back(this->gridData->prismConnectivity[prism->back()]);
					this->prisms.erase(prism);
					break;
				}
				else
					prism++;
		}

			for (auto quadrangle = quadrangles.begin(); quadrangle != quadrangles.end(); quadrangle++) {
				for (auto vertex = quadrangle->cbegin(); vertex != quadrangle->cend()-1; vertex++)
					this->addVertex(*vertex);

				for (auto hexahedron = this->hexahedra.begin(); hexahedron != this->hexahedra.end();)
					if (hasElements(hexahedron->cbegin(), hexahedron->cend()-1, quadrangle->cbegin(), quadrangle->cend()-1)) {

						std::stable_sort(hexahedron->begin(), hexahedron->end()-1);
						std::stable_sort(quadrangle->begin(), quadrangle->end()-1);
						std::array<int, 5> difference = *quadrangle;
						std::set_difference(hexahedron->begin(), hexahedron->end()-1, quadrangle->begin(), quadrangle->end()-1, difference.begin());
						*quadrangle = difference;

						this->reordered->hexahedronConnectivity.push_back(this->gridData->hexahedronConnectivity[hexahedron->back()]);
						this->hexahedra.erase(hexahedron);
						break;
					}
					else
						hexahedron++;
			}

		}

		printf("\n\n");
		// print(triangles.cbegin(), triangles.cend(), "triangles " + std::to_string(triangles.size()));
		print(this->reordered->prismConnectivity.cbegin(), this->reordered->prismConnectivity.cend(), "PRISMS " + std::to_string(this->reordered->prismConnectivity.size()));
		// print(quadrangles.cbegin(), quadrangles.cend(), "quadrangles " + std::to_string(quadrangles.size()));
		print(this->reordered->hexahedronConnectivity.cbegin(), this->reordered->hexahedronConnectivity.cend(), "HEXAHEDRA " + std::to_string(this->reordered->hexahedronConnectivity.size()));

		printf("\n\tnumberOfPrisms               : %4i", int(this->prisms.size()));
		printf("\n\tnumberOfHexahedrons          : %4i", int(this->hexahedra.size()));
		printf("\n\tnumberOfVertices             : %4i", int(this->vertices.size()));

		printf("\n######################\n");
}

void RadialGridDataReordered::addVertex(int handle) {
	if (!hasElement(this->vertices.cbegin(), this->vertices.cend(), handle))
		this->vertices.push_back(handle);
}

void RadialGridDataReordered::copyPrism(std::vector<std::array<int, 7>>::iterator prism) {
	this->reordered->prismConnectivity.push_back(this->gridData->prismConnectivity[prism->back()]);
	prism = this->prisms.erase(prism);
}

void RadialGridDataReordered::copyHexahedron(std::vector<std::array<int, 9>>::iterator hexahedron) {
	this->reordered->hexahedronConnectivity.push_back(this->gridData->hexahedronConnectivity[hexahedron->back()]);
	hexahedron = this->hexahedra.erase(hexahedron);
}

void RadialGridDataReordered::copyVertices() {
	for (auto vertex : this->vertices)
		this->reordered->coordinates.emplace_back(this->gridData->coordinates[vertex]);
}

void RadialGridDataReordered::fixIndices() {
	std::unordered_map<int, int> originalToFinal;
	int index = 0;
	for (auto vertex : this->vertices)
		originalToFinal[vertex] = index++;

	for (auto& hexahedron : this->reordered->hexahedronConnectivity)
		for (auto vertex = hexahedron.begin(); vertex != hexahedron.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& prism : this->reordered->prismConnectivity)
		for (auto vertex = prism.begin(); vertex != prism.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& triangle : this->reordered->triangleConnectivity)
		for (auto vertex = triangle.begin(); vertex != triangle.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& quadrangle : this->reordered->quadrangleConnectivity)
		for (auto vertex = quadrangle.begin(); vertex != quadrangle.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& line : this->reordered->lineConnectivity)
		for (auto vertex = line.begin(); vertex != line.end() - 1; vertex++)
			*vertex = originalToFinal[*vertex];

	for (auto& vertex : this->reordered->wells[0].vertices)
		vertex = originalToFinal[vertex];

	for (auto& boundary : this->reordered->boundaries)
		for (auto& vertex : boundary.vertices)
			vertex = originalToFinal[vertex];

	for (int s = 0; s < this->numberOfSegments; s++) {

		for (int p = 0; p < this->numberOfPrismsPerSegment; p++)
			this->reordered->prismConnectivity[s * this->numberOfPrismsPerSegment + p].back() = this->elementShift++;

		for (int h = 0; h < this->numberOfHexahedronsPerSegment; h++)
			this->reordered->hexahedronConnectivity[s * this->numberOfHexahedronsPerSegment + h].back() = this->elementShift++;

	}

	for (int s = 0; s < this->numberOfSegments; s++)
		this->reordered->lineConnectivity[s].back() = this->elementShift++;
}