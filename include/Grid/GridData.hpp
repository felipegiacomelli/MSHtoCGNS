#ifndef GRID_DATA_HPP
#define GRID_DATA_HPP

#include <BoostInterface/MPI.hpp>
#include <BoostInterface/UniquePointer.hpp>
#include <BoostInterface/SharedPointer.hpp>

struct BoundaryGridData {
	friend class boost::serialization::access;

	std::vector<std::vector<int>>	lineConnectivity;			
	std::vector<std::vector<int>>	triangleConnectivity;		
	std::vector<std::vector<int>>	quadrangleConnectivity;		
	std::string	name;	

	private:
		template< class Archive_ >
		void serialize(Archive_ &archive, const unsigned int version) {
			archive &this->lineConnectivity;
			archive &this->triangleConnectivity;
			archive &this->quadrangleConnectivity;
			archive &this->name;
		}
};

struct WellGridData {
	friend class boost::serialization::access;

	int	wellNode;			
	std::vector<std::vector<int>>	lineConnectivity;	
	std::string	name;	

	private:
		template< class Archive_ >
		void serialize(Archive_ &archive, const unsigned int version) {
			archive &this->wellNode;
			archive &this->lineConnectivity;
			archive &this->name;
		}
};

struct RegionData {
	friend class boost::serialization::access;

	std::vector<int> elementsOnRegion;	
	std::string	name;			

	private:
		template< class Archive_ >
		void serialize(Archive_ &archive, const unsigned int version) {
			archive &this->elementsOnRegion;
			archive &this->name;
		}
};

struct GridData {
	friend class boost::serialization::access;

	GridData() : numberOfLocalVertices(-1) {}

	int dimension;		
	double thickness;		
	int numberOfLocalVertices;
	std::vector<int> petscIndexes;

	std::vector<std::vector<double>> coordinates;				
	std::vector<std::vector<int>> triangleConnectivity;		
	std::vector<std::vector<int>> quadrangleConnectivity;		
	std::vector<std::vector<int>> tetrahedronConnectivity;	
	std::vector<std::vector<int>> hexahedronConnectivity;		
	std::vector<std::vector<int>> pyramidConnectivity;		
	std::vector<std::vector<int>> prismConnectivity;			

	std::vector<BoundaryGridData> boundaries; 
	std::vector<WellGridData> wells;		
	std::vector<RegionData> regions;	

	private:
		template< class Archive_ >
		void serialize(Archive_ & archive, const unsigned int version) {
			archive &this->thickness;
			archive &this->dimension;
			archive &this->numberOfLocalVertices;
			archive &this->petscIndexes;
			archive &this->coordinates;
			archive &this->triangleConnectivity;
			archive &this->quadrangleConnectivity;
			archive &this->tetrahedronConnectivity;
			archive &this->hexahedronConnectivity;
			archive &this->pyramidConnectivity;
			archive &this->prismConnectivity;
			archive &this->boundaries;
			archive &this->wells;
			archive &this->regions;
		}
};

using GridDataShared = SharedPointer<GridData>;
using GridDataUnique = UniquePointer<GridData>;

#endif