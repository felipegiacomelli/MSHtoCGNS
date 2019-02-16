#include "MSHtoCGNS/FileMend/GridDataExtractor.hpp"

GridDataExtractor::GridDataExtractor(boost::shared_ptr<GridData> original, std::string gridDataExtractorScript) : original(original) {
    boost::property_tree::read_json(gridDataExtractorScript, this->propertyTree);
    this->initialize();
}

GridDataExtractor::GridDataExtractor(boost::shared_ptr<GridData> original, boost::property_tree::ptree propertyTree) : original(original), propertyTree(propertyTree) {
    this->initialize();
}

void GridDataExtractor::initialize() {
    this->checkGridData();
    this->readScript();
    this->buildElementConnectivities();
    this->createExtract();
    this->extractRegions();
    this->extractBoundaries();
    this->extractWells();
    this->extractVertices();
    this->fixIndices();
}

void GridDataExtractor::checkGridData() {
    if (this->original->dimension != 3)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - original dimension must be 3 and not " + std::to_string(this->original->dimension));
}

void GridDataExtractor::readScript() {
    this->gridDataExtractorDatum.emplace_back();

    if (this->propertyTree.get_child_optional("regions"))
        for (auto region : this->propertyTree.get_child("regions"))
            this->gridDataExtractorDatum.back().regions.emplace_back(region.second.get_value<std::string>());

    if (this->propertyTree.get_child_optional("boundaries"))
        for (auto boundary : this->propertyTree.get_child("boundaries"))
            this->gridDataExtractorDatum.back().boundaries.emplace_back(boundary.second.get_value<std::string>());

    if (this->propertyTree.get_child_optional("wells"))
        for (auto wells : this->propertyTree.get_child("wells"))
            this->gridDataExtractorDatum.back().wells.emplace_back(wells.second.get_value<std::string>());
}

