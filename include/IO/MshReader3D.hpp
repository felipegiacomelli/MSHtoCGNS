#ifndef MSH_READER_3D_HPP
#define MSH_READER_3D_HPP

#include <IO/MshReader.hpp>

class MshReader3D : public MshReader {
	public:
		MshReader3D() = default;
		MshReader3D(const std::string&);

	private:
		void readPhysicalEntities() override;
		void addElements() override;
		void defineBoundaryVerticesIndices() override;
	};

#endif