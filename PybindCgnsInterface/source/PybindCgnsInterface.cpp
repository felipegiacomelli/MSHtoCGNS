#include <pybind11/pybind11.h>

namespace py = pybind11;

void registerCgnsInterface(py::module& m);
void registerGridData(py::module& m);

PYBIND11_MODULE(PybindCgnsInterface, m) {
    registerCgnsInterface(m);
    registerGridData(m);
}
