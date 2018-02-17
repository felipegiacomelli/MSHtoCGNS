#ifndef GRID_DATA_HPP
#define GRID_DATA_HPP

#include <BoostInterface/MPI.hpp>
#include <BoostInterface/UniquePointer.hpp>
#include <BoostInterface/SharedPointer.hpp>

struct BoundaryData {
	friend class boost::serialization::access;

	std::vector<std::vector<int>> lineConnectivity;			
	std::vector<std::vector<int>> triangleConnectivity;		
	std::vector<std::vector<int>> quadrangleConnectivity;
	std::vector<int> verticesIndices;		
	std::string	name;	

	private:
		template<class Archive>
		void serialize(Archive &archive, const unsigned version) {
			archive &this->lineConnectivity;
			archive &this->triangleConnectivity;
			archive &this->quadrangleConnectivity;
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

	std::vector<BoundaryData> boundaries; 

	private:
		template<class Archive>
		void serialize(Archive & archive, const unsigned version) {
			archive &this->thickness;
			archive &this->dimension;
			archive &this->numberOfLocalVertices;
			archive &this->petscIndexes;
			archive &this->coordinates;
			archive &this->triangleConnectivity;
			archive &this->quadrangleConnectivity;
			archive &this->tetrahedronConnectivity;
			archive &this->hexahedronConnectivity;
			archive &this->boundaries;
		}
};

using GridDataShared = SharedPointer<GridData>;
using GridDataUnique = UniquePointer<GridData>;

#endif