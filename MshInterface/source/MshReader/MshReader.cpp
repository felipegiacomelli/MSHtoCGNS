#include "MSHtoCGNS/MshInterface/MshReader.hpp"

namespace msh {
    int getMshGridDimension(std::string path) {
        if (!boost::filesystem::exists(boost::filesystem::path(path).parent_path()))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The parent path does not exist");

        if (!boost::filesystem::exists(path))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no file in the given path");

        char buffer[1024];
        std::ifstream file(std::ifstream(path.c_str()));

        file.seekg(0, std::ios::beg);
        while (strcmp(buffer, "$PhysicalNames") && !file.eof())
            file >> buffer;

        if (file.eof())
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no Physical Entities data in the grid file");

        int numberOfPhysicalEntities;
        file >> numberOfPhysicalEntities;
        std::vector<int> dimensions;
        for (int i = 0; i < numberOfPhysicalEntities; ++i) {
            int type;
            int index;
            std::string name;
            file >> type >> index >> name;
            dimensions.push_back(type);
        }

        int dimension = *std::max_element(dimensions.cbegin(), dimensions.cend());
        if (dimension == 2 || dimension == 3)
            return dimension;
        else
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Dimension must be either 2 or 3, not " + std::to_string(dimension));
    }
}
MshReader::MshReader(std::string filePath) : filePath(filePath) {
    this->checkFile();
    this->gridData = boost::make_shared<GridData>();
    this->readPhysicalNames();
    this->readNodes();
    this->readElements();
    this->determinePhysicalEntitiesRange();
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

    this->file.seekg(0, std::ios::beg);
    while (strcmp(buffer, "$MeshFormat") && !this->file.eof())
        this->file >> this->buffer;
    this->file >> this->version;

    if (this->version != 2.2)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - File version (" + std::to_string(this->version) + ") is older than 2.2");
}

void MshReader::readPhysicalNames() {
    this->file.seekg(0, std::ios::beg);
    while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof())
        this->file >> this->buffer;
    if (this->file.eof())
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no Physical Entities data in the grid file");

    this->file >> this->numberOfPhysicalEntities;
    for (int i = 0; i < this->numberOfPhysicalEntities; ++i) {
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
    for (int i = 0; i < numberOfVertices; ++i)
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
        for (int i = 0; i < numberOfElements+1; ++i) {
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

    if (connectivities[0][2] != 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Elements must have exactly 2 tags");

    // Before
    // index, type, number-of-tags, physical-entity-index, geometrical-entity-index, node-number-list

    for (unsigned i = 0; i < this->connectivities.size(); ++i) {
        this->connectivities[i].erase(this->connectivities[i].begin() + 4);
        this->connectivities[i].erase(this->connectivities[i].begin() + 2);
        this->connectivities[i].erase(this->connectivities[i].begin());
    }

    // After
    // type, physical-entity-index, node-number-list
}

void MshReader::determinePhysicalEntitiesRange() {
    std::stable_sort(this->connectivities.begin(), this->connectivities.end(), [=](const auto& a, const auto& b) {return a[this->sectionIndex] < b[this->sectionIndex];});

    std::vector<int> indices;
    for (const auto& connectivity : this->connectivities)
        indices.emplace_back(connectivity[this->sectionIndex]);

    for (int index : this->entitiesIndices) {
        auto range = std::equal_range(indices.cbegin(), indices.cend(), index);
        this->physicalEntitiesRange.emplace_back(std::array<int, 2>{static_cast<int>(std::distance(indices.cbegin(), range.first)), static_cast<int>(std::distance(indices.cbegin(), range.second))});
    }
}

void MshReader::addRegion(std::string name, int begin, int end) {
    this->gridData->regions.emplace_back(RegionData());
    this->gridData->regions.back().name = name;
    this->gridData->regions.back().begin = begin;
    this->gridData->regions.back().end = end;
}

void MshReader::addBoundary(std::string name, int begin, int end) {
    this->gridData->boundaries.emplace_back(BoundaryData());
    this->gridData->boundaries.back().name = name;
    this->gridData->boundaries.back().begin = begin;
    this->gridData->boundaries.back().end = end;
}
