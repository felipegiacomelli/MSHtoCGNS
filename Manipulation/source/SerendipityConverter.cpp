#include "MSHtoCGNS/Manipulation/SerendipityConverter.hpp"
#include <cgnslib.h>

SerendipityConverter::SerendipityConverter(boost::shared_ptr<GridData> gridData) : gridData(gridData) {
    if (this->gridData->dimension != 2) {
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "gridData dimension must be 2, not " + std::to_string(gridData->dimension)));
    }
    this->serendipity = boost::make_shared<GridData>();
    this->serendipity->dimension = this->gridData->dimension;
}

void SerendipityConverter::convert() {
    std::set<int> removedVertices;
    for (const auto& connectivity : this->gridData->connectivities) {
        switch (connectivity.front()) {
            case TRI_10:
                removedVertices.insert(connectivity[10]);
                break;
            case QUAD_9:
                removedVertices.insert(connectivity[9]);
                break;
            case QUAD_16:
                removedVertices.insert(connectivity[13]);
                removedVertices.insert(connectivity[14]);
                removedVertices.insert(connectivity[15]);
                removedVertices.insert(connectivity[16]);
                break;
            default:
                continue;
        }
    }
}
