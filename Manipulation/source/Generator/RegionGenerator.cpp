#include "MSHtoCGNS/Manipulation/Generator/RegionGenerator.hpp"
#include <cgnslib.h>

RegionGenerator::RegionGenerator(boost::shared_ptr<GridData> gridData, std::string scriptPath) : Generator(gridData, scriptPath) {
    this->generateCoordinates();
    this->generateRegions();
}

RegionGenerator::RegionGenerator(boost::shared_ptr<GridData> gridData, boost::property_tree::ptree script) : Generator(gridData, script) {
    this->generateCoordinates();
    this->generateRegions();
}

void RegionGenerator::generateCoordinates() {
    this->coordinates.reserve(this->gridData->coordinates.size());
    for (auto c : this->gridData->coordinates) {
        this->coordinates.emplace_back(Point(c[0], c[1], c[2]));
    }
}

void RegionGenerator::generateRegions() {
    for (const auto& wellRegion : this->script.get_child("regions")) {
        auto name = wellRegion.second.get<std::string>("name");
        Box box;
        boost::geometry::read_wkt(boost::str(boost::format("POINT(%s)") % wellRegion.second.get<std::string>("begin")), box.min_corner());
        boost::geometry::read_wkt(boost::str(boost::format("POINT(%s)") % wellRegion.second.get<std::string>("end")), box.max_corner());

        std::vector<std::vector<SectionData>::iterator> regions;
        for (auto section = this->gridData->sections.begin(); section != this->gridData->sections.end(); ++section) {
            if (section->dimension == this->gridData->dimension) {
                regions.emplace_back(section);
            }
        }
        std::sort(regions.begin(), regions.end(), [](const auto& a, const auto& b){return a->begin < b->begin;});

        auto& cs = this->gridData->connectivities;
        int notRemovedFromLastRegion = 0;
        for (auto region = regions.begin(); region != regions.end(); ++region) {
            for (auto connectivity = cs.begin() + (*region)->begin; connectivity != cs.begin() + (*region)->end; ++connectivity) {
                if (connectivity->at(0) == TETRA_4 || connectivity->at(0) == HEXA_8 || connectivity->at(0) == PENTA_6 || connectivity->at(0) == PYRA_5) {
                    if (this->isElementInRange(connectivity, box)) {
                        connectivity->back() = this->marker;
                        for (auto former = regions.begin(); former != region; ++former) {
                            (*former)->end++;
                        }
                        if (region != regions.end() - 1) {
                            notRemovedFromLastRegion++;
                        }
                    }
                }
            }
        }

        std::stable_sort(cs.begin(), cs.end(), [](const auto& a, const auto& b){return a.back() < b.back();});
        auto it = std::find_if_not(cs.begin(), cs.end(), [=](const auto& c){return c.back() == this->marker;});
        if (it == cs.end()) {
            throw std::runtime_error(error(__PRETTY_FUNCTION__, "There are no elements inside box"));
        }
        int end = std::distance(cs.begin(), it);

        for (auto region : regions) {
            region->begin += end;
        }
        regions.back()->begin -= notRemovedFromLastRegion;

        this->gridData->sections.insert(this->gridData->sections.begin(), SectionData(name, this->gridData->dimension, 0, end, std::vector<int>{}));

        for (auto connectivity = cs.begin(); connectivity != cs.end(); ++connectivity) {
            connectivity->back() = std::distance(cs.begin(), connectivity);
        }
    }
}

bool RegionGenerator::isElementInRange(std::vector<std::vector<int>>::iterator connectivity, const Box& box) {
    MultiPoint element;
    for (auto vertex = connectivity->cbegin() + 1; vertex != connectivity->cend() - 1; ++vertex) {
        boost::geometry::append(element, this->coordinates[*vertex]);
    }

    Point centroid;
    boost::geometry::centroid(element, centroid);

    return boost::geometry::within(centroid, box);
}
