#include "MSHtoCGNS/MshInterface/MshReader.hpp"
#include "MSHtoCGNS/Utilities/Print.hpp"

MshReader::MshReader(std::string filePath) : filePath(filePath) {
    this->checkFile();
    this->gridData = boost::make_shared<GridData>();
}

void MshReader::checkFile() {
    boost::filesystem::path input(this->filePath);
    if (!boost::filesystem::exists(input.parent_path()))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The parent path does not exist");

    if (!boost::filesystem::exists(this->filePath))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no file in the given path");

    if (input.extension() != ".msh")
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The file extension is not .msh");

    this->file = std::ifstream(this->filePath.c_str());
}

void MshReader::readPhysicalNames() {
    this->file.seekg(0, std::ios::beg);
    while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof())
        this->file >> this->buffer;
    if (this->file.eof())
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no Physical Entities data in the grid file");

    this->file >> this->numberOfPhysicalEntities;
    for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
        int type, index;
        std::string name;
        this->file >> type >> index >> name;
        this->entitiesTypes.push_back(type - 1);
        this->entitiesIndices.push_back(index - 1);
        this->entitiesNames.emplace_back(std::string());
        std::transform(name.begin() + 1, name.end() - 1, std::back_inserter(this->entitiesNames.back()), ::toupper);
    }
}

void MshReader::readNodes() {
    int numberOfVertices, temporary;
    this->file.seekg(0, std::ios::beg);
    while (strcmp(this->buffer, "$Nodes") && !this->file.eof())
        this->file >> this->buffer;
    if (this->file.eof())
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no Node data in the grid file");

    this->file >> numberOfVertices;
    this->gridData->coordinates.resize(numberOfVertices, std::array<double, 3>());
    for (int i = 0; i < numberOfVertices; i++)
        this->file >> temporary >> this->gridData->coordinates[i][0] >> this->gridData->coordinates[i][1] >> this->gridData->coordinates[i][2];
}

void MshReader::readElements() {
    int numberOfElements;
    this->file.seekg(0, std::ios::beg);
    while (strcmp(this->buffer, "$Elements") && !this->file.eof())
        this->file >> this->buffer;
    if (this->file.eof())
        this->file.clear();
    else {
        this->file >> numberOfElements;
        for (int i = 0; i < numberOfElements+1; i++) {
            std::string line;
            std::getline(this->file, line);
            std::istringstream stream(line);
            std::vector<int> connectivity;
            int value;
            while (stream >> value)
                connectivity.push_back(value - 1);
            this->connectivities.emplace_back(std::move(connectivity));
        }
    }
    this->connectivities.erase(this->connectivities.begin());

    // $PhysicalNames
    // dimension, index, "name"

    // $Elements
    // index, type, number-of-tags, physical-entity-index, geometrical-entity-index, node-number-list

    for (auto& connectivity : this->connectivities)
        for (auto& index : connectivity)
            ++index;

    // index, type, number-of-tags, physical-entity-index, geometrical-entity-index, node-number-list
    // print2D(this->connectivities.cbegin(), this->connectivities.cend(), "\n\tconnectivities\n\n");

    // if (connectivities[0][2] != 1)
        // throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Elements must have exactly 2 tags");

    for (unsigned i = 0; i < this->connectivities.size(); i++) {
        this->connectivities[i].erase(this->connectivities[i].begin() + 4);
        this->connectivities[i].erase(this->connectivities[i].begin() + 2);
    }

    // index, type, physical-entity-index, node-number-list
    std::stable_sort(this->connectivities.begin(), this->connectivities.end(), [=](const auto& a, const auto& b) {return a[this->sectionIndex] < b[this->sectionIndex];});
    print2D(this->connectivities.cbegin(), this->connectivities.cend(), "\n\tconnectivities\n\n");

    std::vector<int> range;
    for (const auto& connectivity : connectivities)
        range.emplace_back(connectivity[2]);

    for (auto index : this->entitiesIndices) {
        auto its = std::equal_range(range.cbegin(), range.cend(), index+1);
        printf("\n\t%2i: %3li - %3li\n", index+1, std::distance(range.cbegin(), its.first), std::distance(range.cbegin(), its.second));
    }

    for (auto& connectivity : this->connectivities)
        for (auto& index : connectivity)
            --index;
}

void MshReader::divideConnectivities() {
    this->elements = std::vector<std::vector<int>>(this->connectivities.begin() + this->numberOfFacets, this->connectivities.end());
    std::stable_sort(this->elements.begin(), this->elements.end(), [=](const auto& a, const auto& b){return a[this->sectionIndex] < b[this->sectionIndex];});
    for (unsigned i = 0; i < this->elements.size(); i++)
        this->elements[i].push_back(i);
    int numberOfElements = elements.size();

    this->facets = std::vector<std::vector<int>>(this->connectivities.begin(), this->connectivities.begin() + this->numberOfFacets);
    std::stable_sort(this->facets.begin(), this->facets.end(), [=](const auto& a, const auto& b){return a[this->sectionIndex] < b[this->sectionIndex];});
    for (unsigned i = 0; i < this->facets.size(); i++)
        this->facets[i].push_back(numberOfElements + i);
}

void MshReader::assignElementsToRegions() {
    int counter = 0;
    std::vector<unsigned> regionStart;
    regionStart.emplace_back(0);
    for (unsigned i = 0; i < this->elements.size()-1; i++) {
        if (this->elements[i][sectionIndex] == this->elements[i+1][sectionIndex])
            counter++;
        else {
            counter++;
            regionStart.push_back(counter);
        }
    }
    regionStart.push_back(this->elements.size());

    for (unsigned i = 0; i < regionStart.size()-1; i++)
        for (unsigned j = regionStart[i]; j < regionStart[i+1]; j++)
            this->elements[j][sectionIndex] = i;

    this->regionElements.resize(this->numberOfRegions, std::vector<int>());
    for (unsigned i = 0; i < this->elements.size(); i++)
        this->regionElements[this->elements[i][sectionIndex]].push_back(i);
}

void MshReader::assignFacetsToBoundaries() {
    this->boundaryFacets.resize(this->numberOfBoundaries, std::vector<int>());
    for (unsigned i = 0; i < this->facets.size(); i++)
        this->boundaryFacets[this->facets[i][sectionIndex]].push_back(i);
}
