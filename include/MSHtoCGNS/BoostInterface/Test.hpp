#ifndef __BOOST_INTERFACE_TEST_HPP__
#define __BOOST_INTERFACE_TEST_HPP__

#include <boost/test/execution_monitor.hpp>
#include <boost/test/unit_test.hpp>

#define TestCase BOOST_AUTO_TEST_CASE
#define TestSuite BOOST_AUTO_TEST_SUITE
#define TestSuiteEnd BOOST_AUTO_TEST_SUITE_END

#define FixtureTestCase BOOST_FIXTURE_TEST_CASE
#define FixtureTestSuite BOOST_FIXTURE_TEST_SUITE
#define TestGlobalFixture BOOST_TEST_GLOBAL_FIXTURE

#define checkClose BOOST_CHECK_CLOSE
#define checkEqual BOOST_CHECK_EQUAL
#define checkSmall BOOST_CHECK_SMALL

#define checkEqualCollections BOOST_CHECK_EQUAL_COLLECTIONS

#define checkCloseCollection(aa, bb, tolerance) { \
    auto a = std::begin(aa), ae = std::end(aa); \
    auto b = std::begin(bb); \
    BOOST_REQUIRE_EQUAL(std::distance(a, ae), std::distance(b, std::end(bb))); \
    for(; a != ae; ++a, ++b) { \
        BOOST_CHECK_CLOSE(*a, *b, tolerance); \
    } \
}

#endif
