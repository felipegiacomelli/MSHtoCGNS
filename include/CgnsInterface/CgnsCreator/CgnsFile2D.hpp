#ifndef CGNS_FILE_2D_HPP	
#define CGNS_FILE_2D_HPP

#include <CgnsInterface/CgnsCreator/CgnsFile.hpp>

class CgnsFile2D : public CgnsFile {
	public:
		CgnsFile2D() = default;
		CgnsFile2D(GridDataShared, const std::string&);

		~CgnsFile2D() = default;

	private:
		void setupFile() override;
		void writeCoordinates() override;
		void writeSections() override;
};

#endif