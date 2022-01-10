#include <pybind11/pybind11.h>

#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"

PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>)

namespace py = pybind11;

void registerCgnsCreator(py::module& m) {
    py::class_<CgnsCreator>(m, "CgnsCreator")
    .def(
        py::init<GridDataPtr, std::string>(),
        py::arg("grid_data"),
        py::arg("output_path")
    )
    .def("GetFileName", &CgnsCreator::getFileName);
}
