#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <IO/GridReader.hpp>

int main() {
	GridReader reader("/home/felipe/Downloads/mesh.msh");

	// std::cout << "Number of elements: "  << numberOfElements  << std::endl;
	// std::cout << "Lines             : " << numberOfLines     << std::endl;
	// std::cout << "Triangles         : " << numberOfTriangles << std::endl;

	//if (PRINT_COORDINATES) {
	//	std::cout << std::endl << "\tcoordinates" << std::endl;
	//	for (auto i = coordinates.cbegin(); i < coordinates.cend(); i++) {
	//		for (auto j = i->cbegin(); j < i->cend(); j++) {
	//			std::cout <<  std::setw(6) << std::scientific << std::showpos << std::setprecision(4) << "\t" << *j;
	//		}
	//		std::cout << std::endl;
	//	}
	//}
	//if (PRINT_ELEMENTS) {
	//	std::cout << std::endl << "\telements" << std::endl;
	//	for (auto i = elements.cbegin(); i < elements.cend(); i++) {
	//		for (auto j = i->cbegin(); j < i->cend(); j++) {
	//			std::cout <<  std::setw(4) << std::right << "\t" << *j;
	//		}
	//		std::cout << std::endl;
	//	}
	//}

	return 0;
}