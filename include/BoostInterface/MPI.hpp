#ifndef MPI_HPP
#define MPI_HPP

#include <boost/mpi.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/access.hpp>

using namespace boost;
using MPICommunicator = boost::mpi::communicator;
using MPIEnvironment = boost::mpi::environment;

bool isMasterProcessor(const int&);

#endif