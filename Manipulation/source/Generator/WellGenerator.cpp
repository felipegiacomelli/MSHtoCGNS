#include "MSHtoCGNS/Manipulation/Generator/WellGenerator.hpp"
#include <cgnslib.h>

WellGenerator::WellGenerator(boost::shared_ptr<GridData> gridData, std::string scriptPath) : Generator(gridData, scriptPath) {
    this->checkGridData();
    this->generateWells();
}

WellGenerator::WellGenerator(boost::shared_ptr<GridData> gridData, boost::property_tree::ptree script) : Generator(gridData, script) {
    this->checkGridData();
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

void WellGenerator::generateWells() {
   auto position = std::find_if(this->gridData->connectivities.cbegin(), this->gridData->connectivities.cend(), [](const auto& c){return c[0] == BAR_2;});
   this->linesShift = std::distance(this->gridData->connectivities.cbegin(), position);

    std::vector<Point> coordinates;
    coordinates.reserve(this->gridData->coordinates.size());
    for (auto c : this->gridData->coordinates) {
        coordinates.emplace_back(Point(c[0], c[1], c[2]));
    }

    for (const auto& wellRegion : this->script.get_child("wells")) {
        auto regionName = wellRegion.second.get<std::string>("regionName");
        auto wellName = wellRegion.second.get<std::string>("wellName");
        Point begin;
        boost::geometry::read_wkt(boost::str(boost::format("POINT(%s)") % wellRegion.second.get<std::string>("wellStart")), begin);

        auto region = std::find_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [=](auto e){return e.name == regionName;});

        for (auto element = this->gridData->connectivities.cbegin() + region->begin; element != this->gridData->connectivities.cbegin() + region->end; ++element)
            if (element->front() == PENTA_6)
                this->prisms.emplace_back(element->cbegin() + 1, element->cend() - 1);

        for (const auto& prism : this->prisms)
            for (const auto& index : prism)
                if (boost::geometry::distance(coordinates[index], begin) < this->tolerance)
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

            boost::unordered_map<int, int> map;
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

        this->gridData->sections.emplace_back(SectionData{wellName, 1, this->linesShift, this->linesShift +  numberOfLines, vertices});

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
