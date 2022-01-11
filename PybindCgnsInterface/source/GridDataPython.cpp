#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "MSHtoCGNS/GridData/GridData.hpp"

PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>)

namespace py = pybind11;

void registerGridData(py::module& m) {
    py::class_<SectionData>(m, "SectionData")
        .def(py::init<std::string, int, int, int>(),
                py::arg("name"), py::arg("dimension"), py::arg("begin"), py::arg("end")
        )
        .def_readwrite("name", &SectionData::name)
        .def_readwrite("dimension", &SectionData::dimension)
        .def_readwrite("begin", &SectionData::begin)
        .def_readwrite("end", &SectionData::end)
        .def_readwrite("vertices", &SectionData::vertices);

    py::class_<GridData, GridDataPtr>(m, "GridData")
        .def(py::init<int>(), py::arg("dimension"))
        .def_readwrite("dimension", &GridData::dimension)
        .def_readwrite("coordinates", &GridData::coordinates)
        .def_readwrite("connectivities", &GridData::connectivities)
        .def_readwrite("sections", &GridData::sections)
        .def_readwrite("number_of_local_vertices", &GridData::numberOfLocalVertices);
}
