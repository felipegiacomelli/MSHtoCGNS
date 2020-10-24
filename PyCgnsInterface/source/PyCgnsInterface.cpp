#include "MSHtoCGNS/PyCgnsInterface/PyCgnsInterface.hpp"

BOOST_PYTHON_MODULE(PyCgnsInterface) {
    boost::python::class_<std::vector<int>>("IntVector")
        .def(boost::python::vector_indexing_suite<std::vector<int>>());

    boost::python::class_<std::vector<double>>("DoubleVector")
        .def(boost::python::vector_indexing_suite<std::vector<double>>());

    void (CgnsWriter::*writeSolutionString)(std::string) = &CgnsWriter::writeSolution;
    void (CgnsWriter::*writeSolutionDouble)(double) = &CgnsWriter::writeSolution;
    void (CgnsWriter::*writeFieldList)(std::string, const boost::python::list&) = &CgnsWriter::writeField;

    boost::python::class_<CgnsWriter>("CgnsWriter", boost::python::init<std::string, std::string>())
        .def("writeSolution", writeSolutionString)
        .def("writeSolution", writeSolutionDouble)
        .def("writeField", writeFieldList)
        .def("finalizeTransient", &CgnsWriter::finalizeTransient);
}
