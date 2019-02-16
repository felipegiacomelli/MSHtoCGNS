#include <chrono>

#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"
#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"
#include "MSHtoCGNS/FileMend/GridDataExtractor.hpp"
#include "MSHtoCGNS/FileMend/CgnsCreator/MultipleBasesCgnsCreator3D.hpp"
#include "MSHtoCGNS/Utilities/Print.hpp"

void removeDuplicatePoints(boost::shared_ptr<GridData> gridData);
void rectifyConnectivities(boost::shared_ptr<GridData> gridData, std::unordered_map<int, int> conversionTable);

template<typename T>
void rectifyIndices(std::unordered_map<int, int> conversionTable, std::vector<T>& indices) {
    for (auto& element : indices) {
        print1D(element.cbegin(), element.cend(), "\n");
        for (auto index = element.begin(); index != element.end() - 1; ++index)
            *index = conversionTable[*index];
        print1D(element.cbegin(), element.cend(), "\n\n");
    }
}

double distance(std::array<double, 3> a, std::array<double, 3> b) {
    return std::sqrt(std::inner_product(a.cbegin(), a.cend(), b.cbegin(), 0.0, std::plus<>(), [](auto c, auto d){return (c-d)*(c-d);}));
}

int main() {
    boost::property_tree::ptree script;
    boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMultipleBases.json", script);
    boost::filesystem::path inputPath(script.get<std::string>("path.input"));
    boost::filesystem::path outputPath(script.get<std::string>("path.output"));

    auto start = std::chrono::steady_clock::now();

    boost::shared_ptr<GridData> gridData;
    if (inputPath.extension() == std::string(".msh")) {
        MshReader3D reader(inputPath.string());
        gridData = reader.gridData;
    }
    else if (inputPath.extension() == std::string(".cgns")) {
        CgnsReader3D reader(inputPath.string());
        gridData = reader.gridData;
    }
    else
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - file extension " + inputPath.extension().string() + " not supported");

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsedSeconds = end - start;
    std::cout << std::endl << "\tGrid path: " << inputPath.string();
    std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

    printGridDataInformation(gridData, purple("original gridData"));

    //
        printf("\n\t%s: %3lu", "coordinates", gridData->coordinates.size());
        removeDuplicatePoints(gridData);
        printf("\n\t%s: %3lu", "coordinates", gridData->coordinates.size());
        printf("\n\n");

        std::vector<std::pair<std::set<int>, int>> nonpay;
        for (int i = gridData->regions[0].begin; i <  gridData->regions[0].end; ++i)
            nonpay.emplace_back(std::make_pair(std::set<int>(gridData->hexahedrons[i].cbegin(), gridData->hexahedrons[i].cend()-1), i));

        std::vector<std::pair<std::set<int>, int>> pay;
        for (int i = gridData->regions[1].begin; i <  gridData->regions[1].end; ++i)
            pay.emplace_back(std::make_pair(std::set<int>(gridData->hexahedrons[i].cbegin(), gridData->hexahedrons[i].cend()-1), i));

        printf("\n\t%s: %2lu", "nonpay", nonpay.size());
        printf("\n\t%s: %2lu", "pay", pay.size());
        printf("\n\n");

        auto hex = pay.cbegin();
        for (auto nonHex = nonpay.begin(); nonHex != nonpay.end();)
            if (nonHex->first == hex->first) {
                printf("\t%2i\n\n", nonHex->second);
                nonHex = nonpay.erase(nonHex);
                ++hex;
            }
            else
                ++nonHex;

        // printf("nonpay\n\n");
        // for (auto nonHex : nonpay)
        //     printf("\t%2i\n", nonHex.second);

        // printf("pay\n\n");
        // for (auto hex : pay)
        //     printf("\t%2i\n", hex.second);

        printf("\n\t%s: %2lu", "nonpay", nonpay.size());
        printf("\n\t%s: %2lu", "pay", pay.size());
        printf("\n\n");

        std::vector<std::array<int, 9>> newHexahedrons;
        for (auto i = 0u; i < pay.size(); ++i) {
            newHexahedrons.emplace_back(gridData->hexahedrons[pay[i].second]);
            newHexahedrons.back().back() = i;
        }

        for (auto i = 0u; i < nonpay.size(); ++i) {
            newHexahedrons.emplace_back(gridData->hexahedrons[nonpay[i].second]);
            newHexahedrons.back().back() = pay.size() + i;
        }

        gridData->hexahedrons = newHexahedrons;

        // printf("newHexahedrons\n\n");
        // for (auto nonHex : newHexahedrons)
        //     printf("\t%2i\n", nonHex.back());

        gridData->regions[0].begin = pay.size();
        gridData->regions[0].end = pay.size() + nonpay.size();
        gridData->regions[0].name = "NONPAY";

        gridData->regions[1].begin = 0;
        gridData->regions[1].end = pay.size();
        gridData->regions[1].name = "PAY";

        std::swap(gridData->regions[0], gridData->regions[1]);

        for (auto& quadrangle : gridData->quadrangles) {
            quadrangle.back() -= 8;
            printf("\t%3i\n", quadrangle.back());
        }

        gridData->boundaries.erase(gridData->boundaries.begin() + 6, gridData->boundaries.end());
        for (auto& boundary : gridData->boundaries) {
            boundary.begin -= 8;
            boundary.end -= 8;
        }
        gridData->boundaries.emplace_back();
        gridData->boundaries.back().name = "INTERFACE";
        gridData->boundaries.back().begin = 160;
        gridData->boundaries.back().end = 184;
        printGridDataInformation(gridData, purple("mod gridData"));

        CgnsCreator3D(gridData, "/home/felipe/Downloads/");
    //

    boost::shared_ptr<GridData> extract;
    if (script.get_child_optional("ScriptGridDataExtractor")) {
        GridDataExtractor gridDataExtractor(gridData, script.get_child("ScriptGridDataExtractor"));
        extract = gridDataExtractor.extract;
    }

    std::vector<boost::shared_ptr<GridData>> gridDatas{gridData, extract};
    std::vector<std::string> baseNames{"Rock", "Reservoir"};
    std::vector<std::string> zoneNames{"Rock", "Reservoir"};

    printGridDataInformation(extract, purple("extract gridData"));

    start = std::chrono::steady_clock::now();
    // MultipleBasesCgnsCreator3D(gridDatas, baseNames, zoneNames, outputPath.string());
    // MultipleBasesCgnsCreator3D(gridDatas, baseNames, zoneNames, outputPath.string());
    end = std::chrono::steady_clock::now();

    elapsedSeconds = end - start;
    std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
    std::cout << std::endl << "\tOutput file location       : " << outputPath.string() << std::endl << std::endl;

    return 0;
}

void removeDuplicatePoints(boost::shared_ptr<GridData> gridData) {
    std::unordered_map<int, int> conversionTable;
    std::vector<std::array<double, 3>> newCoordinates;
    for (auto p = 0u; p < gridData->coordinates.size(); ++p) {
        auto point = gridData->coordinates[p];
        bool duplicate = false;
        for (auto q = 0u; q < newCoordinates.size(); ++q) {
            if (distance(point, newCoordinates[q]) < 1.0e-6) {
                conversionTable[p] = q;
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            newCoordinates.push_back(point);
            conversionTable[p] = newCoordinates.size() - 1;
        }
    }
    rectifyConnectivities(gridData, conversionTable);
    gridData->coordinates = newCoordinates;
}

void rectifyConnectivities(boost::shared_ptr<GridData> gridData, std::unordered_map<int, int> conversionTable) {
    rectifyIndices(conversionTable, gridData->quadrangles);
    rectifyIndices(conversionTable, gridData->hexahedrons);
}
