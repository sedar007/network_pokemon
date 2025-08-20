#include <boost/test/unit_test.hpp>
#include <networkPokemon.h>

BOOST_AUTO_TEST_SUITE(ModbusMasterTests)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
	pokemon::modbus_master master;

	BOOST_TEST(3 == 3);
}

BOOST_AUTO_TEST_CASE(LoadFromFile)
{
	pokemon::modbus_master master;

	bool succeeded = master.load_from_file();

	BOOST_CHECK_EQUAL(true, succeeded);
}

BOOST_AUTO_TEST_SUITE_END()
