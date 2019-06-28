#include "MSHtoCGNS/FileMend/WellGenerator.hpp"

WellGenerator::WellGenerator(boost::shared_ptr<GridData> gridData, std::string wellGeneratorScript) : gridData(gridData) {
    this->checkGridData();
    boost::property_tree::read_json(wellGeneratorScript, this->propertyTree);
    this->readScript();
    this->generateWells();
}

WellGenerator::WellGenerator(boost::shared_ptr<GridData> gridData, boost::property_tree::ptree propertyTree) : gridData(gridData), propertyTree(propertyTree) {
    this->checkGridData();
    this->readScript();
    this->generateWells();
}

void WellGenerator::checkGridData() {
    if (this->gridData->dimension != 3)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

    if (this->gridData->wells.size() != 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Number of wells in gridData must be 0 and not " + std::to_string(this->gridData->wells.size()));

    if (this->gridData->lines.size() != 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Number of lines in gridData must be 0 and not " + std::to_string(this->gridData->lines.size()));
}

void WellGenerator::readScript() {
    for (const auto& wellRegion : this->propertyTree.get_child("wellRegions")) {
        this->wellGeneratorDatum.emplace_back();

        this->wellGeneratorDatum.back().regionName = wellRegion.second.get<std::string>("regionName");

        int index = 0;
        for (const auto& coordinate : wellRegion.second.get_child("wellStart"))
            this->wellGeneratorDatum.back().wellStart[index++] = coordinate.second.get_value<double>();

        this->wellGeneratorDatum.back().wellName = wellRegion.second.get<std::string>("wellName");
    }
}

void WellGenerator::generateWells() {
        this->linesShift = this->gridData->tetrahedrons.size() + this->gridData->hexahedrons.size() + this->gridData->prisms.size()
                                        + this->gridData->pyramids.size() + this->gridData->triangles.size() + this->gridData->quadrangles.size();

    for (auto wellGeneratorData : this->wellGeneratorDatum) {

        auto wellRegion = std::find_if(this->gridData->regions.cbegin(), this->gridData->regions.cend(), [=](auto r){return r.name == wellGeneratorData.regionName;});

        for (auto i = this->gridData->prisms.cbegin(); i != this->gridData->prisms.cend(); ++i)
            if (i->back() >= wellRegion->begin && i->back() < wellRegion->end)
                this->prisms.emplace_back(i->cbegin(), i->cend()-1);

        for (const auto& prism : this->prisms)
            for (const auto& index : prism)
                if (isClose(this->gridData->coordinates[index], wellGeneratorData.wellStart))
                    this->currentIndex = index;

        this->defineQuantities();

        std::vector<int> vertices;
        vertices.push_back(this->currentIndex);

        for (int k = 0; k < this->numberOfSegments; ++k) {
            std::set<int> wellStartPrisms;
            for (auto i = 0u; i < this->prisms.size(); ++i)
                if (std::find(this->prisms[i].cbegin(), this->prisms[i].cend(), this->currentIndex) != this->prisms[i].cend())
                    wellStartPrisms.insert(i);

            std::set<int> wellStartVertices;
            for (const auto& prismIndex : wellStartPrisms)
                wellStartVertices.insert(this->prisms[prismIndex].cbegin(), this->prisms[prismIndex].cend());

            std::unordered_map<int, int> map;
            for (const auto& vertexIndex : wellStartVertices)
                for (const auto& prismIndex : wellStartPrisms)
                    if (std::find(this->prisms[prismIndex].cbegin(), this->prisms[prismIndex].cend(), vertexIndex) != this->prisms[prismIndex].cend())
                        map[vertexIndex]++;

            this->currentIndex = std::find_if(map.cbegin(), map.cend(), [=](auto entry){return entry.first != this->currentIndex && entry.second == numberOfElementsPerSection;})->first;

            for (auto rit = wellStartPrisms.crbegin(); rit != wellStartPrisms.crend(); ++rit)
                this->prisms.erase(this->prisms.begin() + *rit);

            vertices.push_back(this->currentIndex);
        }

        unsigned numberOfLines = vertices.size() - 1;

        for (unsigned i = 0; i < numberOfLines; ++i)
            this->gridData->lines.emplace_back(std::array<int, 3>{vertices[i], vertices[i+1], int(i) + this->linesShift});

        std::sort(vertices.begin(), vertices.end());

        WellData well;
        well.name = wellGeneratorData.wellName;
        well.begin = this->linesShift;
        well.end = this->linesShift +  numberOfLines;
        well.vertices = std::move(vertices);
        this->gridData->wells.emplace_back(std::move(well));

        this->linesShift +=  numberOfLines;
    }
}

void WellGenerator::defineQuantities() {
    std::set<int> wellStartPrisms;
    for (auto i = 0u; i < this->prisms.size(); ++i)
        if (std::find(this->prisms[i].cbegin(), this->prisms[i].cend(), this->currentIndex) != this->prisms[i].cend())
            wellStartPrisms.insert(i);

    this->numberOfPrisms = this->prisms.size();
    this->numberOfElementsPerSection = wellStartPrisms.size();
    this->numberOfSegments = this->numberOfPrisms / this->numberOfElementsPerSection;
}

bool WellGenerator::isClose(const std::array<double, 3>& coordinate, const std::array<double, 3>& referencePoint) {
    bool close = true;

    for (int i = 0; i < 3; ++i)
        close &= fabs(coordinate[i] - referencePoint[i]) < this->tolerance;

    return close;
}
