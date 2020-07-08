#include "MSHtoCGNS/Manipulation/GridDataExtractor.hpp"

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
    this->createExtract();
    this->extractEntities();
    this->extractVertices();
    this->rectifyConnectivities();
}

void GridDataExtractor::checkGridData() {
    if (this->original->dimension != 3)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - original dimension must be 3 and not " + std::to_string(this->original->dimension));
}

void GridDataExtractor::readScript() {
    this->gridDataExtractorDatum.emplace_back();

    if (this->propertyTree.get_child_optional("regions"))
        for (auto region : this->propertyTree.get_child("regions"))
            this->gridDataExtractorDatum.back().sections.emplace_back(region.second.get_value<std::string>());

    if (this->propertyTree.get_child_optional("boundaries"))
        for (auto boundary : this->propertyTree.get_child("boundaries"))
            this->gridDataExtractorDatum.back().sections.emplace_back(boundary.second.get_value<std::string>());

    if (this->propertyTree.get_child_optional("wells"))
        for (auto wells : this->propertyTree.get_child("wells"))
            this->gridDataExtractorDatum.back().sections.emplace_back(wells.second.get_value<std::string>());
}

void GridDataExtractor::createExtract() {
    this->extract = boost::make_shared<GridData>();
    this->extract->dimension = this->original->dimension;
}

void GridDataExtractor::extractEntities() {
    for (auto name : this->gridDataExtractorDatum.back().sections) {
        auto iterator(std::find_if(this->original->sections.cbegin(),this->original->sections.cend(), [=](auto section){return section.name == name;}));
        if (iterator == this->original->sections.cend())
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no section " + name + " in gridData");
        auto section(*iterator);

        auto begin = this->original->connectivities.cbegin() + section.begin;
        auto end = this->original->connectivities.cbegin() + section.end;

        section.begin = this->localIndex;
        for (auto element = begin; element != end; ++element) {

            for (auto vertex = element->cbegin() + 1; vertex != element->cend() - 1; ++vertex)
                this->vertices.insert(*vertex);

            this->extract->connectivities.emplace_back(*element);
            this->extract->connectivities.back().back() = this->localIndex++;
        }
        section.end = this->localIndex;

        this->extract->sections.emplace_back(section);
    }
}

void GridDataExtractor::extractVertices() {
    for (auto vertex : this->vertices)
        this->extract->coordinates.emplace_back(this->original->coordinates[vertex]);
}

void GridDataExtractor::rectifyConnectivities() {
    boost::unordered_map<int, int> originalToExtract;
    int index = 0;
    for (auto vertex : vertices)
        originalToExtract[vertex] = index++;

    for (auto& connectivity : this->extract->connectivities)
        for (auto index = connectivity.begin() + 1; index != connectivity.end() - 1; ++index)
            *index = originalToExtract[*index];

    for (auto& section : this->extract->sections)
        for (auto& vertex : section.vertices)
            vertex = originalToExtract[vertex];
}
