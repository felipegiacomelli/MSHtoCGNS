#include "MSHtoCGNS/FileMend/CgnsReader/MultipleBasesCgnsReader3D.hpp"
#include <cgnslib.h>

MultipleBasesCgnsReader3D::MultipleBasesCgnsReader3D(std::string filePath) : CgnsReader3D(filePath, false) {
    this->readBases();
}

void MultipleBasesCgnsReader3D::readBases() {
    for (int base = 1; base <= this->numberOfBases; ++base) {
        if (cg_base_read(this->fileIndex, base, this->buffer, &this->cellDimension, &this->physicalDimension))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read base");
        this->bases[std::string(this->buffer)] = base;
    }
}

boost::shared_ptr<GridData> MultipleBasesCgnsReader3D::read(std::string name) {
    auto entry = std::find_if(this->bases.cbegin(), this->bases.cend(), [=](auto e){return e.first == name;});
    if (entry == this->bases.cend())
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Base " + name + " not found");

    this->baseIndex = entry->second;
    this->readBase();
    this->readZone();
    this->readNumberOfSections();
    this->readNumberOfBoundaries();
    this->createGridData();

    this->readCoordinates();
    this->readSections();
    this->findBoundaryVertices();
    this->findRegionVertices();
    this->findWellVertices();

    return boost::make_shared<GridData>(*this->gridData);
}

boost::shared_ptr<GridData> MultipleBasesCgnsReader3D::read(int baseIndex) {
    auto entry = std::find_if(this->bases.cbegin(), this->bases.cend(), [=](auto e){return e.second == baseIndex;});
    if (entry == this->bases.cend())
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Base " + std::to_string(baseIndex) + " not found");

    this->baseIndex = entry->second;
    this->readBase();
    this->readZone();
    this->readNumberOfSections();
    this->readNumberOfBoundaries();
    this->createGridData();

    this->readCoordinates();
    this->readSections();
    this->findBoundaryVertices();
    this->findRegionVertices();
    this->findWellVertices();

    return boost::make_shared<GridData>(*this->gridData);
}

MultipleBasesCgnsReader3D::~MultipleBasesCgnsReader3D() {
    for (auto entry : this->bases)
        printf("\n\t%10s : %i\n", entry.first.c_str(), entry.second);
}
