#include "MSHtoCGNS/MshInterface/MshReader.hpp"
#include <cgnslib.h>

boost::unordered_map<int, int> mshToCgns{{1, BAR_2}, {2, TRI_3}, {3, QUAD_4}, {4, TETRA_4}, {5, HEXA_8}, {6, PENTA_6}, {7, PYRA_5}};

MshReader::MshReader(std::string filePath) : filePath(filePath) {
    this->checkFile();
    this->gridData = boost::make_shared<GridData>();
    this->readPhysicalNames();
    this->readEntities();
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

    this->findLine("$MeshFormat");
    std::getline(this->file, this->line);
    this->version = this->line.substr(0, this->line.find_first_of(" "));
    this->isLegacy = this->version == "2.2";

    if (this->version != "2.2" && this->version != "4.1")
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - File version must be either 2.2 or 4.1, not " + this->version);
}

void MshReader::readPhysicalNames() {
    this->findLine("$PhysicalNames");
    std::getline(this->file, this->line);

    while (std::getline(this->file, this->line) && this->line != "$EndPhysicalNames") {
        std::vector<std::string> entries;
        boost::split(entries, this->line, boost::is_any_of(" "));

        this->sectionsDimension.emplace_back(std::stoi(entries[0]));
        this->sectionsIndices.emplace_back(std::stoi(entries[1]) - 1);
        this->sectionsNames.emplace_back(boost::to_upper_copy(entries[2].substr(1, entries[2].size() - 2)));
    }

    this->gridData->dimension = *std::max_element(this->sectionsDimension.cbegin(), this->sectionsDimension.cend());
}

void MshReader::readEntities() {
    if (!this->isLegacy) {
        this->findLine("$Entities");
        std::getline(this->file, this->line);
        std::vector<std::string> entries;
        boost::split(entries, line, boost::is_any_of(" "));
        std::vector<int> numbersOfGeometricalEntities;
        for (auto entry : entries) {
            numbersOfGeometricalEntities.emplace_back(std::stoi(entry));
        }

        this->localToGlobal = std::vector<std::vector<int>>(4, std::vector<int>{});

        for (int dimension = 0; dimension <= 3; ++dimension) {
            auto& map = this->localToGlobal[dimension];
            map.resize(numbersOfGeometricalEntities[dimension]);

            for (int counter = 0; counter < numbersOfGeometricalEntities[dimension]; ++counter) {
                std::getline(this->file, this->line);
                if (line == "$EndEntities")
                    break;

                std::vector<std::string> entries;
                boost::split(entries, this->line, boost::is_any_of(" "));

                int position = entries.size() > 7 ? 7 : 4;
                int numberOfPhysicalEntities = std::stoi(entries[position]);
                int firstPhysicalEntity = numberOfPhysicalEntities > 0 ? std::stoi(entries[position + 1]) - 1 : -1;

                map[counter] = firstPhysicalEntity;
            }

            map.erase(std::remove_if(map.begin(), map.end(), [](auto p){return p == -1;}), map.end());
        }
    }
}

void MshReader::readNodes() {
    this->findLine("$Nodes");
    std::getline(this->file, this->line);
    if (this->isLegacy) {
        int numberOfVertices = std::stoi(this->line);
        this->gridData->numberOfLocalVertices = numberOfVertices;
        this->gridData->coordinates.resize(numberOfVertices, std::array<double, 3>());
        for (int temporary, vertex = 0; vertex < numberOfVertices; ++vertex) {
            this->file >> temporary >> this->gridData->coordinates[vertex][0] >> this->gridData->coordinates[vertex][1] >> this->gridData->coordinates[vertex][2];
        }
    }
    else {
        int position = this->line.find_first_of(" ");
        int numberOfVertices = std::stoi(this->line.substr(position, this->line.find_first_of(" ", position + 1)));

        std::vector<std::pair<int, std::array<double, 3>>> coordinates;
        coordinates.reserve(numberOfVertices);

        while (std::getline(this->file, this->line) && this->line != "$EndNodes") {
            // entityDim(int) entityTag(int) parametric(int) numNodesInBlock(size_t)
            std::vector<std::pair<int, std::array<double, 3>>> vertices(std::stoi(this->line.substr(this->line.find_last_of(" "))));

            // nodeTag(size_t)
            for (unsigned vertex = 0; vertex < vertices.size(); ++vertex) {
                std::getline(this->file, this->line);
                vertices[vertex].first = std::stoi(this->line) - 1;
            }

            // x(double) y(double) z(double)
            for (unsigned vertex = 0; vertex < vertices.size(); ++vertex) {
                std::getline(this->file, this->line);
                std::vector<std::string> entries;
                boost::split(entries, this->line, boost::is_any_of(" "));
                vertices[vertex].second = std::array<double, 3>{std::stod(entries[0]), std::stod(entries[1]), std::stod(entries[2])};
            }

            coordinates.insert(coordinates.end(), vertices.begin(), vertices.end());
        }

        std::sort(coordinates.begin(), coordinates.end(), [](const auto& a, const auto& b){return a.first < b.first;});

        this->gridData->coordinates.reserve(numberOfVertices);

        std::transform(coordinates.begin(), coordinates.end(), std::back_inserter(this->gridData->coordinates), [](const auto& pair){return pair.second;});
    }
}

