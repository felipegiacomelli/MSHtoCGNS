#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cgnslib.h>

int main() {
    printf("\n\tCGNS Element types\n");
    printf("\n\t\t%22s: %2i", "ElementTypeNull", ElementTypeNull);
    printf("\n\t\t%22s: %2i", "ElementTypeUserDefined", ElementTypeUserDefined);
    printf("\n\t\t%22s: %2i", "NODE", NODE);
    printf("\n\t\t%22s: %2i", "BAR_2", BAR_2);
    printf("\n\t\t%22s: %2i", "BAR_3", BAR_3);
    printf("\n\t\t%22s: %2i", "TRI_3", TRI_3);
    printf("\n\t\t%22s: %2i", "TRI_6", TRI_6);
    printf("\n\t\t%22s: %2i", "QUAD_4", QUAD_4);
    printf("\n\t\t%22s: %2i", "QUAD_8", QUAD_8);
    printf("\n\t\t%22s: %2i", "QUAD_9", QUAD_9);
    printf("\n\t\t%22s: %2i", "TETRA_4", TETRA_4);
    printf("\n\t\t%22s: %2i", "TETRA_10", TETRA_10);
    printf("\n\t\t%22s: %2i", "PYRA_5", PYRA_5);
    printf("\n\t\t%22s: %2i", "PYRA_14", PYRA_14);
    printf("\n\t\t%22s: %2i", "PENTA_6", PENTA_6);
    printf("\n\t\t%22s: %2i", "PENTA_15", PENTA_15);
    printf("\n\t\t%22s: %2i", "PENTA_18", PENTA_18);
    printf("\n\t\t%22s: %2i", "HEXA_8", HEXA_8);
    printf("\n\t\t%22s: %2i", "HEXA_20", HEXA_20);
    printf("\n\t\t%22s: %2i", "HEXA_27", HEXA_27);
    printf("\n\t\t%22s: %2i", "MIXED", MIXED);
    printf("\n\t\t%22s: %2i", "PYRA_13", PYRA_13);
    printf("\n\t\t%22s: %2i", "NGON_n", NGON_n);
    printf("\n\t\t%22s: %2i", "NFACE_n", NFACE_n);
    printf("\n\t\t%22s: %2i", "BAR_4", BAR_4);
    printf("\n\t\t%22s: %2i", "TRI_9", TRI_9);
    printf("\n\t\t%22s: %2i", "TRI_10", TRI_10);
    printf("\n\t\t%22s: %2i", "QUAD_12", QUAD_12);
    printf("\n\t\t%22s: %2i", "QUAD_16", QUAD_16);
    printf("\n\t\t%22s: %2i", "TETRA_16", TETRA_16);
    printf("\n\t\t%22s: %2i", "TETRA_20", TETRA_20);
    printf("\n\t\t%22s: %2i", "PYRA_21", PYRA_21);
    printf("\n\t\t%22s: %2i", "PYRA_29", PYRA_29);
    printf("\n\t\t%22s: %2i", "PYRA_30", PYRA_30);
    printf("\n\t\t%22s: %2i", "PENTA_24", PENTA_24);
    printf("\n\t\t%22s: %2i", "PENTA_38", PENTA_38);
    printf("\n\t\t%22s: %2i", "PENTA_40", PENTA_40);
    printf("\n\t\t%22s: %2i", "HEXA_32", HEXA_32);
    printf("\n\t\t%22s: %2i", "HEXA_56", HEXA_56);
    printf("\n\t\t%22s: %2i", "HEXA_64", HEXA_64);
    printf("\n\n");
    return 0;
}
