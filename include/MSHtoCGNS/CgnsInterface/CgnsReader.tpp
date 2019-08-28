#pragma once

template<typename T>
void CgnsReader::addConnectivity(std::vector<T>& connectivities, const std::vector<int>& connectivity) {
    connectivities.emplace_back();
    std::copy_n(std::cbegin(connectivity), connectivity.size(), std::begin(connectivities.back()));
}

template<typename T>
void CgnsReader::addConnectivity(std::vector<T>& connectivities, const std::vector<int>& connectivity, int numberOfElements, int numberOfVertices) {
    for (int e = 0; e < numberOfElements; ++e) {
        connectivities.emplace_back();
        auto& element = connectivities.back();
        for (int k = 0; k < numberOfVertices; ++k) {
            element[k] = connectivity[e * numberOfVertices + k] - 1;
        }
        element.back() = this->elementStart - 1 + e;
    }
}
