#include <BoostInterface/MPI.hpp>

bool isMasterProcessor(const int& worldRank) {
	return worldRank == 0;
}