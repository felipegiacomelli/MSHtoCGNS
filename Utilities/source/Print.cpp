#include "MSHtoCGNS/Utilities/Print.hpp"
#include <cgnslib.h>

std::string red(std::string message) {
    return "\033[1;31m" + message + "\033[0m";
}

std::string yellow(std::string message) {
    return "\033[1;33m" + message + "\033[0m";
}

std::string purple(std::string message) {
    return "\033[1;35m" + message + "\033[0m";
}

std::string cyan(std::string message) {
    return "\033[1;36m" + message + "\033[0m";
}

void printEntity(SectionData section) {
    std::cout << std::endl << "\t\t\t" << std::setw(20) << std::left << section.name << ": " << std::setw(6) << std::right << section.begin << " - " << section.end << "\t|" << std::setw(6) << section.end - section.begin;
}

void printGridDataInformation(boost::shared_ptr<GridData> gD, std::string message) {
    std::cout << std::setfill(' ');

    std::cout << std::endl << "\t" << message;

    int numberOfVertices = gD->coordinates.size();
    std::cout << std::endl << "\n\t\tnumberOfVertices: " << numberOfVertices;

    int numberOfElements = gD->connectivities.size();
    std::cout << "\n\n\t\tnumberOfElements: " << numberOfElements << std::endl;
    std::cout << "\n\t\t\ttetrahedrons: " << std::count_if(gD->connectivities.cbegin(), gD->connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;});
    std::cout << "\n\t\t\thexahedrons: " << std::count_if(gD->connectivities.cbegin(), gD->connectivities.cend(), [](const auto& c){return c.front() == HEXA_8;});
    std::cout << "\n\t\t\tprisms: " << std::count_if(gD->connectivities.cbegin(), gD->connectivities.cend(), [](const auto& c){return c.front() == PENTA_6;});
    std::cout << "\n\t\t\tpyramids: " << std::count_if(gD->connectivities.cbegin(), gD->connectivities.cend(), [](const auto& c){return c.front() == PYRA_5;});
    std::cout << "\n\t\t\ttriangle: " << std::count_if(gD->connectivities.cbegin(), gD->connectivities.cend(), [](const auto& c){return c.front() == TRI_3;});
    std::cout << "\n\t\t\tquadrangle: " << std::count_if(gD->connectivities.cbegin(), gD->connectivities.cend(), [](const auto& c){return c.front() == QUAD_4;});
    std::cout << "\n\t\t\tlines: " << std::count_if(gD->connectivities.cbegin(), gD->connectivities.cend(), [](const auto& c){return c.front() == BAR_2;});

    std::cout << std::endl << "\n\t\tnumberOfRegions: " << std::count_if(gD->sections.cbegin(), gD->sections.cend(), [](const auto& e){return e.dimension == 3;}) << std::endl;
    for (auto section : gD->sections)
        if (section.dimension == 3)
            printEntity(section);

    std::cout << std::endl << "\n\t\tnumberOfBoundaries: " << std::count_if(gD->sections.cbegin(), gD->sections.cend(), [](const auto& e){return e.dimension == 2;}) << std::endl;
    for (auto section : gD->sections)
        if (section.dimension == 2)
            printEntity(section);

    std::cout << std::endl << "\n\t\tnumberOfWells: " << std::count_if(gD->sections.cbegin(), gD->sections.cend(), [](const auto& e){return e.dimension == 1;}) << std::endl;
    for (auto section : gD->sections)
        if (section.dimension == 1)
            printEntity(section);

    std::cout << std::endl << std::endl;
}
