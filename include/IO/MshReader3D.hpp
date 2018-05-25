#ifndef IO_MSH_READER_3D_HPP
#define IO_MSH_READER_3D_HPP

#include <IO/MshReader.hpp>

class MshReader3D : public MshReader {
	public:
		MshReader3D(const std::string& filePath);

		~MshReader3D() = default;

	private:
		void readPhysicalEntities() override;
		void determineNumberOfFacets() override;
		void addRegions() override;
		void addBoundaries() override;
		void defineBoundaryVertices() override;
};

#endif