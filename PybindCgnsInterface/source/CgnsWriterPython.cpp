#include <pybind11/pybind11.h>

#include "MSHtoCGNS/CgnsInterface/CgnsWriter.hpp"

namespace py = pybind11;

void registerCgnsWriter(py::module& m) {
    void (CgnsWriter::*writeSolutionString)(std::string) = &CgnsWriter::writeSolution;
    void (CgnsWriter::*writeFieldList)(std::string, const boost::python::list&) = &CgnsWriter::writeField;

    py::class_<CgnsWriter>(m, "CgnsWriter")
    .def(py::init<std::string, std::string>())
    .def("writeSolution", writeSolutionString)
    .def("writeField", writeFieldList);
}
