#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <IO/GridReader.hpp>

int main() {
	GridReader reader("/home/felipe/Downloads/mesh.msh");
	GridData gridData = reader.getGridData();
	printGridData(gridData);
	return 0;
}