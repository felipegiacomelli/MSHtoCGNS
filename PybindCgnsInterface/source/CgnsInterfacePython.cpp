#include <pybind11/pybind11.h>

#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsWriter.hpp"

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>)

void registerCgnsInterface(py::module& m) {
    py::class_<CgnsCreator>(m, "CgnsCreator")
    .def(
        py::init<GridDataPtr, std::string>(),
        py::arg("grid_data"),
        py::arg("output_path")
    )
    .def("GetFileName", &CgnsCreator::getFileName);

    py::class_<CgnsReader>(m, "CgnsReader")
    .def(
        py::init<std::string>(),
        py::arg("file_path")
    )
    .def_readwrite("grid_data", &CgnsReader::gridData);

    void (CgnsWriter::*writeSolutionString)(std::string) = &CgnsWriter::writeSolution;
    void (CgnsWriter::*writeSolutionDouble)(double) = &CgnsWriter::writeSolution;
    void (CgnsWriter::*writeFieldDouble)(std::string, const std::vector<double>&) = &CgnsWriter::writeField;
    void (CgnsWriter::*writeFieldList)(std::string, const std::vector<int>&) = &CgnsWriter::writeField;
    py::class_<CgnsWriter>(m, "CgnsWriter")
    .def(
        py::init<std::string, std::string>(),
        py::arg("file_path"),
        py::arg("grid_location") = "Vertex"
    )
    .def("WriteSolution", writeSolutionString, py::arg("name"))
    .def("WriteSolution", writeSolutionDouble, py::arg("time_value"))
    .def("WriteField", writeFieldDouble, py::arg("name"), py::arg("values"))
    .def("WriteField", writeFieldList, py::arg("name"), py::arg("values"))
    .def("FinalizeTransient", &CgnsWriter::finalizeTransient);
}
