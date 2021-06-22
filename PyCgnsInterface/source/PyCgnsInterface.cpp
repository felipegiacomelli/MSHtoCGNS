#include "MSHtoCGNS/BoostInterface/Python.hpp"

#include "MSHtoCGNS/GridData/GridData.hpp"

#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsWriter.hpp"

namespace py = boost::python;

void appendCoordinate(GridDataPtr gridData, const boost::python::list& coordinates) {
    auto converted = std::vector<double>(boost::python::stl_input_iterator<double>(coordinates), boost::python::stl_input_iterator<double>());
    gridData->coordinates.emplace_back(std::array<double, 3>{converted[0], converted[1], converted[2]});
}

void appendConnectivity(GridDataPtr gridData, const boost::python::list& connectivity) {
    auto converted = std::vector<int>(boost::python::stl_input_iterator<int>(connectivity), boost::python::stl_input_iterator<int>());
    gridData->connectivities.emplace_back(converted);
}

BOOST_PYTHON_MODULE(PyCgnsInterface) {

    // py::class_<std::array<double, 3>>("Double3Array")
    //     .def(py::vector_indexing_suite<std::array<double, 3>>());

    // py::class_<std::vector<std::array<double, 3>>>("Double3ArrayVector")
    //     .def(py::vector_indexing_suite<std::vector<std::array<double, 3>>>());

    py::class_<std::vector<int>>("IntVector")
        .def(py::vector_indexing_suite<std::vector<int>>());

    py::class_<std::vector<std::vector<int>>>("IntVector2D")
        .def(py::vector_indexing_suite<std::vector<std::vector<int>>>());

    py::class_<std::vector<double>>("DoubleVector")
        .def(py::vector_indexing_suite<std::vector<double>>());

    py::class_<SectionData>(
        "SectionData",
        py::init<std::string, int, int, int>(
            (py::arg("name"), py::arg("dimension"), py::arg("begin"), py::arg("end"))
        )
    )
        .def_readwrite("name", &SectionData::name)
        .def_readwrite("dimension", &SectionData::dimension)
        .def_readwrite("begin", &SectionData::begin)
        .def_readwrite("end", &SectionData::end)
        .def_readwrite("vertices", &SectionData::vertices);

    py::class_<std::vector<SectionData>>("SectionDataVector")
        .def(py::vector_indexing_suite<std::vector<SectionData>>());

    py::class_<GridData, GridDataPtr>("GridData")
        .def_readwrite("dimension", &GridData::dimension)
        .def_readwrite("coordinates", &GridData::coordinates)
        .def_readwrite("connectivities", &GridData::connectivities)
        .def_readwrite("sections", &GridData::sections)
        .def_readwrite("numberOfLocalVertices", &GridData::numberOfLocalVertices);

    py::def("AppendCoordinate", &appendCoordinate, (py::arg("gridData"), py::arg("coordinate")));
    py::def("AppendConnectivity", &appendCoordinate, (py::arg("gridData"), py::arg("connectivity")));

    py::class_<CgnsCreator>(
        "CgnsCreator",
        py::init<GridDataPtr, std::string>(
            (py::arg("gridData"), py::arg("outputPath"))
        )
    )
        .def("getFileName", &CgnsCreator::getFileName);

    void (CgnsWriter::*writeSolutionString)(std::string) = &CgnsWriter::writeSolution;
    void (CgnsWriter::*writeSolutionDouble)(double) = &CgnsWriter::writeSolution;
    void (CgnsWriter::*writeFieldList)(std::string, const py::list&) = &CgnsWriter::writeField;

    py::class_<CgnsWriter>(
        "CgnsWriter",
        py::init<std::string, std::string>(
            (py::arg("filePath"), py::arg("gridLocation"))
        )
    )
        .def("writeSolution", writeSolutionString)
        .def("writeSolution", writeSolutionDouble)
        .def("writeField", writeFieldList)
        .def("finalizeTransient", &CgnsWriter::finalizeTransient);
}
