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

void appendSection(GridDataPtr gridData, const SectionData& sectionData) {
    gridData->sections.emplace_back(sectionData);
}

BOOST_PYTHON_MODULE(PyCgnsInterface) {
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

    py::class_<GridData, GridDataPtr>("GridData", py::init<int>((py::arg("dimension"))))
        .def_readwrite("dimension", &GridData::dimension)
        .def_readwrite("coordinates", &GridData::coordinates)
        .def_readwrite("connectivities", &GridData::connectivities)
        .def_readwrite("sections", &GridData::sections)
        .def_readwrite("number_of_local_vertices", &GridData::numberOfLocalVertices);

    py::def("AppendCoordinate", &appendCoordinate, (py::arg("grid_data"), py::arg("coordinate")));
    py::def("AppendConnectivity", &appendConnectivity, (py::arg("grid_data"), py::arg("connectivity")));
    py::def("AppendSection", &appendSection, (py::arg("grid_data"), py::arg("section_data")));

    py::class_<CgnsCreator>(
        "CgnsCreator",
        py::init<GridDataPtr, std::string>(
            (py::arg("grid_data"), py::arg("output_path"))
        )
    )
        .def("GetFileName", &CgnsCreator::getFileName);

    void (CgnsWriter::*writeSolutionString)(std::string) = &CgnsWriter::writeSolution;
    void (CgnsWriter::*writeSolutionDouble)(double) = &CgnsWriter::writeSolution;
    void (CgnsWriter::*writeFieldList)(std::string, const py::list&) = &CgnsWriter::writeField;

    py::class_<CgnsWriter>(
        "CgnsWriter",
        py::init<std::string, std::string>(
            (py::arg("file_path"), py::arg("grid_location"))
        )
    )
        .def("WriteSolution", writeSolutionString)
        .def("WriteSolution", writeSolutionDouble)
        .def("WriteField", writeFieldList)
        .def("FinalizeTransient", &CgnsWriter::finalizeTransient);
}
