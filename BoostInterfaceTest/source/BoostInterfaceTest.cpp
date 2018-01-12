#define BOOST_TEST_MODULE BoostInterfaceTestModule

#include <BoostInterface/Test.hpp>
#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/MPI.hpp>
#include <BoostInterface/Foreach.hpp>

TestSuite(Filesystem)
	TestCase(CreateDirectory) {
		std::string Path = "Test/";
		createDirectory(Path);
		
		check(exists(Path) == true);
	}

	TestCase(DeleteDirectory) {
		std::string Path = "Test/";
		deleteDirectory(Path);
		
		check(exists(Path) == false);
	}
TestSuiteEnd()