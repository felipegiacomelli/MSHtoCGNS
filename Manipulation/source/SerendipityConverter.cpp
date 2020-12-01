#include "MSHtoCGNS/Manipulation/SerendipityConverter.hpp"
#include <cgnslib.h>

SerendipityConverter::SerendipityConverter(boost::shared_ptr<GridData> gridData) : gridData(gridData) {
    if (this->gridData->dimension != 2) {
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "gridData dimension must be 2, not " + std::to_string(gridData->dimension)));
    }
    this->serendipity = boost::make_shared<GridData>();
    this->serendipity->dimension = this->gridData->dimension;
    this->serendipity->connectivities = this->gridData->connectivities;
    this->serendipity->sections = this->gridData->sections;
    this->convert();
}

void SerendipityConverter::convert() {
    this->convertElements();
    this->setCoordinates();
    this->rectifyConnectivities();
}

void SerendipityConverter::convertElements() {
    for (auto& connectivity : this->serendipity->connectivities) {
        std::vector<int> remove;
        switch (connectivity.front()) {
            case QUAD_9: {
                connectivity[0] = QUAD_8;
                remove = std::vector<int>{9};
                break;
            }
            case TRI_10: {
                connectivity[0] = TRI_9;
                remove = std::vector<int>{10};
                break;
            }
            case QUAD_16: {
                connectivity[0] = QUAD_12;
                remove = std::vector<int>{16, 15, 14, 13};
                break;
            }
            default:
                continue;
        }
        this->convertToSerendipity(connectivity, remove);
    }
}

void SerendipityConverter::convertToSerendipity(std::vector<int>& connectivity, std::vector<int> remove) {
    for (auto r : remove) {
        this->verticesToRemove.insert(connectivity[r]);
        connectivity.erase(connectivity.begin() + r);
    }
}

void SerendipityConverter::setCoordinates() {
    this->coordinates.reserve(this->gridData->coordinates.size());
    for (unsigned c = 0; c < this->gridData->coordinates.size(); ++c)
        this->coordinates.emplace_back(std::make_pair(c, this->gridData->coordinates[c]));

    for (auto v = this->verticesToRemove.crbegin(); v != this->verticesToRemove.crend(); ++v)
        this->coordinates.erase(this->coordinates.begin() + *v);

    this->serendipity->numberOfLocalVertices = this->coordinates.size();
    this->serendipity->coordinates.reserve(this->coordinates.size());
    for (auto coordinate : this->coordinates)
        this->serendipity->coordinates.emplace_back(coordinate.second);
}

void SerendipityConverter::rectifyConnectivities() {
    std::unordered_map<int, int> originalToSerendipity;
    int counter = 0;
    for (auto coordinate : this->coordinates)
        originalToSerendipity[coordinate.first] = counter++;

    for (auto& connectivity : this->serendipity->connectivities)
        for (auto index = connectivity.begin() + 1; index != connectivity.end() - 1; ++index)
            *index = originalToSerendipity[*index];
}
