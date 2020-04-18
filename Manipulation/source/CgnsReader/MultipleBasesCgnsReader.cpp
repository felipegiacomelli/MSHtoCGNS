#include "MSHtoCGNS/Manipulation/CgnsReader/MultipleBasesCgnsReader.hpp"
#include <cgnslib.h>

MultipleBasesCgnsReader::MultipleBasesCgnsReader(std::string filePath) : CgnsReader(filePath, false) {
    this->readBases();
}

void MultipleBasesCgnsReader::readBases() {
    for (int base = 1; base <= this->numberOfBases; ++base) {
        if (cg_base_read(this->fileIndex, base, this->buffer, &this->cellDimension, &this->physicalDimension))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read base");

        this->bases[std::string(this->buffer)] = base;
    }
}

void MultipleBasesCgnsReader::print() const {
    for (auto entry : this->bases)
        std::cout << std::endl << "\t" << std::left << std::setw(15) << entry.first << " : " << entry.second << std::endl;
}

bool MultipleBasesCgnsReader::exists(std::string name) const {
    return this->bases.cend() != std::find_if(this->bases.cbegin(), this->bases.cend(), [=](auto e){return e.first == name;});
}

boost::shared_ptr<GridData> MultipleBasesCgnsReader::read(std::string name) {
    auto entry = std::find_if(this->bases.cbegin(), this->bases.cend(), [=](auto e){return e.first == name;});
    if (entry == this->bases.cend())
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Base " + name + " not found");

    this->baseIndex = entry->second;

    this->readBase();
    this->readZone();
    this->createGridData();
    this->CgnsReader::read();

    return boost::make_shared<GridData>(*this->gridData);
}

boost::shared_ptr<GridData> MultipleBasesCgnsReader::read(int baseIndex) {
    auto entry = std::find_if(this->bases.cbegin(), this->bases.cend(), [=](auto e){return e.second == baseIndex + 1;});
    if (entry == this->bases.cend())
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Base " + std::to_string(baseIndex) + " not found");

    this->baseIndex = entry->second;

    this->readBase();
    this->readZone();
    this->createGridData();
    this->CgnsReader::read();

    return boost::make_shared<GridData>(*this->gridData);
}
