#include <pybind11/pybind11.h>

#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"

void registerCgnsWriter(pybind11::module& m);

PYBIND11_MODULE(PybindCgnsInterface, m) {
    registerCgnsWriter(m);
}