void GridDataExtractor::buildElementConnectivities() {
    for (auto i = this->original->tetrahedrons.cbegin(); i != this->original->tetrahedrons.cend(); ++i)
        this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

    for (auto i = this->original->hexahedrons.cbegin(); i != this->original->hexahedrons.cend(); ++i)
        this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

    for (auto i = this->original->prisms.cbegin(); i != this->original->prisms.cend(); ++i)
        this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

    for (auto i = this->original->pyramids.cbegin(); i != this->original->pyramids.cend(); ++i)
        this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

    for (auto i = this->original->triangles.cbegin(); i != this->original->triangles.cend(); ++i)
        this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

    for (auto i = this->original->quadrangles.cbegin(); i != this->original->quadrangles.cend(); ++i)
        this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

    for (auto i = this->original->lines.cbegin(); i != this->original->lines.cend(); ++i)
        this->elementConnectivities.emplace_back(i->cbegin(), i->cend());

    std::stable_sort(this->elementConnectivities.begin(), this->elementConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

    for (unsigned i = 0; i < this->elementConnectivities.size(); ++i)
        this->elementConnectivities[i].pop_back();
}

void GridDataExtractor::createExtract() {
    this->extract = boost::make_shared<GridData>();
    this->extract->dimension = 3;
}

void GridDataExtractor::extractRegions() {
    for (auto name : this->gridDataExtractorDatum.back().regions) {

        auto iterator(std::find_if(this->original->regions.cbegin(),this->original->regions.cend(), [=](auto r){return r.name == name;}));
        if (iterator == this->original->regions.cend())
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no region " + name + " in gridData");
        auto region(*iterator);

        auto regionBegin = this->elementConnectivities.begin() + region.begin;
        auto regionEnd = this->elementConnectivities.begin() + region.end;

        for (auto element = regionBegin; element != regionEnd; ++element) {

            for (auto vertex : *element)
                this->vertices.insert(vertex);

            element->push_back(this->localIndex++);

            switch (element->size()) {
                case 5: {
                    std::array<int, 5> tetrahedron;
                    std::copy_n(element->cbegin(), 5, std::begin(tetrahedron));
                    this->extract->tetrahedrons.emplace_back(std::move(tetrahedron));
                    break;
                }
                case 9: {
                    std::array<int, 9> hexahedron;
                    std::copy_n(element->cbegin(), 9, std::begin(hexahedron));
                    this->extract->hexahedrons.emplace_back(std::move(hexahedron));
                    break;
                }
                case 7: {
                    std::array<int, 7> prism;
                    std::copy_n(element->cbegin(), 7, std::begin(prism));
                    this->extract->prisms.emplace_back(std::move(prism));
                    break;
                }
                case 6: {
                    std::array<int, 6> pyramid;
                    std::copy_n(element->cbegin(), 6, std::begin(pyramid));
                    this->extract->pyramids.emplace_back(std::move(pyramid));
                    break;
                }
            }

        }

        region.begin = regionBegin->back();
        region.end = (--regionEnd)->back() + 1;

        this->extract->regions.emplace_back(region);
    }
}

void GridDataExtractor::extractBoundaries() {
    for (auto name : this->gridDataExtractorDatum.back().boundaries) {

        auto iterator(std::find_if(this->original->boundaries.cbegin(),this->original->boundaries.cend(), [=](auto b){return b.name == name;}));
        if (iterator == this->original->boundaries.cend())
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no boundary " + name + " in gridData");
        auto boundary(*iterator);

        //
        std::vector<int> deleteIndices;
        for (auto i = this->original->triangles.cbegin(); i != this->original->triangles.cend(); ++i)
            if (i->back() >= boundary.begin && i->back() <= boundary.end)
                deleteIndices.emplace_back(i - this->original->triangles.cbegin());

        for (auto rit = deleteIndices.crbegin(); rit != deleteIndices.crend(); ++rit)
            this->original->triangles.erase(this->original->triangles.begin() + *rit);

        deleteIndices.clear();
        for (auto i = this->original->quadrangles.cbegin(); i != this->original->quadrangles.cend(); ++i)
            if (i->back() >= boundary.begin && i->back() <= boundary.end)
                deleteIndices.emplace_back(i - this->original->quadrangles.cbegin());

        for (auto rit = deleteIndices.crbegin(); rit != deleteIndices.crend(); ++rit)
            this->original->quadrangles.erase(this->original->quadrangles.begin() + *rit);

        this->original->boundaries.erase(iterator);
        //

        auto boundaryBegin = this->elementConnectivities.begin() + boundary.begin;
        auto boundaryEnd = this->elementConnectivities.begin() + boundary.end;

        for (auto facet = boundaryBegin; facet != boundaryEnd; ++facet) {

            facet->push_back(this->localIndex++);

            switch (facet->size()) {
                case 4: {
                    std::array<int, 4> triangle;
                    std::copy_n(facet->cbegin(), 4, std::begin(triangle));
                    this->extract->triangles.emplace_back(std::move(triangle));
                    break;
                }
                case 5: {
                    std::array<int, 5> quadrangle;
                    std::copy_n(facet->cbegin(), 5, std::begin(quadrangle));
                    this->extract->quadrangles.emplace_back(std::move(quadrangle));
                    break;
                }
            }

        }

        boundary.begin = boundaryBegin->back();
        boundary.end = (--boundaryEnd)->back() + 1;

        this->extract->boundaries.emplace_back(boundary);
    }
}

void GridDataExtractor::extractWells() {
    for (auto name : this->gridDataExtractorDatum.back().wells) {

        auto iterator(std::find_if(this->original->wells.cbegin(),this->original->wells.cend(), [=](auto w){return w.name == name;}));
        if (iterator == this->original->wells.cend())
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no well " + name + " in gridData");
        auto well(*iterator);

        auto wellBegin = this->elementConnectivities.begin() + well.begin;
        auto wellEnd = this->elementConnectivities.begin() + well.end;

        for (auto element = wellBegin; element != wellEnd; ++element) {

            element->push_back(this->localIndex++);

            std::array<int, 3> line;
            std::copy_n(element->cbegin(), 3, std::begin(line));
            this->extract->lines.emplace_back(std::move(line));
        }

        well.begin = wellBegin->back();
        well.end = (--wellEnd)->back() + 1;

        this->extract->wells.emplace_back(well);
    }
}

void GridDataExtractor::extractVertices() {
    for (auto vertex : this->vertices)
        this->extract->coordinates.push_back(this->original->coordinates[vertex]);
}

void GridDataExtractor::fixIndices() {
    std::unordered_map<int, int> originalToExtract;
    int index = 0;
    for (auto vertex : vertices)
        originalToExtract[vertex] = index++;

    for (auto& tetrahedron : this->extract->tetrahedrons)
        for (auto vertex = tetrahedron.begin(); vertex != tetrahedron.end() - 1; ++vertex)
            *vertex = originalToExtract[*vertex];

    for (auto& hexahedron : this->extract->hexahedrons)
        for (auto vertex = hexahedron.begin(); vertex != hexahedron.end() - 1; ++vertex)
            *vertex = originalToExtract[*vertex];

    for (auto& prism : this->extract->prisms)
        for (auto vertex = prism.begin(); vertex != prism.end() - 1; ++vertex)
            *vertex = originalToExtract[*vertex];

    for (auto& pyramid : this->extract->pyramids)
        for (auto vertex = pyramid.begin(); vertex != pyramid.end() - 1; ++vertex)
            *vertex = originalToExtract[*vertex];

    for (auto& triangle : this->extract->triangles)
        for (auto vertex = triangle.begin(); vertex != triangle.end() - 1; ++vertex)
            *vertex = originalToExtract[*vertex];

    for (auto& quadrangle : this->extract->quadrangles)
        for (auto vertex = quadrangle.begin(); vertex != quadrangle.end() - 1; ++vertex)
            *vertex = originalToExtract[*vertex];

    for (auto& line : this->extract->lines)
        for (auto vertex = line.begin(); vertex != line.end() - 1; ++vertex)
            *vertex = originalToExtract[*vertex];

    for (auto& well : this->extract->wells)
        for (auto& vertex : well.vertices)
            vertex = originalToExtract[vertex];

    for (auto& boundary : this->extract->boundaries)
        for (auto& vertex : boundary.vertices)
            vertex = originalToExtract[vertex];
}
