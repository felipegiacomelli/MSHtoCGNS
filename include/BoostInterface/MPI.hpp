#ifndef MPI_HPP
#define MPI_HPP

#include <boost/mpi.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/access.hpp>

bool isMasterProcessor(const int&);

#endif