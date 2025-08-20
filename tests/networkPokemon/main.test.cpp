#include <boost/test/unit_test.hpp>
#include <networkPokemon.h>

BOOST_AUTO_TEST_SUITE(MainTests)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
	pokemon::Test test;

	BOOST_TEST(3 == 3);
}

BOOST_AUTO_TEST_CASE(LoadFromFile)
{
	pokemon::Test master;


	master.setAge(10);
	BOOST_CHECK_EQUAL(master.getAge(), 10);
	master.setName("TestName");
	BOOST_CHECK_EQUAL(master.getName(), "TestName");
}

BOOST_AUTO_TEST_SUITE_END()
