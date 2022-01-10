#include <pybind11/pybind11.h>

namespace py = pybind11;

void registerCgnsCreator(py::module& m);
void registerCgnsWriter(py::module& m);
void registerGridData(py::module& m);

PYBIND11_MODULE(PybindCgnsInterface, m) {
    registerCgnsCreator(m);
    registerCgnsWriter(m);
    registerGridData(m);
}
