#include <BoostInterface/Test.hpp>
#include <BoostInterface/Filesystem.hpp>

TestSuite(Filesystem)

TestCase(CreateDirectory) {
    std::string directory = "Test/";
    createDirectory(directory);

    check(boost::filesystem::exists(directory));
}

TestCase(DeleteDirectory) {
    std::string directory = "Test/";
    deleteDirectory(directory);

    check(!boost::filesystem::exists(directory));
}

TestCase(CreateDirectories) {
    std::string directories = "Test/Test";
    createDirectories(directories);

    check(boost::filesystem::exists(directories));

    deleteDirectory("Test/");
}

TestSuiteEnd()
