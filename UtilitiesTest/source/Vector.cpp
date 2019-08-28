#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"

TestCase(IteratorAppend) {
    std::vector<std::vector<int>> a{{10, 10, 20}, {30, 40, 50}, {60, 70, 80}};

    std::vector<int> b;
    append(a.cbegin(), a.cend(), std::back_inserter(b));

    checkEqual(b[0], 10);
    checkEqual(b[1], 10);
    checkEqual(b[2], 20);
    checkEqual(b[3], 30);
    checkEqual(b[4], 40);
    checkEqual(b[5], 50);
    checkEqual(b[6], 60);
    checkEqual(b[7], 70);
    checkEqual(b[8], 80);
}

TestCase(VectorAppend) {
    std::vector<std::array<int, 3>> a{std::array<int, 3>{10, 20, 30}, std::array<int, 3>{40, 50, 60}};

    std::vector<std::vector<int>> b;
    append(a, b);

    checkEqual(b[0][0], 10);
    checkEqual(b[0][1], 20);
    checkEqual(b[0][2], 30);
    checkEqual(b[1][0], 40);
    checkEqual(b[1][1], 50);
    checkEqual(b[1][2], 60);
}