void MshReader::readElements() {
    this->findLine("$Elements");
    std::getline(this->file, this->line);
    if (this->isLegacy) {
        this->connectivities.reserve(std::stoi(this->line));

        while (std::getline(this->file, this->line) && this->line != "$EndElements") {
            std::vector<std::string> entries;
            boost::split(entries, this->line, boost::is_any_of(" "));

            auto& connectivity = this->connectivities.emplace_back();
            connectivity.reserve(entries.size());
            std::transform(entries.cbegin(), entries.cend(), std::back_inserter(connectivity), [](const auto& e){return std::stoi(e) - 1;});
        }

        if (connectivities[0][2] != 1)
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Elements must have exactly 2 tags");

        // Before
        // index, type, number-of-tags, physical-section-index, geometrical-section-index, node-number-list

        for (auto& connectivity : this->connectivities) {
            connectivity.erase(connectivity.begin() + 4);
            connectivity.erase(connectivity.begin() + 2);
            connectivity.erase(connectivity.begin());
            connectivity[0] = mshToCgns[connectivity[0] + 1];
            std::iter_swap(connectivity.begin(), connectivity.begin() + 1);
        }
    }
    else {
        int position = this->line.find_first_of(" ");
        int numberOfElements = std::stoi(this->line.substr(position, this->line.find_first_of(" ", position + 1)));
        this->connectivities.reserve(numberOfElements);

        while (std::getline(this->file, this->line) && this->line != "$EndElements") {
            std::vector<std::string> entries;
            boost::split(entries, line, boost::is_any_of(" "));
            int entityDim = std::stoi(entries[0]);
            int entityTag = std::stoi(entries[1]) - 1;
            int elementType = std::stoi(entries[2]);
            int elementsInBlock = std::stoi(entries[3]);

            std::vector<std::string> elements(elementsInBlock);

            for (int element = 0; element < elementsInBlock; ++element) {
                auto& connectivity = this->connectivities.emplace_back(std::vector<int>{this->localToGlobal[entityDim][entityTag], mshToCgns[elementType]});
                std::getline(file, line);
                std::vector<std::string> conns;
                boost::split(conns, line, boost::is_any_of(" "));

                // Before
                // elementTag(size_t) nodeTag(size_t)

                for (auto conn = conns.cbegin() + 1; conn != conns.cend() - 1; ++conn) {
                    connectivity.emplace_back(std::atoi(conn->c_str()) - 1);
                }
            }
        }
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
        auto [begin, end] = std::equal_range(indices.cbegin(), indices.cend(), index);
        this->physicalEntitiesRange.emplace_back(std::array<long int, 2>{std::distance(indices.cbegin(), begin), std::distance(indices.cbegin(), end)});
    }
}

void MshReader::addSections() {
    for (unsigned physicalEntity = 0; physicalEntity < this->sectionsNames.size(); ++physicalEntity) {
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

void MshReader::findLine(std::string content) {
    while (std::getline(this->file, this->line))
        if (this->line == content)
            break;
    if (this->file.eof())
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no " + content + " in the .msh file");
}

MshReader::~MshReader() {
    this->file.close();
}
