#include "MSHtoCGNS/CgnsInterface/CgnsOpener.hpp"
#include <cgnslib.h>
#include <cgns_io.h>

CgnsOpener::CgnsOpener(std::string file, std::string mode) : file(file), temporaryFile(file + ".temp") {
    this->setMode(mode);
    this->checkFile();
    this->openFile();
    this->readNumberOfBases();
    this->readBase();
    this->readZone();
}

void CgnsOpener::setMode(std::string mode) {
    if (mode == std::string("Read"))
        this->mode = 0;
    else if (mode == std::string("Modify"))
        this->mode = 2;
    else
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Mode must be either Read or Modify"));
}

void CgnsOpener::checkFile() {
    if (!boost::filesystem::exists(this->file.parent_path()))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "The parent path " + this->file.parent_path().string() + " does not exist"));

    if (!boost::filesystem::exists(this->file))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, + "There is no file in path " + boost::filesystem::absolute(this->file).string()));

    if (cg_is_cgns(boost::filesystem::absolute(this->file).c_str(), &this->fileType))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "File " + boost::filesystem::absolute(this->file).string() + " is not a valid cgns file", cg_get_error()));

    if (this->fileType == 1)
        this->convertToHdf5();
}

void CgnsOpener::convertToHdf5() {
    int adfFileIndex;
    if (cgio_open_file(boost::filesystem::absolute(this->file).c_str(), CGIO_MODE_READ, CGIO_FILE_ADF, &adfFileIndex))
        cgio_error_exit("cgio_open_file (adf)");

    int hdfFileIndex;
    if (cgio_open_file(boost::filesystem::absolute(this->temporaryFile).c_str(), CGIO_MODE_WRITE, CGIO_FILE_HDF5, &hdfFileIndex))
        cgio_error_exit("cgio_open_file (hdf5)");

    if (cgio_copy_file(adfFileIndex, hdfFileIndex, 1))
        cgio_error_exit("cgio_copy_file");

    if (cgio_close_file(adfFileIndex))
        cgio_error_exit("cgio_close_file (adf)");

    if (cgio_close_file(hdfFileIndex))
        cgio_error_exit("cgio_close_file (hdf5)");

    boost::filesystem::copy_file(this->temporaryFile, this->file, boost::filesystem::copy_option::overwrite_if_exists);
    boost::filesystem::remove_all(this->temporaryFile);
}

void CgnsOpener::openFile() {
    if (cg_open(boost::filesystem::absolute(this->file).c_str(), this->mode, &this->fileIndex))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not open file " + boost::filesystem::absolute(this->file).string(), cg_get_error()));

    if (cg_version(this->fileIndex, &this->fileVersion))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not read file version", cg_get_error()));

    if (this->fileVersion <= 3.10)
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "File version (" + std::to_string(this->fileVersion) + ") is older than 3.11"));
}

void CgnsOpener::readNumberOfBases() {
    if (cg_nbases(this->fileIndex, &this->numberOfBases))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not read number of bases"));

    if (this->numberOfBases < 1)
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "The CGNS file has no base"));
}

void CgnsOpener::readBase() {
    if (cg_base_read(this->fileIndex, this->baseIndex, this->buffer, &this->cellDimension, &this->physicalDimension))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not read base"));
}

void CgnsOpener::readZone() {
    if (cg_nzones(this->fileIndex, this->baseIndex, &this->zoneIndex))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not read number of zones"));

    if (this->zoneIndex != 1)
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "The CGNS file has more than one zone"));

    ZoneType_t zoneType;
    if (cg_zone_type(this->fileIndex, this->baseIndex, this->zoneIndex, &zoneType))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not read zone type"));

    if (zoneType != Unstructured)
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Only unstructured zones are supported"));

    if (cg_zone_read(this->fileIndex, this->baseIndex, this->zoneIndex, this->buffer, this->sizes))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not read zone"));
}

CgnsOpener::~CgnsOpener() {
    cg_close(this->fileIndex);
}
