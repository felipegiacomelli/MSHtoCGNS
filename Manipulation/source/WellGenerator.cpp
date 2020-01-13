#include "MSHtoCGNS/Manipulation/WellGenerator.hpp"
#include <cgnslib.h>

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

    if (std::count_if(this->gridData->connectivities.cbegin(), this->gridData->connectivities.cend(), [](const auto& c){return c[0] == BAR_2;}) != 0)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData lines size must be 0");

    if (std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& e){return e.dimension == 1;}) != 0)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Number of wells in gridData must be 0");
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
   auto position = std::find_if(this->gridData->connectivities.cbegin(), this->gridData->connectivities.cend(), [](const auto& c){return c[0] == BAR_2;});
   this->linesShift = std::distance(this->gridData->connectivities.cbegin(), position);

   for (auto wellGeneratorData : this->wellGeneratorDatum) {
        auto region = std::find_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [=](auto e){return e.name == wellGeneratorData.regionName;});

        for (auto element = this->gridData->connectivities.cbegin() + region->begin; element != this->gridData->connectivities.cbegin() + region->end; ++element)
            if (element->front() == PENTA_6)
                this->prisms.emplace_back(element->cbegin() + 1, element->cend() - 1);

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

        int numberOfLines = vertices.size() - 1;

        for (int i = 0; i < numberOfLines; ++i)
            this->gridData->connectivities.emplace_back(std::vector<int>{BAR_2, vertices[i], vertices[i+1], i + this->linesShift});

        std::sort(vertices.begin(), vertices.end());

        this->gridData->sections.emplace_back(SectionData{wellGeneratorData.wellName, 1, this->linesShift, this->linesShift +  numberOfLines, vertices});

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
