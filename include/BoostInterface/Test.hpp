#ifndef TEST_HPP
#define TEST_HPP

#include <boost/test/execution_monitor.hpp>
#include <boost/test/unit_test.hpp>

#define TestCase BOOST_AUTO_TEST_CASE
#define TestSuite BOOST_AUTO_TEST_SUITE
#define TestSuiteEnd BOOST_AUTO_TEST_SUITE_END

#define GlobalFixture BOOST_GLOBAL_FIXTURE
#define FixtureTestCase BOOST_FIXTURE_TEST_CASE
#define FixtureTestSuite BOOST_FIXTURE_TEST_SUITE

#define check BOOST_CHECK
#define checkClose BOOST_CHECK_CLOSE
#define checkEqual BOOST_CHECK_EQUAL

#endif
