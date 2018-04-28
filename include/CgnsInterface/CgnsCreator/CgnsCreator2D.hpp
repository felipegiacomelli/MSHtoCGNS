#ifndef CGNS_FILE_2D_HPP	
#define CGNS_FILE_2D_HPP

#include <CgnsInterface/CgnsCreator/CgnsCreator.hpp>

class CgnsCreator2D : public CgnsCreator {
	public:
		CgnsCreator2D() = default;
		CgnsCreator2D(GridDataShared, const std::string&);

		~CgnsCreator2D() = default;

	private:
		void setupFile() override;
		void writeCoordinates() override;
		void writeSections() override;
};

#endif