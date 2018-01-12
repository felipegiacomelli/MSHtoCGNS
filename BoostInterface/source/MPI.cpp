#include <BoostInterface/MPI.hpp>

bool isMasterProcessor(const int& WorldRank) {
	return WorldRank == 0;
}