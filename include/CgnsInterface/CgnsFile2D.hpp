#ifndef CGNS_INTERFACE_CGNS_FILE_2D_HPP	
#define CGNS_INTERFACE_CGNS_FILE_2D_HPP

#include <iostream>
#include <string>
#include <vector>

#include <cgnslib.h>

#include <BoostInterface/Filesystem.hpp>
#include <Utilities/Vector.hpp>
#include <Grid/GridData.hpp>
#include <CgnsInterface/CgnsFile.hpp>

class CgnsFile2D : public CgnsFile {
	public:
		CgnsFile2D() = default;
		CgnsFile2D(const GridData&, const std::string&);

		~CgnsFile2D();

	private:
		void writeBase() override;
		void writeZone() override;
		void writeCoordinates() override;
		void writeSections() override;
		//void writeBoundaryConditions() override;
};

#endif