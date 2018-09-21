#include <FileMend/GridDataExtractor.hpp>

GridDataExtractor::GridDataExtractor(GridDataShared original, std::string gridDataExtractorScript) : original(original), gridDataExtractorScript(gridDataExtractorScript) {
	this->checkGridData();
	this->readScript();
	this->buildElementConnectivities();
}

void GridDataExtractor::checkGridData() {
	if (this->original->dimension != 3)
		throw std::runtime_error("WellGenerator: original dimension must be 3 and not " + std::to_string(this->original->dimension));
}

void GridDataExtractor::readScript() {
	boost::property_tree::ptree iroot;
	boost::property_tree::read_json(this->gridDataExtractorScript, iroot);

	this->gridDataExtractorDatum.emplace_back();

	for (auto region : iroot.get_child("regions"))
		this->gridDataExtractorDatum.back().regions.emplace_back(region.second.get_value<std::string>());

	for (auto boundary : iroot.get_child("boundaries"))
		this->gridDataExtractorDatum.back().boundaries.emplace_back(boundary.second.get_value<std::string>());
}

void GridDataExtractor::buildElementConnectivities() {
	for (auto i = this->original->tetrahedronConnectivity.cbegin(); i != this->original->tetrahedronConnectivity.cend(); i++)
		this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

	for (auto i = this->original->hexahedronConnectivity.cbegin(); i != this->original->hexahedronConnectivity.cend(); i++)
		this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

	for (auto i = this->original->prismConnectivity.cbegin(); i != this->original->prismConnectivity.cend(); i++)
		this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

	for (auto i = this->original->pyramidConnectivity.cbegin(); i != this->original->pyramidConnectivity.cend(); i++)
		this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

	for (auto i = this->original->triangleConnectivity.cbegin(); i != this->original->triangleConnectivity.cend(); i++)
		this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

	for (auto i = this->original->quadrangleConnectivity.cbegin(); i != this->original->quadrangleConnectivity.cend(); i++)
		this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

	std::stable_sort(this->elementConnectivities.begin(), this->elementConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

	for (unsigned i = 0; i < this->elementConnectivities.size(); i++)
		this->elementConnectivities[i].pop_back();
}

void GridDataExtractor::extractRegions() {
	for (auto name : this->gridDataExtractorDatum.back().regions) {
		auto iterator(std::find_if(this->original->regions.cbegin(),this->original->regions.cend(), [=](auto r){return r.name == name;}));
		if (iterator == this->original->regions.cend())
			throw std::runtime_error("GridDataExtractor: There is no region " + name + " in gridData");
		auto region(*iterator);

		auto regionBegin = this->elementConnectivities.begin() + region.elementsOnRegion.front();
		auto regionEnd = this->elementConnectivities.begin() + region.elementsOnRegion.back() + 1;

		std::iota(region.elementsOnRegion.begin(), region.elementsOnRegion.end(), this->localIndex);

		for (auto element = regionBegin; element != regionEnd; element++) {

			for (auto vertex  : *element)
				this->vertices.insert(vertex);

			element->push_back(this->localIndex);

			switch (element->size()) {
				case 5: {
					std::array<int, 5> tetrahedron;
					std::copy_n(element->cbegin(), 5, std::begin(tetrahedron));
					this->extract->tetrahedronConnectivity.emplace_back(std::move(tetrahedron));
					break;
				}
				case 9: {
					std::array<int, 9> hexahedron;
					std::copy_n(element->cbegin(), 9, std::begin(hexahedron));
					this->extract->hexahedronConnectivity.emplace_back(std::move(hexahedron));
					break;
				}
				case 7: {
					std::array<int, 7> prism;
					std::copy_n(element->cbegin(), 7, std::begin(prism));
					this->extract->prismConnectivity.emplace_back(std::move(prism));
					break;
				}
				case 6: {
					std::array<int, 6> pyramid;
					std::copy_n(element->cbegin(), 6, std::begin(pyramid));
					this->extract->pyramidConnectivity.emplace_back(std::move(pyramid));
					break;
				}
			}

			this->localIndex++;

		}

		this->extract->regions.emplace_back(region);
	}
}

void GridDataExtractor::extractBoundaries() {

}
