#include <pybind11/pybind11.h>

#include "MSHtoCGNS/CgnsInterface/CgnsWriter.hpp"

void registerCgnsWriter(pybind11::module& m) {
    void (CgnsWriter::*writeSolutionString)(std::string) = &CgnsWriter::writeSolution;
    void (CgnsWriter::*writeFieldList)(std::string, const boost::python::list&) = &CgnsWriter::writeField;

    pybind11::class_<CgnsWriter>(m, "CgnsWriter")
    .def(pybind11::init<std::string, std::string>())
    .def("writeSolution", writeSolutionString)
    .def("writeField", writeFieldList);
}
