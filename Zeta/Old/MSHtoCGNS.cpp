#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureMshReader.hpp"
#include <cgnslib.h>

#define TOLERANCE 1e-12

TestCase(Dimension) {
    // std::cout << "ElementTypeNull: " << ElementTypeNull << std::endl;
    // std::cout << "ElementTypeUserDefined: " << ElementTypeUserDefined << std::endl;
    // std::cout << "NODE: " << NODE << std::endl;
    // std::cout << "BAR_2: " << BAR_2 << std::endl;
    // std::cout << "BAR_3: " << BAR_3 << std::endl;
    // std::cout << "TRI_3: " << TRI_3 << std::endl;
    // std::cout << "TRI_6: " << TRI_6 << std::endl;
    // std::cout << "QUAD_4: " << QUAD_4 << std::endl;
    // std::cout << "QUAD_8: " << QUAD_8 << std::endl;
    // std::cout << "QUAD_9: " << QUAD_9 << std::endl;
    // std::cout << "TETRA_4: " << TETRA_4 << std::endl;
    // std::cout << "TETRA_10: " << TETRA_10 << std::endl;
    // std::cout << "PYRA_5: " << PYRA_5 << std::endl;
    // std::cout << "PYRA_14: " << PYRA_14 << std::endl;
    // std::cout << "PENTA_6: " << PENTA_6 << std::endl;
    // std::cout << "PENTA_15: " << PENTA_15 << std::endl;
    // std::cout << "PENTA_18: " << PENTA_18 << std::endl;
    // std::cout << "HEXA_8: " << HEXA_8 << std::endl;
    // std::cout << "HEXA_20: " << HEXA_20 << std::endl;
    // std::cout << "HEXA_27: " << HEXA_27 << std::endl;
    // std::cout << "MIXED: " << MIXED << std::endl;
    // std::cout << "PYRA_13: " << PYRA_13 << std::endl;
    // std::cout << "NGON_n: " << NGON_n << std::endl;
    // std::cout << "NFACE_n: " << NFACE_n << std::endl;
    // std::cout << "BAR_4: " << BAR_4 << std::endl;
    // std::cout << "TRI_9: " << TRI_9 << std::endl;
    // std::cout << "TRI_10: " << TRI_10 << std::endl;
    // std::cout << "QUAD_12: " << QUAD_12 << std::endl;
    // std::cout << "QUAD_16: " << QUAD_16 << std::endl;
    // std::cout << "TETRA_16: " << TETRA_16 << std::endl;
    // std::cout << "TETRA_20: " << TETRA_20 << std::endl;
    // std::cout << "PYRA_21: " << PYRA_21 << std::endl;
    // std::cout << "PYRA_29: " << PYRA_29 << std::endl;
    // std::cout << "PYRA_30: " << PYRA_30 << std::endl;
    // std::cout << "PENTA_24: " << PENTA_24 << std::endl;
    // std::cout << "PENTA_38: " << PENTA_38 << std::endl;
    // std::cout << "PENTA_40: " << PENTA_40 << std::endl;
    // std::cout << "HEXA_32: " << HEXA_32 << std::endl;
    // std::cout << "HEXA_56: " << HEXA_56 << std::endl;
    // std::cout << "HEXA_64: " << HEXA_64 << std::endl;
    // std::cout << std::endl;

    // std::cout << "" << "ElementTypeNull: " << ElementTypeNull << std::endl;
    // std::cout << "" << "ElementTypeUserDefined: " << ElementTypeUserDefined << std::endl;

    std::cout << "\t{" << "15" << ", " << "NODE"     /*<< NODE     */ << "}," << std::endl;
    std::cout << "\t{" << "1"  << ", " << "BAR_2"    /*<< BAR_2    */ << "}," << std::endl;
    std::cout << "\t{" << "2"  << ", " << "TRI_3"    /*<< TRI_3    */ << "}," << std::endl;
    std::cout << "\t{" << "3"  << ", " << "QUAD_4"   /*<< QUAD_4   */ << "}," << std::endl;
    std::cout << "\t{" << "4"  << ", " << "TETRA_4"  /*<< TETRA_4  */ << "}," << std::endl;
    std::cout << "\t{" << "5"  << ", " << "HEXA_8"   /*<< HEXA_8   */ << "}," << std::endl;
    std::cout << "\t{" << "6"  << ", " << "PENTA_6"  /*<< PENTA_6  */ << "}," << std::endl;
    std::cout << "\t{" << "7"  << ", " << "PYRA_5"   /*<< PYRA_5   */ << "}," << std::endl;
    std::cout << "\t{" << "8"  << ", " << "BAR_3"    /*<< BAR_3    */ << "}," << std::endl;
    std::cout << "\t{" << "9"  << ", " << "TRI_6"    /*<< TRI_6    */ << "}," << std::endl;
    std::cout << "\t{" << "10" << ", " << "QUAD_9"   /*<< QUAD_9   */ << "}," << std::endl;
    std::cout << "\t{" << "11" << ", " << "TETRA_10" /*<< TETRA_10 */ << "}," << std::endl;
    std::cout << "\t{" << "12" << ", " << "HEXA_27"  /*<< HEXA_27  */ << "}," << std::endl;
    std::cout << "\t{" << "13" << ", " << "PENTA_18" /*<< PENTA_18 */ << "}," << std::endl;
    std::cout << "\t{" << "14" << ", " << "PYRA_14"  /*<< PYRA_14  */ << "}," << std::endl;
    std::cout << "\t{" << "16" << ", " << "QUAD_8"   /*<< QUAD_8   */ << "}," << std::endl;
    std::cout << "\t{" << "17" << ", " << "HEXA_20"  /*<< HEXA_20  */ << "}," << std::endl;
    std::cout << "\t{" << "18" << ", " << "PENTA_15" /*<< PENTA_15 */ << "}," << std::endl;
    std::cout << "\t{" << "19" << ", " << "PYRA_13"  /*<< PYRA_13  */ << "}," << std::endl;
    std::cout << "\t{" << "26" << ", " << "BAR_4"    /*<< BAR_4    */ << "}," << std::endl;
    std::cout << "\t{" << "21" << ", " << "TRI_10"   /*<< TRI_10   */ << "}," << std::endl;
    std::cout << "\t{" << "29" << ", " << "TETRA_20" /*<< TETRA_20 */ << "}," << std::endl;
    std::cout << "\t{" << "92" << ", " << "HEXA_64"  /*<< HEXA_64  */ << "}" << std::endl;
    std::cout << std::endl;

    // std::cout << "" << "TRI_9: " << TRI_9 << std::endl;
    // std::cout << "" << "QUAD_12: " << QUAD_12 << std::endl;
    // std::cout << "" << "QUAD_16: " << QUAD_16 << std::endl;
    // std::cout << "" << "TETRA_16: " << TETRA_16 << std::endl;
    // std::cout << "" << "PYRA_21: " << PYRA_21 << std::endl;
    // std::cout << "" << "PYRA_29: " << PYRA_29 << std::endl;
    // std::cout << "" << "PYRA_30: " << PYRA_30 << std::endl;
    // std::cout << "" << "PENTA_24: " << PENTA_24 << std::endl;
    // std::cout << "" << "PENTA_38: " << PENTA_38 << std::endl;
    // std::cout << "" << "PENTA_40: " << PENTA_40 << std::endl;
    // std::cout << "" << "HEXA_32: " << HEXA_32 << std::endl;
    // std::cout << "" << "HEXA_56: " << HEXA_56 << std::endl;
}
