#include <FileMend/SegmentGridExtractor.hpp>
#include <cgnslib.h>

SegmentGridExtractor::SegmentGridExtractor(boost::shared_ptr<GridData> gridData) : gridData(gridData) {
	this->checkGridData();
	this->createSegmentGrid();
	this->defineQuantities();
	this->copyVertices();
	this->copyElements();
	this->copyFacets();
	// this->reorder();
	// this->copyVertices();
	// this->fixBoundaries();
	// this->fixRegion();
	// this->fixWell();
}

void SegmentGridExtractor::checkGridData() {
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

void SegmentGridExtractor::defineQuantities() {
	this->numberOfSegments = this->gridData->lineConnectivity.size();
	this->numberOfPrismsPerSegment = this->gridData->prismConnectivity.size() / this->numberOfSegments;
	this->numberOfHexahedronsPerSegment = this->gridData->hexahedronConnectivity.size() / this->numberOfSegments;
	this->numberOfHexahedronsPerRadius = this->numberOfHexahedronsPerSegment / this->numberOfPrismsPerSegment;
	this->numberOfVerticesPerSection = this->gridData->coordinates.size() / (this->gridData->lineConnectivity.size()+1);
}

void SegmentGridExtractor::createSegmentGrid() {
	this->segmentGrid = boost::make_shared<GridData>();
	this->segmentGrid->dimension = this->gridData->dimension;

	this->segmentGrid->boundaries = this->gridData->boundaries;

	this->segmentGrid->regions = this->gridData->regions;

	this->segmentGrid->wells = this->gridData->wells;
}

void SegmentGridExtractor::copyVertices() {
	for (int vertex = 0; vertex < 2 * this->numberOfVerticesPerSection; vertex++)
		this->segmentGrid->coordinates.emplace_back(this->gridData->coordinates[vertex]);
}

void SegmentGridExtractor::copyElements() {
	for (int prism = 0; prism < this->numberOfPrismsPerSegment; prism++)
		this->segmentGrid->prismConnectivity.emplace_back(this->gridData->prismConnectivity[prism]);

	for (int hexahedron = 0; hexahedron < this->numberOfHexahedronsPerSegment; hexahedron++)
		this->segmentGrid->hexahedronConnectivity.emplace_back(this->gridData->hexahedronConnectivity[hexahedron]);
}

void SegmentGridExtractor::copyFacets() {
	this->segmentGrid->triangleConnectivity = this->gridData->triangleConnectivity;

	for (int quadrangle = 0; quadrangle < this->numberOfPrismsPerSegment; quadrangle++)
		this->segmentGrid->quadrangleConnectivity.emplace_back(this->gridData->quadrangleConnectivity[quadrangle]);

	for (int quadrangle = this->numberOfPrismsPerSegment * this->numberOfSegments; quadrangle < int(this->gridData->quadrangleConnectivity.size()); quadrangle++)
		this->segmentGrid->quadrangleConnectivity.emplace_back(this->gridData->quadrangleConnectivity[quadrangle]);
}

// void SegmentGridExtractor::reorder() {
// 	this->buildFirstSection();
// 	for (int segment = 0; segment < this->numberOfSegments; segment++) {
// 		this->vertexShift = 0;
// 		for (auto triangle = this->triangles.begin(); triangle != this->triangles.end(); triangle++)
// 			for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
// 				if (hasElements(prism->cbegin(), prism->cend()-1, triangle->cbegin(), triangle->cend()-1)) {
// 					this->addVertex((*prism)[3], segment + 1);
// 					this->addVertex((*prism)[4], segment + 1);
// 					this->addVertex((*prism)[5], segment + 1);
// 					this->updateTriangle(prism, triangle);
// 					this->copyPrism(prism);
// 					break;
// 				}
// 				else
// 					prism++;

// 		for (auto quadrangle = this->quadrangles.begin(); quadrangle != this->quadrangles.end(); quadrangle++)
// 			for (auto hexahedron = this->hexahedra.begin(); hexahedron != this->hexahedra.end();)
// 				if (hasElements(hexahedron->cbegin(), hexahedron->cend()-1, quadrangle->cbegin(), quadrangle->cend()-1)) {
// 					this->addVertex((*hexahedron)[3], segment + 1);
// 					this->addVertex((*hexahedron)[2], segment + 1);
// 					this->addVertex((*hexahedron)[6], segment + 1);
// 					this->addVertex((*hexahedron)[7], segment + 1);
// 					this->updateQuadrangle(hexahedron, quadrangle);
// 					this->copyHexahedron(hexahedron);
// 					break;
// 				}
// 				else
// 					hexahedron++;
// 	}
// }

// void SegmentGridExtractor::buildFirstSection() {
// 	for (auto triangle = this->triangles.begin(); triangle != this->triangles.end(); triangle++)
// 		for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
// 			if (hasElements(prism->cbegin(), prism->cend()-1, triangle->cbegin(), triangle->cend()-1)) {
// 				this->addVertex((*prism)[0], 0);
// 				this->addVertex((*prism)[1], 0);
// 				this->addVertex((*prism)[2], 0);
// 				break;
// 			}
// 			else
// 				prism++;

// 	for (auto quadrangle = this->quadrangles.begin(); quadrangle != this->quadrangles.end(); quadrangle++)
// 		for (auto hexahedron = this->hexahedra.begin(); hexahedron != this->hexahedra.end();)
// 			if (hasElements(hexahedron->cbegin(), hexahedron->cend()-1, quadrangle->cbegin(), quadrangle->cend()-1)) {
// 				this->addVertex((*hexahedron)[0], 0);
// 				this->addVertex((*hexahedron)[1], 0);
// 				this->addVertex((*hexahedron)[5], 0);
// 				this->addVertex((*hexahedron)[4], 0);
// 				break;
// 			}
// 			else
// 				hexahedron++;
// }

// void SegmentGridExtractor::addVertex(int vertex, int section) {
// 	if (std::find_if(this->vertices.cbegin(), this->vertices.cend(), [=](auto vh){return vh.first == vertex;}) == this->vertices.cend())
// 		this->vertices.push_back(std::make_pair(vertex, section * this->numberOfVerticesPerSection + this->vertexShift++));
// }

// void SegmentGridExtractor::updateTriangle(std::vector<std::array<int, 7>>::iterator prism, std::vector<std::array<int, 4>>::iterator triangle) {
// 	(*triangle)[0] = (*prism)[3];
// 	(*triangle)[1] = (*prism)[4];
// 	(*triangle)[2] = (*prism)[5];
// }

// void SegmentGridExtractor::updateQuadrangle(std::vector<std::array<int, 9>>::iterator hexahedron, std::vector<std::array<int, 5>>::iterator quadrangle) {
// 	(*quadrangle)[0] = (*hexahedron)[2];
// 	(*quadrangle)[1] = (*hexahedron)[3];
// 	(*quadrangle)[2] = (*hexahedron)[7];
// 	(*quadrangle)[3] = (*hexahedron)[6];
// }

// void SegmentGridExtractor::copyPrism(std::vector<std::array<int, 7>>::iterator prism) {
// 	this->segmentGrid->prismConnectivity.push_back(this->gridData->prismConnectivity[prism->back()]);
// 	prism = this->prisms.erase(prism);
// }

// void SegmentGridExtractor::copyHexahedron(std::vector<std::array<int, 9>>::iterator hexahedron) {
// 	this->segmentGrid->hexahedronConnectivity.push_back(this->gridData->hexahedronConnectivity[hexahedron->back()]);
// 	hexahedron = this->hexahedra.erase(hexahedron);
// }

// void SegmentGridExtractor::fixBoundaries() {
// 	for (int s = 0; s < this->numberOfSegments; s++) {
// 		for (int p = 0; p < this->numberOfPrismsPerSegment; p++)
// 			this->segmentGrid->prismConnectivity[s * this->numberOfPrismsPerSegment + p].back() = this->elementShift++;
// 		for (int h = 0; h < this->numberOfHexahedronsPerSegment; h++)
// 			this->segmentGrid->hexahedronConnectivity[s * this->numberOfHexahedronsPerSegment + h].back() = this->elementShift++;
// 	}
// }

// void SegmentGridExtractor::fixRegion() {
// 	this->quadrangles.clear();
// 	this->triangles.clear();
// 	this->facetShift = this->elementShift;

// 	// cylinder surface
// 	auto boundary = this->segmentGrid->boundaries.end();
// 	for (auto b = this->segmentGrid->boundaries.begin(); b != this->segmentGrid->boundaries.end(); b++)
// 		if (!hasElement(b->vertices.cbegin(), b->vertices.cend(), this->segmentGrid->lineConnectivity.front()[0]) && !hasElement(b->vertices.cbegin(), b->vertices.cend(), this->segmentGrid->lineConnectivity.back()[1]))
// 			boundary = b;

// 	for (auto i = this->segmentGrid->quadrangleConnectivity.cbegin(); i != this->segmentGrid->quadrangleConnectivity.cend(); i++)
// 		if (i->back() >= boundary->facetBegin && i->back() < boundary->facetEnd)
// 			this->quadrangles.emplace_back(*i);

// 	boundary->facetBegin = this->facetShift;
// 	for (int segment = 0; segment < this->numberOfSegments; segment++) {
// 		for (auto i = this->quadrangles.begin(); i != this->quadrangles.end(); i++)
// 			if (*std::min_element(i->cbegin(), i->cend()-1) >=  segment * this->numberOfVerticesPerSection && *std::max_element(i->cbegin(), i->cend()-1) < (segment+2) * this->numberOfVerticesPerSection)
// 				i->back() = this->facetShift++;
// 	}
// 	boundary->facetEnd = this->facetShift;

// 	std::stable_sort(this->quadrangles.begin(), this->quadrangles.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

// 	// first lid
// 	for (auto b = this->segmentGrid->boundaries.begin(); b != this->segmentGrid->boundaries.end(); b++)
// 		if (hasElement(b->vertices.cbegin(), b->vertices.cend(), this->segmentGrid->lineConnectivity.front()[0]))
// 			boundary = b;

// 	boundary->facetBegin = this->facetShift;
// 	for (auto i = this->segmentGrid->triangleConnectivity.cbegin(); i != this->segmentGrid->triangleConnectivity.cend(); i++)
// 		if (i->back() >= boundary->facetBegin && i->back() < boundary->facetEnd) {
// 			this->triangles.emplace_back(*i);
// 			this->triangles.back().back() = this->facetShift++;
// 		}

// 	for (auto i = this->segmentGrid->quadrangleConnectivity.cbegin(); i != this->segmentGrid->quadrangleConnectivity.cend(); i++)
// 		if (i->back() >= boundary->facetBegin && i->back() < boundary->facetEnd) {
// 			this->quadrangles.emplace_back(*i);
// 			this->quadrangles.back().back() = this->facetShift++;
// 		}
// 	boundary->facetEnd = this->facetShift;

// 	// last lid
// 	for (auto b = this->segmentGrid->boundaries.begin(); b != this->segmentGrid->boundaries.end(); b++)
// 		if (hasElement(b->vertices.cbegin(), b->vertices.cend(), this->segmentGrid->lineConnectivity.back()[1]))
// 			boundary = b;

// 	boundary->facetBegin = this->facetShift;
// 	for (auto i = this->segmentGrid->triangleConnectivity.cbegin(); i != this->segmentGrid->triangleConnectivity.cend(); i++)
// 		if (i->back() >= boundary->facetBegin && i->back() < boundary->facetEnd) {
// 			this->triangles.emplace_back(*i);
// 			this->triangles.back().back() = this->facetShift++;
// 		}

// 	for (auto i = this->segmentGrid->quadrangleConnectivity.cbegin(); i != this->segmentGrid->quadrangleConnectivity.cend(); i++)
// 		if (i->back() >= boundary->facetBegin && i->back() < boundary->facetEnd) {
// 			this->quadrangles.emplace_back(*i);
// 			this->quadrangles.back().back() = this->facetShift++;
// 		}
// 	boundary->facetEnd = this->facetShift;

// 	this->segmentGrid->quadrangleConnectivity = std::move(this->quadrangles);
// 	this->segmentGrid->triangleConnectivity = std::move(this->triangles);
// }

// void SegmentGridExtractor::fixWell() {
// 	this->lineShift = this->facetShift;
// 	this->segmentGrid->wells[0].lineBegin = this->lineShift;
// 	for (int s = 0; s < this->numberOfSegments; s++)
// 		this->segmentGrid->lineConnectivity[s].back() = this->lineShift++;
// 	this->segmentGrid->wells[0].lineEnd = this->lineShift;
// }