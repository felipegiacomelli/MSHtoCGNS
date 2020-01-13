#include "MSHtoCGNS/MshInterface/MshReader.hpp"
#include <cgnslib.h>

std::unordered_map<int, int> mshToCgns{{1, BAR_2}, {2, TRI_3}, {3, QUAD_4}, {4, TETRA_4}, {5, HEXA_8}, {6, PENTA_6}, {7, PYRA_5}};

MshReader::MshReader(std::string filePath) : filePath(filePath) {
    this->checkFile();
    this->gridData = boost::make_shared<GridData>();
    this->readPhysicalNames();
    this->readNodes();
    this->readElements();
    this->determinePhysicalEntitiesRange();
    this->addSections();
    this->addElements(this->gridData->sections);
    this->findVertices(this->gridData->sections);
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
        this->sectionsDimension.push_back(type);
        this->sectionsIndices.push_back(index - 1);
        this->sectionsNames.emplace_back();
        std::transform(name.begin() + 1, name.end() - 1, std::back_inserter(this->sectionsNames.back()), ::toupper);
    }

    this->gridData->dimension = *std::max_element(this->sectionsDimension.cbegin(), this->sectionsDimension.cend());
}

void MshReader::readNodes() {
    int numberOfVertices, temporary;
    this->file.seekg(0, std::ios::beg);
    while (strcmp(this->buffer, "$Nodes") && !this->file.eof())
        this->file >> this->buffer;
    if (this->file.eof())
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no Node data in the grid file");

    this->file >> numberOfVertices;
    this->gridData->numberOfLocalVertices = numberOfVertices;
    this->gridData->coordinates.resize(numberOfVertices, std::array<double, 3>());
    for (int i = 0; i < numberOfVertices; ++i)
        this->file >> temporary >> this->gridData->coordinates[i][0] >> this->gridData->coordinates[i][1] >> this->gridData->coordinates[i][2];
}

void MshReader::readElements() {
    this->file.seekg(0, std::ios::beg);
    while (strcmp(this->buffer, "$Elements") && !this->file.eof())
        this->file >> this->buffer;
    if (this->file.eof()) {
        this->file.clear();
    }
    else {
        int numberOfElements;
        this->file >> numberOfElements;
        this->connectivities.reserve(numberOfElements);
        for (int i = 0; i < numberOfElements+1; ++i) {
            std::string line;
            std::getline(this->file, line);
            std::istringstream stream(line);
            this->connectivities.emplace_back();
            int value;
            while (stream >> value)
                this->connectivities.back().push_back(value - 1);
        }
    }
    this->connectivities.erase(this->connectivities.begin());

    if (connectivities[0][2] != 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Elements must have exactly 2 tags");

    // Before
    // index, type, number-of-tags, physical-section-index, geometrical-section-index, node-number-list

    for (unsigned i = 0; i < this->connectivities.size(); ++i) {
        this->connectivities[i].erase(this->connectivities[i].begin() + 4);
        this->connectivities[i].erase(this->connectivities[i].begin() + 2);
        this->connectivities[i].erase(this->connectivities[i].begin());
        this->connectivities[i][0] = mshToCgns[this->connectivities[i][0] + 1];
        std::iter_swap(this->connectivities[i].begin(), this->connectivities[i].begin() + 1);
    }

    // After
    // physical-section-index, type, node-number-list
}

void MshReader::determinePhysicalEntitiesRange() {
    std::stable_sort(this->connectivities.begin(), this->connectivities.end(), [=](const auto& a, const auto& b){return a[this->sectionIndex] < b[this->sectionIndex];});

    std::vector<int> indices;
    for (const auto& connectivity : this->connectivities)
        indices.emplace_back(connectivity[this->sectionIndex]);

    for (int index : this->sectionsIndices) {
        auto range = std::equal_range(indices.cbegin(), indices.cend(), index);
        this->physicalEntitiesRange.emplace_back(std::array<long int, 2>{std::distance(indices.cbegin(), range.first), std::distance(indices.cbegin(), range.second)});
    }
}

void MshReader::addSections() {
    for (int physicalEntity = 0; physicalEntity < this->numberOfPhysicalEntities; ++physicalEntity) {
        this->gridData->sections.emplace_back();
        this->gridData->sections.back().name = this->sectionsNames[physicalEntity];
        this->gridData->sections.back().dimension = this->sectionsDimension[physicalEntity];
        this->gridData->sections.back().begin = this->physicalEntitiesRange[physicalEntity].front();
        this->gridData->sections.back().end = this->physicalEntitiesRange[physicalEntity].back();
    }
    std::stable_sort(this->gridData->sections.begin(), this->gridData->sections.end(), [=](const auto& a, const auto& b){return a.dimension > b.dimension;});
}

void MshReader::addElements(std::vector<SectionData>& sections) {
    for (auto& sections : sections) {
        auto position = this->connectivities.begin() + sections.begin;
        auto end = this->connectivities.begin() + sections.end;
        sections.begin = this->shift;
        while (position != end) {
            position->push_back(this->shift++);
            this->gridData->connectivities.emplace_back(position->begin() + 1, position->end());
            ++position;
        }
        sections.end = this->shift;
    }
}

void MshReader::findVertices(std::vector<SectionData>& sections) {
    for (auto& section : sections) {
        std::set<int> vertices;
        for (auto connectivity = this->gridData->connectivities.cbegin() + section.begin; connectivity != this->gridData->connectivities.cbegin() + section.end; ++connectivity) {
            vertices.insert(connectivity->cbegin() + 1, connectivity->cend() - 1);
        }
        section.vertices = std::vector<int>{vertices.begin(), vertices.end()};
    }
}
